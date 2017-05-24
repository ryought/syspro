// dup2で、execveで実行するプログラムのstdin, stdout先を変える
#include <stdio.h>
#include <unistd.h> // dup2
#include <fcntl.h> // open
#include <sys/wait.h>

int main(int argc, char* argv[], char *envp[]) {
  pid_t pid;
  char* path = argv[1];
  
  if((pid=fork()) == 0){ // child process作成
    // child 0が入る
    printf("path:%s", path);
    int fd = open("test", O_RDONLY);
    dup2(fd, 0);
    int ret = execve(path, &argv[1], envp);
    printf("ret = %d\n", ret);
  } else {
    // parent pidには子プロセスのがはいってる
    int status;
    waitpid(pid, &status, WUNTRACED); // 子供は終了時、親にstatusを返さないといけない。それを待ち受ける。
  }
  return 0;
  
}
