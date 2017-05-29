
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

  struct sigaction si;
  si.sa_handler = handler; // 自作ハンドラへのポインタ
  // SIG_DFL, SIG_IGN は それぞれデフォルト、無視ハンドラへのポインタ
  si.sa_flags = 0; // フラグはなし

  sigaction(SIGCONT, &si, NULL); // old_siは指定なし

  
  
  if((pid = fork()) == 0){
    // child
    
    sleep(5);

    printf("finished\n");
    
    
  } else {
    // parent
    sleep(2);

    kill(pid, SIGSTOP);


    sleep(10);

    
    kill(pid, SIGCONT);

    int status;
    waitpid(pid, &status, WUNTRACED);
  }

  return 0;
}
