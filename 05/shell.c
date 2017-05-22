// 課題B, C シェル
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "parser/parse.h" // パーサ
 
int main(int argc, char* argv[], char *envp[]) {
  pid_t pid;
  char* path = argv[1];

  char s[LINELEN];
  job *curr_job;

  while(get_line(s, LINELEN)){
    curr_job = parse_line(s);

    if((pid=fork()) == 0){ // child process作成
      // child側処理 0が入る
      int ret = execve(curr_job->process_list->program_name,
                       curr_job->process_list->argument_list, envp);
      printf("ret = %d\n", ret);
    } else {
      // parent側 pidには子プロセスのがはいってる
      int status;
      waitpid(pid, &status, WUNTRACED); // 子供は終了時、親にstatusを返さないといけない。それを待ち受ける。
    }
  }
  return 0;
}
