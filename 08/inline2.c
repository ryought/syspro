#include <stdio.h>

int main(void) {
  int in1=10, in2=5, out1, out2;
  printf("%#x\n", out1);
  __asm__ ("pushfw;"
         "movl %%eax, %0;"
        :"=r"(out1),"=r"(out2)       /* 出力 変数out1が%0 変数out1が%1  */
        :           /* 入力 変数in1が%2 変数in2が%3 */
        :"%eax", "cc"
       );
  printf("%#x\n", out1);
  return 0;
}
