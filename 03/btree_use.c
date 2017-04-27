#include "btree_key.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  BTree t = btree_empty();
  
  char cmd[10];
  char str1[100];
  char str2[100];
  
  while ((scanf("%10s", cmd) != EOF)) {
    if (strcmp(cmd, "insert") == 0) {
      if (scanf("%100s %100s", str1, str2) != EOF) {
        t = btree_insert(str1, str2, t);
      }
    } else if (strcmp(cmd, "delete") == 0) {
      if (scanf("%100s", str1) != EOF) {
        t = btree_delete(str1, t);
      }
    } else if (strcmp(cmd, "search") == 0) {
      if (scanf("%100s", str1) != EOF) {
        BTree r = btree_search(str1, t);
        if (btree_isempty(r)) {
          printf("(not found)\n");
        } else {
          printf("%s\n", r->value);
        }
      }
    } else if (strcmp(cmd, "quit") == 0) {
      btree_destroy(t);
      break;
    } else if (strcmp(cmd, "show") == 0) {
      btree_dump(t);
    } else {
      printf("(unknown command)\n");
    }
  }
  return 0;
}
