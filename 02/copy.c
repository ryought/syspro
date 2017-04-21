#include <stdio.h>


int main(int argc, char *argv[]){
  char *p;
  int i;
  for(i=0; i<argc; i++){
    p = argv[i];
    printf("argv[%d] = %s\n", i, p);
  }
  return 0;
}
