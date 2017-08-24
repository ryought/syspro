#define _REENTRANT

#include "bb.h"
#include <stdio.h>
#include <pthread.h>

// ひたすら200個をぷっとしていくスレッド
void *f1(void *arg){
  BBuffer *b = (BBuffer *)arg;
  for(int i=0; i<100; i++){
    bb_put(*b, 1);
    printf("[t1] put 1\n");
  }
  pthread_exit(NULL);
}

void *f2(void *arg){
  BBuffer *b = (BBuffer *)arg;
  for(int i=0; i<100; i++){
    bb_put(*b, 2);
    printf("[t2] put 2\n");
  }
  pthread_exit(NULL);
}

int main() {
  pthread_t thread1, thread2;
  BBuffer b = bb_create();

  if (pthread_create(&thread1, NULL, f1, &b) != 0){
    return 1;
  }
  if (pthread_create(&thread2, NULL, f2, &b) != 0){
    return 1;
  }

  for(int i=0; i<200; i++){
    printf("[m] get %d\n", bb_get(b));
  }

  if (pthread_join(thread1, NULL) != 0) {
    return 1;
  }
  if (pthread_join(thread2, NULL) != 0) {
    return 1;
  }
  
  return 0;
}
