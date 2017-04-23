# 課題A

コンパイラの警告を消すには、a.cを以下のように変更する

```
#include <stdio.h>
#include "c.h"

const char *a = "Hello, C++ world!\n";  // 追加1: プロトタイプ宣言

int mystrcmp(const char *, const char *);

int main(void)
{
	const char *s = hello_c();
	if (mystrcmp(s, a) < 0)
		printf("%s", s);  // 追加2: フォーマット文字列
	else
		printf("%s", a);  // 追加3: フォーマット文字列
}
```

としたらよい。
