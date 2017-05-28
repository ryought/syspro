// 課題B, C シェル
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
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

 
int main(int argc, char* argv[], char *envp[]) {
  pid_t pid;
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

  signal (SIGINT, SIG_IGN);
  signal (SIGQUIT, SIG_IGN);
  signal (SIGTSTP, SIG_IGN);
  signal (SIGTTIN, SIG_IGN);
  signal (SIGTTOU, SIG_IGN);
  signal (SIGCHLD, SIG_IGN);


  setpgid(0, 0);
  tcsetpgrp(0, getpgid(0));

  
  while(get_line(s, LINELEN)){
    curr_job = parse_line(s);

    if(curr_job == NULL)
      continue;
    
    // jobの実行モードによって切り分ける
    if(curr_job->mode == FOREGROUND) {
      // foreground実行
      process *process = curr_job->process_list;
      pgid = 0;  // 初期化

      
      while(process != NULL){
        // builtin 判定
        if(!strcmp(process->program_name, "exit"))
          return 0;

        fd[0] = fd0(process, fd, pfd);
        // 新しいパイプ作る
        pipe(pfd);

        fd[1] = fd1(process, fd, pfd);
        // 親子間の通信用パイプ
        pipe(gfd);

        // process作成
        if((pid=fork()) == 0){
          // child側
          printf("(child) pid:%d pgid:%d tcpgid:%d\n",getpid(), getpgid(0), tcgetpgrp(0));

          // pgidの設定 
          if(pgid == 0){
            //pgid = getpid;
            setpgid(pid, pid); // 最初のプロセス
            pgid = getpgid(pid);
          } else {
            printf("hoge");
            if(setpgid(pid, pgid) != 0)  // 次以降
              perror(0);
            printf("%d\n", getpgid(0));
          }
          
          printf("(child) pgid:%d tcpgid:%d\n", getpgid(0), tcgetpgrp(0));

          tcsetpgrp(0, getpgid(0));

          printf("(child) pgid:%d tcpgid:%d\n", getpgid(0), tcgetpgrp(0));

          signal (SIGINT, SIG_DFL);
          signal (SIGQUIT, SIG_DFL);
          signal (SIGTSTP, SIG_DFL);
          signal (SIGTTIN, SIG_DFL);
          signal (SIGTTOU, SIG_DFL);
          signal (SIGCHLD, SIG_DFL);
          
          

          // 親側にgroupidを通知
          write(gfd[1], &pgid, sizeof(pgid));
          close(gfd[1]);
        
          dup2(fd[0], 0);
          dup2(fd[1], 1);
          execve(process->program_name, process->argument_list, envp);
        
        } else {
          // parent

          printf("(parent) pgid:%d tcpgid:%d\n", getpgid(0), tcgetpgrp(0));
          
          

          
          // fdのclose処理
          if(fd[0] != 0)  close(fd[0]); // 読み込みはしないので閉じる
          if(fd[1] != 1)  close(fd[1]);  // 書き込み先にEOF送る

          int status;
          waitpid(pid, &status, WUNTRACED); // 子供は終了時、親にstatusを返さないといけない。それを待ち受ける。

          
          // child pgidを取得
          read(gfd[0], &pgid, sizeof(pgid));
          close(gfd[0]);
          printf("%d\n", pgid);


          tcsetpgrp(0, getpgid(0));
        }
        process = process->next; // 更新処理
      }

      close(pfd[0]);
      close(pfd[1]); 
      
    } else {
      // background実行  非同期で実行する
      printf("background\n");

      // シグナルハンドラ
    }
  }
  return 0;
}
