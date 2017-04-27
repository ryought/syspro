#define _REENTRANT

#include "bb.h"
#include <stdio.h>
#include <pthread.h>

// ひたすら200個をぷっとしていくスレッド
void *f(void *arg){
  BBuffer *b = (BBuffer *)arg;
  int i;

  printf("[t] get %d\n", bb_get(*b));
  
  pthread_exit(NULL);
}


int main() {
  pthread_t thread;
  BBuffer b = bb_create();
  int i;

  if (pthread_create(&thread, NULL, f, &b) != 0){
    return 1;
  }

  for (i=0; i<1000; i++) {
    printf(".");
  }
  printf("\n");

  printf("[m] put 10\n");
  bb_put(b, 10);

  if (pthread_join(thread, NULL) != 0) {
    return 1;
  }
  
  return 0;
}
