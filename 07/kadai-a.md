# カーネルソースを読む


getpgidを読む


sys.c
実体 syscall_defineでシステムコールとして登録してる。
ユーザーがSYSCALL(getpgid)で呼び出すと、これで登録されたプログラムが呼ばれる。
pid_tは型？
結局return pid_vnr(task_pgrp(current / task))をやってる

SYSCALL_DEFINE1(getpgid, pid_t, pid)
{
 	struct task_struct *p;
	struct pid *grp;
	int retval;

	rcu_read_lock();
	if (!pid)
		grp = task_pgrp(current); // currentはプロセス構造体
	else {
		retval = -ESRCH;
		p = find_task_by_vpid(pid);
		if (!p)
			goto out;
		grp = task_pgrp(p);
		if (!grp)
			goto out;

		retval = security_task_getpgid(p);
		if (retval)
			goto out;
	}
	retval = pid_vnr(grp);
out:
	rcu_read_unlock();
	return retval;  // pid_vnr(grp)を返してる
}



task_pgrp(task)は、グループリーダーのpidを返す
sched.h
static inline struct pid *task_pgrp(struct task_struct *task)
{
	return task->group_leader->pids[PIDTYPE_PGID].pid;
}

task_structは実行中のタスク(プロセスとほとんど同じ意味？)の状態をしまっておく。




pid_vnr(grp)は、pidを受け取って(pid_nr_nsを経由して)upid->nrを返す(upidはpid.numbers[ns->level])
pid.c
pid_t pid_vnr(struct pid *pid)
{
	return pid_nr_ns(pid, current->nsproxy->pid_ns);
}

pid_t pid_nr_ns(struct pid *pid, struct pid_namespace *ns)
{
	struct upid *upid;
	pid_t nr = 0;

	if (pid && ns->level <= pid->level) {
		upid = &pid->numbers[ns->level];
		if (upid->ns == ns)
			nr = upid->nr;
	}
	return nr;
}

これでコールの全貌。
struct upidの意味がわからない。




参考までにSYSCALL_DEFINEマクロの定義について

syscalls.h - SYSCALL_DEFINEのマクロが引数別にある

#define SYSCALL_DEFINE1(name, ...) SYSCALL_DEFINEx(1, _##name, __VA_ARGS__)
#define SYSCALL_DEFINE2(name, ...) SYSCALL_DEFINEx(2, _##name, __VA_ARGS__)
#define SYSCALL_DEFINE3(name, ...) SYSCALL_DEFINEx(3, _##name, __VA_ARGS__)
#define SYSCALL_DEFINE4(name, ...) SYSCALL_DEFINEx(4, _##name, __VA_ARGS__)
#define SYSCALL_DEFINE5(name, ...) SYSCALL_DEFINEx(5, _##name, __VA_ARGS__)
#define SYSCALL_DEFINE6(name, ...) SYSCALL_DEFINEx(6, _##name, __VA_ARGS__)

引数xの時のやつは次にある

```
#ifdef CONFIG_FTRACE_SYSCALLS
#define SYSCALL_DEFINEx(x, sname, ...)				\
	static const char *types_##sname[] = {			\
		__SC_STR_TDECL##x(__VA_ARGS__)			\
	};							\
	static const char *args_##sname[] = {			\
		__SC_STR_ADECL##x(__VA_ARGS__)			\
	};							\
	SYSCALL_METADATA(sname, x);				\
	__SYSCALL_DEFINEx(x, sname, __VA_ARGS__)
#else
#define SYSCALL_DEFINEx(x, sname, ...)				\
	__SYSCALL_DEFINEx(x, sname, __VA_ARGS__)
#endif
```

よくわかんないけど、引数を展開した後、__SYSCALL_DEFINExがテーブルに登録する感じなんだろうか。 



## syscallがユーザーから呼ばれる時の流れ



## エントリーポイント
呼び出す名前 数字
syscall_table_32.S  -  アセンブリ構造(実行コード) 関数のテーブル

entry_32.S - 実行


unistd.h - 関数名とテーブルの位置の表


## 関数実体
kernel/hoge.c


asmlinkage int sys_hoge() {}

で定義




## 関数の実行

``` assembly
/*
 * syscall stub including irq exit should be protected against kprobes
 */
	.pushsection .kprobes.text, "ax"
	# system call handler stub
ENTRY(system_call)
	RING0_INT_FRAME			# can't unwind into user space anyway
	pushl %eax			# save orig_eax
	CFI_ADJUST_CFA_OFFSET 4
	SAVE_ALL
	GET_THREAD_INFO(%ebp)
					# system call tracing in operation / emulation
	testl $_TIF_WORK_SYSCALL_ENTRY,TI_flags(%ebp)
	jnz syscall_trace_entry
	cmpl $(nr_syscalls), %eax
	jae syscall_badsys
syscall_call:
	call *sys_call_table(,%eax,4)
	movl %eax,PT_EAX(%esp)		# store the return value
syscall_exit:
	LOCKDEP_SYS_EXIT
	DISABLE_INTERRUPTS(CLBR_ANY)	# make sure we don't miss an interrupt
					# setting need_resched or sigpending
					# between sampling and the iret
	TRACE_IRQS_OFF
	movl TI_flags(%ebp), %ecx
	testl $_TIF_ALLWORK_MASK, %ecx	# current->work
	jne syscall_exit_work

```

