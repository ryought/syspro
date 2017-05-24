// select システムコールのサンプル
// 5秒間入力を待って、読めたかどうかの通知

#include <stdio.h>
//#include <stdlib.h>
#include <sys/time.h> // struct timevalの定義


int main(){
  fd_set rfds;

  // 集合の操作
  FD_ZERO(&rfds); 
  FD_SET(0, &rfds);

  struct timeval tv;
  tv.tv_sec = 5;
  tv.tv_usec = 0;

  int i;
  i = select(1, &rfds, NULL, NULL, &tv);

  if ( i == -1 )
    printf("error");
  else if (i)
    printf("caught %d", i);

  return 0;
}
