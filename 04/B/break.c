#include <stdio.h>

int main() {
  int i = 0;
  while(i < 10){
    printf("hoge\n");
    i++;
    if(i < 5)
      continue;
    printf("yahoo\n");
  } 
  return 0;
}
