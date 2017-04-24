#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef char * String;
typedef struct tnode * BTree;
struct tnode {
    int value;
    BTree left;
    BTree right;
};


/* 空木を返す。 */
BTree btree_create() {
  BTree t = malloc(sizeof(struct tnode));
  if (t != NULL) {
    t->value = 0;
    t->left = NULL;
    t->right = NULL;
  } else {
    printf("error\n");
  }
  return t;
}


/* 空木かどうかの判定  */
/* 空だったら1、そうでなければ0を返す*/
int btree_isempty(BTree t)
{
  if (t == NULL) {
    return 1;
  } else {
    if (t->value == 0 && t->left == NULL && t->right == NULL) {
      return 1;
    } else {
      return 0;
    }
  } 
}
 
BTree btree_insert(int x, BTree t)
{
    /*
     * 文字列`key'をキーとして、文字列`val'を二分探索木`t'に挿入し、その木を返す。
     * `key'と`val'の内容はデータベースにコピーされる。
     * 同じキーを持つ項目が既に存在する場合は、その項目を上書きする。
     */
  BTree t2; // returnする木
  t2 = btree_create();

  if (btree_isempty(t)) {
    t2->value = x;
    t2->left = NULL;
    t2->right = NULL;
  } else {
    if (t->value < x) {
      t2->value = t->value;
      t2->left = t->left;
      t2->right = btree_insert(x, t->right);
    } else if (t->value >= x) {
      t2->value = t->value;
      t2->left = btree_insert(x, t->left);
      t2->right = t->right;
    }
  }
  return t2;
}


BTree btree_min(BTree t) {
  while (! btree_isempty(t)) {
    if (! btree_isempty(t->left)) {
      t = t->left;
    } else {
      return t;
    }
  }
}

BTree btree_joint(int value, BTree left, BTree right) {
  BTree t = btree_create();
  t->value = value;
  t->left = left;
  t->right = right;
  return t;
}

BTree btree_elim_min(BTree t) {
  if (btree_isempty(t->left)) {
    if (btree_isempty(t->right)) {
      return btree_create();
    } else {
      return t->right;
    }
  } else {
    return btree_joint(t->value, btree_elim_min(t->left), t->right);
  }
}

// 
BTree btree_delete(int value, BTree t)
{
    /*
     * `value`を、二分探索木`t'から削除し、その木を返す。
     */
  if (! btree_isempty(t)) {
    if (t->value == value) {
      if (btree_isempty(t->left) && btree_isempty(t->right)) {
        return btree_create();
      } else if (! btree_isempty(t->left) && ! btree_isempty(t->right)) {
        BTree m = btree_min(t->right);
        return btree_joint(m->value, t->left, btree_elim_min(t->right)); 
      } else if (btree_isempty(t->left)) {
        return t->right;
      } else {
        return t->left;
      }
    } else if (t->value < value) {
      return btree_joint(t->value, t->left, btree_delete(value, t->right));
    } else if (t->value > value) {
      return btree_joint(t->value, btree_delete(value, t->left), t->right);
    }
  }
}
 

 
void btree_destroy(BTree t)
{
    /*
     * 二分探索木`t'の内容を全て消去し、メモリを解放する。
     */
  if (! btree_isempty(t)) {
    btree_destroy(t->left);
    btree_destroy(t->right);
    free(t);
  }
}


void btree_dump(BTree t) {
  if (btree_isempty(t)) {
    return;
  }
  btree_dump(t->left);
  printf("%d\n", t->value);
  btree_dump(t->right);
}
