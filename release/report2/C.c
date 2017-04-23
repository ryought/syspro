/* 課題C 高級ライブラリを使ったコピー  */
// stdioにあるfread, fwriteを使ってコピーする

#include <stdio.h>
#include <sys/time.h>

int main(int argc, char *argv[]){
  if (argc != 3) {
    // not enough args
    return -1;
  }
  
  struct timeval start, end;
  FILE *fp1, *fp2;
  char buf[512];
  int i;

  gettimeofday(&start, NULL);
  
  fp1 = fopen(argv[1], "rb");
  fp2 = fopen(argv[2], "wb");

  if (fp1 && fp2) {
    while ((i = fread(buf, 1, 512, fp1)) > 0) {
      fwrite(buf, 1, i, fp2);
      //printf("%s, %d\n", buf, i);
    }
    fclose(fp1);
    fclose(fp2);
  }
  
  gettimeofday(&end, NULL);
  printf("%ld\n",
	 (end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
 
  return 0;
}
