// getpid(2)を複数回実行するサンプル
// 直接呼び出しの場合
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

int main(){
  struct timeval start, end;
  pid_t pid;
  int i;

  gettimeofday(&start, NULL);

  for(i=0; i<1; i++){
    pid = getpid();
  }

  gettimeofday(&end, NULL);
  
  printf("%ld\n", 
    (end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
  
  
  
  gettimeofday(&start, NULL);

  for(i=0; i<10000; i++){
    pid = getpid();
  }

  gettimeofday(&end, NULL);
  printf("%d\n", pid);
  printf("%ld\n", 
    (end.tv_sec-start.tv_sec)*1000 + (end.tv_usec-start.tv_usec));
  
 

  return 0;
}

