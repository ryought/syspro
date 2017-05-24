// 2回forkするexample

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[], char *envp[]) {
  pid_t pid1, pid2;
  
  if((pid1=fork()) == 0){ // child process作成
    // child 0が入る
    printf("execve1\n");
    int ret = execve("a.out", NULL, envp);
    printf("ret = %d\n", ret);
    
  } else {
    // parent pidには子プロセスのがはいってる
    printf("parent\n");
    int status;
    waitpid(pid1, &status, WUNTRACED); // 子供は終了時、親にstatusを返さないといけない。それを待ち受ける。
  }

  printf("passed\n");

  if((pid2=fork()) == 0){
    printf("execve2\n");
    int ret2 = execve("a.out", NULL, envp);
    printf("ret2 = %d\n", ret2);
  } else {
    printf("parent2\n");
    int status2;
    waitpid(pid2, &status2, WUNTRACED); // 子供は終了時、親にstatusを返さないといけない。それを待ち受ける。
  }

  printf("exit\n");
  return 0;
  
}
