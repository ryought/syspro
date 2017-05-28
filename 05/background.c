// fork()時のbackgroundとforegroundの違い
//  setpgid()してもforegroundのidはそのままだから、変更した側はbackgroundになる


#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>


// signalハンドラ
void handler(int signal) {
  printf("called!!\n");
}


int main() {
  pid_t pid;
  printf("(before fork) pid %d, pgid %d, ppid %d\n", getpid(), getpgid(0), getppid());

  struct sigaction si;
  si.sa_handler = handler; // 自作ハンドラへのポインタ
  // SIG_DFL, SIG_IGN は それぞれデフォルト、無視ハンドラへのポインタ
  si.sa_flags = 0; // フラグはなし

  sigaction(SIGTTOU, &si, NULL); // old_siは指定なし

  
  
  if((pid = fork()) == 0){
    // child
    printf("(child) pid %d, pgid %d, ppid %d\n", getpid(), getpgid(pid), getppid());

    printf("%d\n", setpgid(getpid(), getpid())); //分岐

    printf("(pg create child) pid %d, pgid %d, ppid %d\n", getpid(), getpgid(0), getppid());
    printf("(before backgroundize)%d\n", tcgetpgrp(0));

    printf("%d\n", tcsetpgrp(0, getpgid(0)));

    printf("(after)%d\n", tcgetpgrp(0));

    sleep(5); 
    
  } else {
    // parent
    int status;

    printf("(parent) pid %d, pgid %d, ppid %d\n", getpid(), getpgid(0), getppid());
    printf("(foreground?) %d\n", tcgetpgrp(0));

    printf("hoge\n");

    sleep(2);

    printf("hoge\n");

    sleep(2);

    printf("hoge\n");
    waitpid(pid, &status, WUNTRACED);
  }

  return 0;
}
