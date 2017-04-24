#include <stdio.h>
#define SIZE 10

typedef struct bb {
  int buf[SIZE];
  int head;
  int tail;
} BBuffer;

BBuffer bb_create() {
  BBuffer b;
  b.head = 0;
  b.tail = 0;
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
int bb_get(BBuffer *b) {
  // 読み出すものがない時は、入ってくるのを待つ
  /* if (EMPTY) { */

  /* } */
  /* return  */
}  


/* bb_put : バッファの末尾に要素を追加する */
void bb_put(BBuffer *b, int i) {
  // いっぱいの時は先頭のが読み出されるのを待つ
  /* if (FULL) { */
    
  /* } */
  (*b).tail = bb_incr((*b).tail);
  (*b).buf[(*b).tail] = i;
}


int main() {
  int x = 0;

  int i;
  for (i=0; i<20; i++) {
    x = bb_incr(x);
    printf("%d\n", x);  
  }

  for (i=0; i<20; i++) {
    x = bb_decr(x);
    printf("%d\n", x);
  }
  
  return 0;
}
