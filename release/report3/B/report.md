# B
thread1,2がそれぞれ独立に100個の数字をBBufferにputする一方で、main側が200回getするコードを書いて実験した。Bufferのサイズは10にしているから、fullになる状況があっておかしくない。
実行したところ、emptyもfullも生じている条件下で、putの回数とgetの回数が一致した。ここからputの漏れや重複は起きていないことがわかる。またbuffer外の領域にアクセスすることによるエラーも生じていないから、boundも機能していることがわかる。
