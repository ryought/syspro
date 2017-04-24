#include <stdio.h>
#include <stdlib.h>
#define SIZE 10

typedef struct bb * BBuffer;
struct bb {
  int buf[SIZE];
  int head;
  int tail;
};

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
  // 読み出すものがない時は、入ってくるのを待つ
  //printf("%d~%d\n", b->head, b->tail);
  if (b->head == b->tail) {
    //printf("empty\n");
    return -1;
  }
  int prev_head = b->head;
  b->head = bb_incr(b->head);
  return b->buf[prev_head];
}  


/* bb_put : バッファの末尾に要素を追加する */
void bb_put(BBuffer b, int i) {
  //printf("%d~%d\n", b->head, b->tail);
  // いっぱいの時は先頭のが読み出されるのを待つ
  if (bb_incr(b->tail) == b->head) {
    printf("full\n");
    return;
  }
  b->buf[b->tail] = i;
  //printf("add %d at %d\n", i, b->tail);
  b->tail = bb_incr(b->tail);
}

void bb_dump(BBuffer b) {
  int i;
  for (i=0; i<SIZE; i++) {
    printf("%d ", b->buf[i]);
  }
  printf("\n");
}


int main() {
  BBuffer b = bb_create();
  bb_put(b, 1);
  bb_put(b, 50);
  bb_put(b, 200);
  bb_put(b, 387);
  bb_put(b, 3);
  bb_put(b, 6);
  bb_put(b, 3);
  bb_put(b, 8);
  bb_put(b, 9);
  bb_put(b, 10);
  bb_dump(b);

  printf("%d\n", bb_get(b));
  printf("%d\n", bb_get(b));
  printf("%d\n", bb_get(b));
  printf("%d\n", bb_get(b));

  bb_dump(b);

  bb_put(b, 45);
  bb_put(b, 46);
  bb_put(b, 47);

  bb_dump(b);
  
  printf("%d\n", bb_get(b));
  printf("%d\n", bb_get(b));
  printf("%d\n", bb_get(b));
  printf("%d\n", bb_get(b));
  printf("%d\n", bb_get(b));
  printf("%d\n", bb_get(b));
  printf("%d\n", bb_get(b));
  
  
  return 0;
}
