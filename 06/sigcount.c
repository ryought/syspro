// 課題A
// SIGINT の呼び出し回数を数える
// countが10以上になったらexitと表示して止まる

#include <stdio.h>
#include <signal.h>

// 呼び出し回数
// volatileオプション付き
//  countはmain内で操作されないので、最適化で条件判定が消されてしまうのを防ぐ
volatile int count;


// signalハンドラ
void handler(int signal) {
  count++;
}


int main() {
  struct sigaction si;
  si.sa_handler = handler; // 自作ハンドラへのポインタ
  // SIG_DFL, SIG_IGN は それぞれデフォルト、無視ハンドラへのポインタ
  si.sa_flags = 0; // フラグはなし

  sigaction(SIGINT, &si, NULL); // old_siは指定なし

  while(1){
    if(count >= 10)
      break;
  }

  printf("exit\n");
  
  return 0;
}
