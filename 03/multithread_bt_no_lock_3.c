#include "btree.h"
#include <stdio.h>
#include <pthread.h>

#define BLOCK 20

void *appender1(void *arg){
  BTree *t = (BTree *)arg;
  int i;
  for (i=1; i<=BLOCK; i++) {
    printf("[thread1] adding %d\n", i);
    *t = btree_insert(i, *t);
  }
  pthread_exit(NULL);
}

void *appender2(void *arg){
  BTree *t = (BTree *)arg;
  int i;
  for (i=BLOCK+1; i<=BLOCK*2; i++) {
    printf("[thread2] adding %d\n", i);
    *t = btree_insert(i, *t);
  }
  pthread_exit(NULL);
}



int main() {
  pthread_t thread1, thread2;
  int i;
  BTree t = btree_create();
  //t = btree_insert(100, t);

  if (pthread_create(&thread1, NULL, appender1, &t) != 0){
    printf("error: ptheread_create\n");
    return 1;
  }
  if (pthread_create(&thread2, NULL, appender2, &t) != 0){
    printf("error: ptheread_create\n");
    return 1;
  }

  for (i=BLOCK*2+1; i<=BLOCK*3; i++) {
    printf("[main] adding %d\n", i);
    t = btree_insert(i, t);
  }

  if (pthread_join(thread1, NULL) != 0) {
    printf("error: pthread_join\n");
    return 1;
  }
  if (pthread_join(thread2, NULL) != 0) {
    printf("error: pthread_join\n");
    return 1;
  }
  printf("[main] finished\n");
  btree_dump(t);
  return 0; 
}
