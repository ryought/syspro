# 課題A
ファイル
- btree.h
- A1.c
- A2.c
- A3.c

## イ ロックなし
まず二分木ライブラリ`btree.h`を作った。
次にそれを使って、mainとthreadの2つから同じBTreeに値を順番に同時に追加していくプログラム`A1.c`を作った。`appender()`はthreadとなって1~20までの値を、`main()`の中では同じBTreeに21~40までの値を同時に（並列に）追加していく。このとき、特に保護や排他処理は考えていない。

その実行結果は次の通り。

``` 
[main] adding 21
[thread] adding 1
[main] adding 22
[main] adding 23
[main] adding 24
[main] adding 25
[main] adding 26
[main] adding 27
[thread] adding 2
[main] adding 28
[thread] adding 3
[main] adding 29
[thread] adding 4
[main] adding 30
[thread] adding 5
[main] adding 31
[thread] adding 6
[main] adding 32
[thread] adding 7
[main] adding 33
[thread] adding 8
[main] adding 34
[thread] adding 9
[main] adding 35
[main] adding 36
[main] adding 37
[main] adding 38
[thread] adding 10
[main] adding 39
[thread] adding 11
[main] adding 40
[thread] adding 12
[thread] adding 13
[thread] adding 14
[thread] adding 15
[thread] adding 16
[thread] adding 17
[thread] adding 18
[thread] adding 19
[thread] adding 20
[main] finished
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
38
39
40
```


ここからわかるように、
- 値の追加処理そのものは、ログを見ればわかるように両thread共に全ての値について処理をおこなえている。
- しかし追加された値には抜けがある。
  - たとえば1,35,36など
  - 保護を入れていないことが原因。


スレッドを2つ別に立てる(main, thread1, thread2のように)プログラム`A2.c`も作って試したが、同様の結果であった。


## ロ
イのプログラム`A1.c`にmutexを使った同期を入れたプログラム`A3.c`を作った。
具体的には、変数に影響が及ぶ`btree_insert`の呼び出し前にlock、呼び出しが終わったらunlockをするようにした。

結果は、

``` text
[main] adding 21
[thread] adding 1
[main] adding 22
[thread] adding 2
[main] adding 23
[thread] adding 3
[main] adding 24
[thread] adding 4
[main] adding 25
[thread] adding 5
[main] adding 26
[thread] adding 6
[main] adding 27
[thread] adding 7
[main] adding 28
[thread] adding 8
[main] adding 29
[thread] adding 9
[main] adding 30
[thread] adding 10
[main] adding 31
[thread] adding 11
[main] adding 32
[thread] adding 12
[main] adding 33
[thread] adding 13
[main] adding 34
[thread] adding 14
[main] adding 35
[thread] adding 15
[main] adding 36
[thread] adding 16
[main] adding 37
[thread] adding 17
[main] adding 38
[thread] adding 18
[main] adding 39
[thread] adding 19
[main] adding 40
[thread] adding 20
[main] finished
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
```

のようになって、数字は全て正しく追加されていることがわかる。
