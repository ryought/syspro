#include "btree.h"
#include <stdio.h>


int main(){
  BTree t;
  t = btree_create();
  t = btree_insert(3, t);
  t = btree_insert(1, t);
  t = btree_insert(2, t);
  t = btree_insert(11, t);
  t = btree_insert(6, t);
  t = btree_insert(11, t);
  btree_dump(t);
  return 0;
}
