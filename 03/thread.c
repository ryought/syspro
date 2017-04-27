#include <stdio.h>
#include <pthread.h>

void *thread1(void){
  foo = 9;
}

int main() {
  int foo;
  pthread_create(,thread1,);
  foo = 2;

  printf("foo=%d\n", foo);
}
