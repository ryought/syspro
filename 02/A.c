#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

int main(){
  struct timeval *tp;
  pid_t pid;
  int i;

  
  //gettimeofday(tp, NULL);
  //printf("%ld %d\n", tp->tv_sec, tp->tv_usec);

  for(i=0; i<10; i++){
    pid = getpid();
    printf("%d\n", pid);
  }

  /* gettimeofday(tp, NULL); */
  /* printf("%ld %d\n", tp->tv_sec, tp->tv_usec); */

  /* for(i=0; i<1000; i++){ */
  /*   pid = getpid(); */
  /* } */

  /* gettimeofday(tp, NULL); */
  /* printf("%ld %d\n", tp->tv_sec, tp->tv_usec); */
  
  
  return 0;
}
