// 課題A 実行するだけ
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[], char *envp[]) {
  pid_t pid;
  char* path = argv[1];
  
  if((pid=fork()) == 0){ // child process作成
    // child 0が入る
    printf("path:%s", path);
    int ret = execve(path, &argv[1], envp);
    printf("ret = %d\n", ret);
  } else {
    // parent pidには子プロセスのがはいってる
    int status;
    waitpid(pid, &status, WUNTRACED); // 子供は終了時、親にstatusを返さないといけない。それを待ち受ける。
  }
  return 0;
  
}
