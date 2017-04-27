/* thread idを取得するサンプル
   http://d.hatena.ne.jp/iww/20101217/p1 を参考に*/


#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

pid_t gettid(void)
{
  return syscall(SYS_gettid);
}

void *gettidtest( void *arg )
{
  int number = (int)arg;

  printf("thread %d:tid=%d\n", number, gettid());
  printf("thread %d:pid=%d\n", number, getpid());
  printf("thread %d:ppid=%d\n", number, getppid());

  sleep(100);
  return NULL;
}

int main( int argc, char **argv )
{
  int i;
  pthread_t p;

  for(i=1;i<=3;i++){
    pthread_create( &p, NULL, gettidtest, (void*)i );
  }
  gettidtest( 0 );

  return 0;
}

