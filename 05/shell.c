// 課題B, C シェル
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include "parser/parse.h" // パーサ
 
int main(int argc, char* argv[], char *envp[]) {
  pid_t pid;
  char s[LINELEN];
  job *curr_job;
  int pfd[2] = {3, 4};  // パイプ
  int fd[2] = {0, 1};  // fd[0]が入力、fd[1]が出力 デフォルトはstdin, stdoutに設定
  char program_name[256];

  // PATHの取得処理
  // TODO ここを環境変数envpから取り出す処理
  char* path[] = {"/usr/bin/", "/bin/"};
  
  while(get_line(s, LINELEN)){
    curr_job = parse_line(s);
    process *process = curr_job->process_list;
    
    
    while(process != NULL){
      // builtin 判定
      if(strcmp(process->program_name, "exit") == 0){
        return 0;
      }
      
      // fd[0] 入力元を変える
      if(process->input_redirection) {
        // インプットファイルがあった時
        fd[0] = open(process->input_redirection, O_RDONLY);
      } else if (fd[1] == pfd[1]) {
        // 一つ前でパイプ使ってる時
        fd[0] = pfd[0];  // 読み出し側のパイプを使う
      } else {
        fd[0] = 0; // stdin
      }

      // 新しいパイプ作る      
      pipe(pfd);

      // fd[1] 出力先を変える設定
      if(process->output_redirection){
        if(process->output_option == TRUNC){
          // 作るモード
          fd[1] = open(process->output_redirection, O_WRONLY|O_CREAT, 0666);
        }else if(process->output_option == APPEND){
          // 追記モード
          fd[1] = open(process->output_redirection, O_RDWR|O_CREAT|O_APPEND, 0666);
        }
      } else if (process->next != NULL){
        // 次がいる時はパイプに接続
        fd[1] = pfd[1];
      } else {
        fd[1] = 1;  // stdout
      }

      // process作成
      if((pid=fork()) == 0){
        // child側
        // DEBUG
        //printf("%d %d %d %d\n", fd[0], fd[1], pfd[0], pfd[1]);
        
        dup2(fd[0], 0);
        dup2(fd[1], 1);

        execve(process->program_name, process->argument_list, envp);
      } else {
        // parent
        // fdのclose処理
        if(fd[0] != 0)  close(fd[0]); // 読み込みはしないので閉じる
        if(fd[1] != 1)  close(fd[1]);  // 書き込み先にEOF送る
        int status;
        waitpid(pid, &status, WUNTRACED); // 子供は終了時、親にstatusを返さないといけない。それを待ち受ける。
      }
      process = process->next; // 更新処理
    }
    close(pfd[0]);
    close(pfd[1]);
  }
  return 0;
}
