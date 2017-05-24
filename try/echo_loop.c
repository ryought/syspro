#include <stdio.h> 

void echo(FILE *fp){
  char buf[512];
  while(fgets(buf, 512, fp) != NULL) {
    printf("[*] got message\n");
    fputs(buf, stdout);
  }
}


int main() {
  while(1){
    echo(stdin);
  }
  return 0;
}
