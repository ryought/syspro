/* 課題C 高級ライブラリを使ったコピー  */
// stdioにあるfread, fwriteを使ってコピーするサンプル

#include <stdio.h>

int main(int argc, char *argv[]){
  if (argc != 3) {
    // not enough args
    return -1;
  }

  FILE *fp1, *fp2;
  char buf[512];
  int i;
  
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
 
  return 0;
}
