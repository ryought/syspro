#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>

int main() {
  char a[80] = "Hello, world";
  ret = syscall(300 , a, strlen(a));
  printf("%d:%s\n", ret, a);
  return 0;
}
