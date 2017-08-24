// システムコールflipを使うユーザープログラム
// int flip(char *p, int len);

#include <stdio.h>
// for syscall(2)
#include <unistd.h>
#include <sys/syscall.h>

// naive implementation
// later rewrite in kernel
int __flip(char *p, int len){
  int i = 0;
  *tmp = (char *)malloc(sizeof(char)*len);
  // copy to tmp
  for(i=0; i<len; i++){
    tmp[i] = p[i];
  }
  // write p in reverse order
  for(i=0; i<len; i++){
    p[i] = tmp[(len-1) - i]
  }
  return len
}

int flip(char *p, int len){
  return syscall(NUM, p, len);
}

int main() {
  char a[80] = "Hello, world";
  ret = flip(a, strlen(a));
  printf("%d: %s\n", ret, a);
  return 0;
}
