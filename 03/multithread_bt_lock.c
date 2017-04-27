#include "btree.h"
#include <stdio.h>
#include <pthread.h>

#define BLOCK 20

// Mutexをいれる
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *appender(void *arg){
  BTree *t = (BTree *)arg;
  int i;
  for (i=1; i<=BLOCK; i++) {
    printf("[thread] adding %d\n", i);
    pthread_mutex_lock(&mutex);
    *t = btree_insert(i, *t);
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}



int main() {
  pthread_t thread;
  
  int i;
  BTree t = btree_create();

  if (pthread_create(&thread, NULL, appender, &t) != 0){
    printf("error: ptheread_create\n");
    return 1;
  }

  for (i=BLOCK+1; i<=BLOCK*2; i++) {
    printf("[main] adding %d\n", i);
    pthread_mutex_lock(&mutex);
    t = btree_insert(i, t);
    pthread_mutex_unlock(&mutex);
  }

  if (pthread_join(thread, NULL) != 0) {
    printf("error: pthread_join\n");
    return 1;
  }
  
  printf("[main] finished\n");
  btree_dump(t);
  return 0; 
}
