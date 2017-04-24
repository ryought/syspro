#include "btree.h"
#include <stdio.h>
#include <pthread.h>

void *appender(void *arg){
  BTree *t = (BTree *)arg;
  int i;
  for (i=1; i<=10; i++) {
    printf("[thread] adding %d\n", i);
    *t = btree_insert(i, *t);
  }
  pthread_exit(NULL);
}




int main() {
  pthread_t thread;
  BTree t = btree_create();
  t = btree_insert(100, t);

  if (pthread_create(&thread, NULL, appender, &t) != 0){
    printf("error: ptheread_create\n");
    return 1;
  }


  if (pthread_join(thread, NULL) != 0) {
    printf("error: pthread_join\n");
    return 1;
  }
  printf("[main] finished\n");
  btree_dump(t);
  return 0; 
}
