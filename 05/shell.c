// 課題B, C シェル
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include "parser/parse.h" // パーサ

// 適当な入力元を返してくれる
int fd0(process *process, int fd[2], int pfd[2]) {
  int x;
  // fd[0] 入力元を変える
  if(process->input_redirection) // インプットファイルがあった時
    x = open(process->input_redirection, O_RDONLY);
  else if (fd[1] == pfd[1]) // 一つ前でパイプ使ってる時
    x = pfd[0];  // 読み出し側のパイプを使う
  else
    x = 0; // stdin
  return x;
}

// 適当な出力先を返してくれる
int fd1(process *process, int fd[2], int pfd[2]) {
  int x;
  // fd[1] 出力先を変える設定
  if(process->output_redirection){
    if(process->output_option == TRUNC){
      // 作るモード
      x = open(process->output_redirection, O_WRONLY|O_CREAT, 0666);
    }else if(process->output_option == APPEND){
      // 追記モード
      x = open(process->output_redirection, O_RDWR|O_CREAT|O_APPEND, 0666);
    }
  } else if (process->next != NULL){
    // 次がいる時はパイプに接続
    x = pfd[1];
  } else {
    x = 1;  // stdout
  }
  return x;
}

// signalハンドラ
void handler(int signal) {
  pid_t pid = 0;
  int status;
  do{
    pid = waitpid(-1, &status, WNOHANG);
  } while (pid>0);
  //printf("%d, %d, %d, %d\n", pid, WIFEXITED(status), WIFSIGNALED(status), WIFSTOPPED(status));
}

// signalハンドラ
void stop(int signal) {
  kill(0, SIGSTOP);
}

int run_process(process *process, int fd[2]) {
  int pgid;
  
  return pgid;
}

void pid_log(char *message) {
  printf("(%s) pid:%d pgid:%d tcpgid:%d\n", message, getpid(), getpgid(0), tcgetpgrp(0));
}
 
int main(int argc, char* argv[], char *envp[]) {
  pid_t pid, watcher;
  int pgid, sh_pgid;
  char s[LINELEN];
  job *curr_job;
  int pfd[2] = {3, 4};  // パイプ
  int fd[2] = {0, 1};  // fd[0]が入力、fd[1]が出力 デフォルトはstdin, stdoutに設定
  int gfd[2];
  char program_name[256];
  
  // PATHの取得処理
  // TODO ここを環境変数envpから取り出す処理
  char* path[] = {"/usr/bin/", "/bin/"};


  // shell側のシグナル設定
  signal (SIGINT, SIG_IGN);   // C-c
  //signal (SIGQUIT, SIG_IGN);  
  signal (SIGTSTP, SIG_IGN);  // C-z
  signal (SIGTTIN, SIG_IGN); // 子供プロセスとの関係
  signal (SIGTTOU, SIG_IGN);
  signal (SIGCHLD, SIG_IGN);

  

  setpgid(0, 0);
  tcsetpgrp(0, getpgid(0));

  
  while(get_line(s, LINELEN)){
    curr_job = parse_line(s);

    if(curr_job == NULL)
      continue;

    // 実行

    process *process = curr_job->process_list;

    // builtin 判定
    if(!strcmp(process->program_name, "exit"))
      return 0;

    if(!strcmp(process->program_name, "bg")){
      if(watcher!=0) {
        printf("background process exist %d\n", watcher);
        tcsetpgrp(0, getpgid(watcher));
        kill(-1*watcher, SIGCONT);
      }else{
        printf("not exist\n");
      }
    }
    
    if(!strcmp(process->program_name, "fg"))
      printf("hi");

    
    if((watcher = fork()) == 0) {
      // 子供
      pid_log("job watcher");
      // 各プロセスにforkして、返りを待つ。全部帰ってきたら死ぬ

      // シグナルの設定を戻す
      //ctrl-c 
      signal (SIGINT, SIG_DFL);
      //signal (SIGQUIT, SIG_DFL);
      //signal (SIGTTIN, SIG_DFL);
      //signal (SIGTTOU, SIG_DFL);
      signal (SIGCHLD, SIG_DFL);
      
      // 一時停止・再開シグナル
      signal (SIGTSTP, SIG_DFL);
      signal (SIGCONT, SIG_DFL);

      

      // job単位のpgidを作る。リーダーはこのwatcherプロセス
      setpgid(watcher, watcher);
      // jobの実行モードによって切り分ける
      // もしforeground実行だったら、こっち側を表にする
      // tcsetpgrpを行わなければ、shell側が表のままになる
      if(curr_job->mode == FOREGROUND)
        tcsetpgrp(0, getpgid(0));

      int fd[2] = {0, 1};
      int pfd[2] = {3, 4};

      while(process != NULL){
        fd[0] = fd0(process, fd, pfd); // processの入力
        pipe(pfd); // pipeで繋がれた前後process間通信用
        fd[1] = fd1(process, fd, pfd); // processの出力先
        // 各プロセスごとのfork
        if((pid = fork()) == 0){
          // 子供 それぞれのプロセスに変わる
          pid_log("process");
          dup2(fd[0], 0);
          dup2(fd[1], 1);
          execve(process->program_name, process->argument_list, envp);
        } else {
          // 親 プロセスの終了をちゃんと待つ(同期動作)
          pid_log("job watcher with process run");
          // fdのclose処理
          if(fd[0] != 0)  close(fd[0]); // 読み込みはしないので閉じる
          if(fd[1] != 1)  close(fd[1]);  // 書き込み先にEOF送る

          // 子供の終了を待つ(同期)。一つのprocessが終了したら、次のプロセスを起動
          int status;
          waitpid(pid, &status, WUNTRACED);
        }
        // 更新処理
        process = process->next;
      }
      // パイプ 使わないのがあるかもしれないので消しておく
      close(pfd[0]);
      close(pfd[1]);
      pid_log("job watcher will exit");
      return 0;
    } else {
      // 本体
      pid_log("shell");
      int watcher_status;
      if(curr_job->mode == FOREGROUND) {
        // foreground実行の時
        signal (SIGCHLD, SIG_DFL);
        // watcherの終了(=jobの終了)を完全に待機
        waitpid(watcher, &watcher_status, WUNTRACED);
        // 戻ってきたらbackgroundになってるshellをforegroundにもどす。
        tcsetpgrp(0, getpgid(0));
      } else {
        // background実行の時
        struct sigaction ai;
        ai.sa_handler = handler;
        ai.sa_flags = 0;
        sigaction(SIGCHLD, &ai, NULL);
        // すぐに戻る。 ハンドラを設定して、watcherのpidに反応するようにする。
      }
      
    }
   
  }
  return 0;
}
