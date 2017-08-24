#include <stdio.h>

int main(void) {
  int in1=10, in2=5, out1, out2;
  printf("%#x, %d\n", out1, out2);
  __asm__("movl %2, %%eax;"
      "cpuid;"
      "addl %3, %%eax;"
      "movl %%eax, %0;"
      "movl %%eax, %1;"
      :"=r"(out1), "=r"(out2)
      :"r"(in1), "r"(in2)
      :"%eax"
      );
  printf("%#x, %d\n", out1, out2);
  return 0;
}
