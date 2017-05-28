#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  pid_t stdin_PGID;
  pid_t pid;

  if((pid = fork()) == 0){
    setpgid(pid, pid);
    printf("The PGID has been changed to %d %d\n", tcgetpgrp(0), getpid());
    printf("%d\n", getpgid(pid));
    tcsetpgrp(0, getpgid(pid));
    tcsetpgrp(1, getpgid(pid));
    printf("hoge\n");
    printf("The PGID has been changed to %d %d\n", tcgetpgrp(0), getpid());

  } else {
    int status;
    waitpid(pid, &status, WUNTRACED);
    printf("BASE The PGID has been changed to %d %d\n", tcgetpgrp(0), getpid());
  }
  
  
  return(EXIT_SUCCESS);
}
