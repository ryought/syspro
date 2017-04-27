#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef char * String;
typedef struct tnode * BTree;
struct tnode {
    String key;
    String value;
    BTree left;
    BTree right;
};


/* 空木を返す。 */
BTree btree_empty() {
  BTree t = malloc(sizeof(struct tnode));
  if (t != NULL) {
    t->key = NULL;
    t->value = NULL;
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
    if (t->key == NULL && t->value == NULL
        && t->left == NULL && t->right == NULL) {
      return 1;
    } else {
      return 0;
    }
  } 
}
 
BTree btree_insert(String key_o, String val_o, BTree t)
{
    /*
     * 文字列`key'をキーとして、文字列`val'を二分探索木`t'に挿入し、その木を返す。
     * `key'と`val'の内容はデータベースにコピーされる。
     * 同じキーを持つ項目が既に存在する場合は、その項目を上書きする。
     */
  BTree t2; // returnする木
  t2 = btree_empty();
  String key, val;
  key = malloc(sizeof(char)*100);
  val = malloc(sizeof(char)*100);
  strcpy(key, key_o);
  strcpy(val, val_o);

  if (btree_isempty(t)) {
    t2->key = key;
    t2->value = val;
    t2->left = NULL;
    t2->right = NULL;
  } else {
    if (strcmp(t->key, key) < 0) {
      t2->key = t->key;
      t2->value = t->value;
      t2->left = t->left;
      t2->right = btree_insert(key, val, t->right);
    } else if (strcmp(t->key, key) > 0) {
      t2->key = t->key;
      t2->value = t->value;
      t2->left = btree_insert(key, val, t->left);
      t2->right = t->right;
    } else if (strcmp(t->key, key) == 0) {
      t2->key = key;
      t2->value = val;
      t2->left = t->left;
      t2->right = t->right;
    }
  }
  return t2;
}

struct tnode *btree_search(String key, BTree t)
{
    /*
     * 二分探索木`t'を検索し、文字列`key'をキーとするノードへのポインタを返す。
     * 見付からない場合は、NULLを返す。
     */
  while (! btree_isempty(t)) {
    if (strcmp(t->key, key) == 0) {
      return t;
    } else if (strcmp(t->key, key) < 0) {
      t = t->right;
    } else if (strcmp(t->key, key) > 0) {
      t = t->left;
    }
  }
  return NULL;
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

BTree btree_joint(String key, String value, BTree left, BTree right) {
  BTree t = btree_empty();
  t->key = key;
  t->value = value;
  t->left = left;
  t->right = right;
  return t;
}

BTree btree_elim_min(BTree t) {
  if (btree_isempty(t->left)) {
    if (btree_isempty(t->right)) {
      return btree_empty();
    } else {
      return t->right;
    }
  } else {
    return btree_joint(t->key, t->value, btree_elim_min(t->left), t->right);
  }
}

// 
BTree btree_delete(String key, BTree t)
{
    /*
     * 文字列`key'をキーとする項目を、二分探索木`t'から削除し、その木を返す。
     */
  if (! btree_isempty(t)) {
    if (strcmp(t->key, key) == 0) {
      if (btree_isempty(t->left) && btree_isempty(t->right)) {
        return btree_empty();
      } else if (! btree_isempty(t->left) && ! btree_isempty(t->right)) {
        BTree m = btree_min(t->right);
        return btree_joint(m->key, m->value, t->left, btree_elim_min(t->right)); 
      } else if (btree_isempty(t->left)) {
        return t->right;
      } else {
        return t->left;
      }
    } else if (strcmp(t->key, key) < 0) {
      return btree_joint(t->key, t->value, t->left, btree_delete(key, t->right));
    } else if (strcmp(t->key, key) > 0) {
      return btree_joint(t->key, t->value, btree_delete(key, t->left), t->right);
    }
  }
}
 

 
void btree_destroy(BTree t)
{
    /*
     * 二分探索木`t'の内容を全て消去し、メモリを解放する。
     */
  if (! btree_isempty(t)) {
    free(t->key);
    free(t->value);
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
  printf("%s: %s\n", t->key, t->value);
  btree_dump(t->right);
}



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
