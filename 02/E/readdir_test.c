#include <stdio.h>
#include <dirent.h>


int main() {
  DIR *dirp = opendir("~/");
  struct dirent *d = readdir(dirp);
  if (d == 0) {
    printf("error\n");
  } else {
    printf("%s\n", d->d_name);
  }
  return 0;
}
