// pipeとdup2を組み合わせる例
//  ls | wc  をやってる。
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
 
int main(int argc, char* argv[], char *envp[]) {
  pid_t pid1, pid2;
  
  int fd[2];
  pipe(fd);

  if((pid1=fork()) == 0){ // child process作成
    // child側処理 0が入る
    dup2(fd[1], 1); // fd[1]は1(stdout)の複製
    close(fd[0]); // fd[0]は使わないので閉じる
    execve("/bin/ls", &argv[0], envp);
    return -1;
  } else {
    // parent側 pidには子プロセスのがはいってる
    close(fd[1]); // fd[1]は使い終えたので閉じる。クローズするとEOFが書き込まれる
    int status1;
    waitpid(pid1, &status1, WUNTRACED); // 子供は終了時、親にstatusを返さないといけない。それを待ち受ける。
  }
    

  if((pid2=fork()) == 0){
    // child
    dup2(fd[0], 0);
    close(fd[1]);
    //char buf[1024];
    //read(0, buf, 1024);
    //printf("yahoo2 %s\n", buf);    
    execve("/usr/bin/wc", &argv[0], envp);
    return -1;
  }else{
    // parent側 pidには子プロセスのがはいってる
    close(fd[0]);
    int status2;
    waitpid(pid2, &status2, WUNTRACED); // 子供は終了時、親にstatusを返さないといけない。それを待ち受ける。
  }
  return 0;
}
