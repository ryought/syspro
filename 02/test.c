#include <stdio.h>


int hoge(){
  int i;
  return (i = 2); 
}

int main(){
  printf("%d\n", hoge());
  return 0;
}

