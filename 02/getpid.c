#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

int main(){
  struct timeval *tp_before, *tp_after;
  gettimeofday(tp_before, NULL);
  
  pid_t pid;

  for(int i=0; i<1000; i++){
    pid = getpid();
  }

  gettimeofday(tp_after, NULL);

  printf("%ld %06d\n", tp_before->tv_sec, tp_before->tv_usec);
  printf("%ld %06d\n", tp_after->tv_sec, tp_after->tv_usec);
  
  return 0;
}
