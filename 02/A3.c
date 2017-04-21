// getpid(2)を複数回実行するサンプル
//  using syscall()
//  こっちだとちゃんとgetpidが10001回呼び出される。
#include <sys/types.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>

int main(){
  struct timeval start, end;
  pid_t pid;
  int i;

  gettimeofday(&start, NULL);

  for(i=0; i<1; i++){
    syscall(SYS_getpid);
  }

  gettimeofday(&end, NULL);
  
  printf("%ld\n", 
    (end.tv_sec-start.tv_sec)*1000 + (end.tv_usec-start.tv_usec));
  
  
  
  gettimeofday(&start, NULL);

  for(i=0; i<10000; i++){
    syscall(SYS_getpid);
  }

  gettimeofday(&end, NULL);
  
  printf("%ld\n", 
    (end.tv_sec-start.tv_sec)*1000 + (end.tv_usec-start.tv_usec));
  
 

  return 0;
}

