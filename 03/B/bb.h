#define _REENTRANT

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define SIZE 10

typedef struct bb * BBuffer;
struct bb {
  int buf[SIZE];
  int head;
  int tail;
};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cvEmpty = PTHREAD_COND_INITIALIZER;
pthread_cond_t cvFull = PTHREAD_COND_INITIALIZER;

BBuffer bb_create() {
  BBuffer b = malloc(sizeof(struct bb));
  int i;
  for (i=0; i<SIZE; i++) {
    b->buf[i] = 0;
  }
  b->head = 0;
  b->tail = 0;
  return b;
}


int bb_incr(int x) {
  if (0 <= x && x < SIZE-1) {
    return x + 1;
  } else if (x == SIZE-1) {
    return 0;
  } else {
    return -1;
  }
}
int bb_decr(int x) {
  if (0 < x && x < SIZE) {
    return x - 1;
  } else if (x == 0) {
    return SIZE - 1;
  } else {
    return -1;
  }
}

/* bb_get : バッファの先頭要素を読み出す */
int bb_get(BBuffer b) {
  pthread_mutex_lock(&mutex);
  // 読み出すものがない時は、入ってくるのを待つ
  //printf("%d~%d\n", b->head, b->tail);
  while (b->head == b->tail) {
    // EMPTYのとき、入ってくるのを待つ
    printf("empty");
    pthread_cond_wait(&cvEmpty, &mutex);
  }

  // 読み込み
  int prev_head = b->head;
  b->head = bb_incr(b->head);

  if (bb_incr(b->tail) != b->head) {
    // getした結果FULLから脱したら通知する
    pthread_cond_signal(&cvFull);
  }
  
  pthread_mutex_unlock(&mutex);

  // 結果の返却
  return b->buf[prev_head];
}  


/* bb_put : バッファの末尾に要素を追加する */
void bb_put(BBuffer b, int i) {
  pthread_mutex_lock(&mutex);
  //printf("%d~%d\n", b->head, b->tail);
  // いっぱいの時は先頭のが読み出されるのを待つ
  while (bb_incr(b->tail) == b->head) {
    // FULLのとき、出て行くのを待つ
    printf("full");
    pthread_cond_wait(&cvFull, &mutex);
  }
  
  // 末尾に追加し、バッファサイズを変える
  b->buf[b->tail] = i;
  b->tail = bb_incr(b->tail);
  
  if (b->head != b->tail) {
    // putした結果EMPTYから脱したら
    pthread_cond_signal(&cvEmpty);
  }
  
  pthread_mutex_unlock(&mutex);
}

/**
 bbufferの中身を表示
**/
void bb_dump(BBuffer b) {
  int i;
  for (i=0; i<SIZE; i++) {
    printf("%d ", b->buf[i]);
  }
  printf("\n");
}
