// 課題B, C シェル
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "parser/parse.h" // パーサ
 
int main(int argc, char* argv[], char *envp[]) {
  pid_t pid1, pid2;
  char* path = argv[1];

  char s[LINELEN];
  job *curr_job;
  int fd[2];
  
  while(get_line(s, LINELEN)){
    curr_job = parse_line(s);
    pipe(fd);

    if((pid1=fork()) == 0){ // child process作成
      // child側処理 0が入る
      if(curr_job->process_list->next != NULL){
        // 次がいるから、出力先をpipe(標準出力1 -> write用fd[1])にする
        dup2(fd[1], 1);
        close(fd[0]);
      }
      int ret1 = execve(curr_job->process_list->program_name,
                        curr_job->process_list->argument_list, envp);
    } else {
      // parent側 pidには子プロセスのがはいってる
      close(fd[1]); // closeしてEOFを送る
      int status1;
      waitpid(pid1, &status1, WUNTRACED); // 子供は終了時、親にstatusを返さないといけない。それを待ち受ける。
    }
    

    if(curr_job->process_list->next != NULL && (pid2=fork()) == 0){
      //printf("child\n");
      // child
      dup2(fd[0], 0);
      close(fd[1]);
      int ret2 = execve(curr_job->process_list->next->program_name,
                        curr_job->process_list->next->argument_list, envp);
    }else{
      // parent側 pidには子プロセスのがはいってる
      close(fd[0]);
      int status2;
      waitpid(pid2, &status2, WUNTRACED); // 子供は終了時、親にstatusを返さないといけない。それを待ち受ける。
    }
  }
  return 0;
}
