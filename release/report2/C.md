# 課題C

## 1 ファイルをコピーするのにかかる時間
B.cのコピーにかかる時間をgettimeofday()によって測定できるよう変更して、ファイルをコピーした。

ダミーデータとして、1MBのデータを用意した。
`dd if=/dev/zero of=file bs=1024 count=1024`

### 結果
バッファーサイズ 1byte    2636915
バッファーサイズ 512byte  5797
バッファーサイズ 8192byte 1085

### 結論
バッファーサイズが大きいほうが、大きいデータのコピーには有利
（ただし、細かいデータを送るときは、バッファーサイズが大きいと無駄が大きいと推測される）

## 2 fread/fwriteを使ったコピーにかかる時間の測定
`stdio.h`の`fread`,`fwrite`を使ってコピーするプログラム`C.c`を用いて実験する。1と同じファイルをコピーした結果は、2080usとなった。

straceを用いてシステムコールの呼び出され方を見てみると、
```
read(3, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"..., 4096) = 4096
write(4, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"..., 4096) = 4096
```
となっていることから、バッファーサイズは4096byteで実行されている。この結果はB.cを用いたコピー時間のバッファーサイズ依存性にも矛盾しない。

