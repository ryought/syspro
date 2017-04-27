#include "btree.h"
#include <stdio.h>
#include <pthread.h>

#define BLOCK 20

void *appender(void *arg){
  BTree *t = (BTree *)arg;
  int i;
  for (i=1; i<=BLOCK; i++) {
    printf("[thread] adding %d\n", i);
    *t = btree_insert(i, *t);
  }
  pthread_exit(NULL);
}



int main() {
  pthread_t thread;
  int i;
  BTree t = btree_create();
  //t = btree_insert(100, t);

  if (pthread_create(&thread, NULL, appender, &t) != 0){
    printf("error: ptheread_create\n");
    return 1;
  }

  for (i=BLOCK*2+1; i<=BLOCK*3; i++) {
    printf("[main] adding %d\n", i);
    t = btree_insert(i, t);
  }

  if (pthread_join(thread, NULL) != 0) {
    printf("error: pthread_join\n");
    return 1;
  }
  
  printf("[main] finished\n");
  btree_dump(t);
  return 0; 
}
