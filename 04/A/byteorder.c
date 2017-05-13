#include <stdio.h>
//#include "unp.h"
 
int main() {
  union {
    short s;
    char c[sizeof(short)];
  } un; // 共用体 for all members, memory offsetが0

  un.s = 0x0102;
  //printf("%s: ", CPU_VENDOR_OS);

  if(sizeof(short) == 2){
    if(un.c[0] == 1 && un.c[1] == 2)
      printf("big\n");
    else if(un.c[0] == 2 && un.c[1] == 1)
      printf("little\n");
    else
      printf("unknown\n");
  } else {
    printf("sizeof(short) = %lu\n", sizeof(short));
  }
  return 0;
}
