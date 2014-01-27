#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

// 쓰레드 종료시 호출될 함수
void clean_up(void *);

// 쓰레드 함수
void *thread_func(void *);

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lmu = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{

   pthread_t pt;
   pthread_create(&pt, NULL, thread_func, NULL);

   // 생성된 쓰레드 pt에 취소 요청을 보낸다.
   pthread_cancel(pt);

   // 5초를 쉰 후에 시그널을 보낸다.
   sleep(5);
   pthread_cancel(pt);
   int state= pthread_cond_signal(&cond);
	printf("%d\n",state);
   fflush(stdout);
   // join후 종료한다.
   pthread_join(pt, NULL);
   printf("exit\n");
   fflush(stdout);
   exit(1);
}

// 쓰레드 종료시 효출될 함수
// 여기에 자원해제루틴을 입력할 수 있을 것이다.
void clean_up(void *arg)
{
   printf("Thread cancel Clean_up function\n");
   fflush(stdout);
}

void *thread_func(void *arg)
{
   // DISABLE 상태다.
   // 이경우 쓰레드에 대해서 취소 요청이 들어오면
   // 바로 취소된다.
   pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

   // 쓰레드 종료시 호출될 함수 등록
   pthread_cleanup_push(clean_up, (void *)NULL);

   while(1)
   {
       pthread_mutex_lock(&lmu);
int *t=(int*)10;
printf("%d",*t);
       printf("THREAD cond wait\n");
       fflush(stdout);
       pthread_cond_wait(&cond, &lmu);
       printf("NO WAIT COND\n");
       sleep(1);
       fflush(stdout);
       pthread_mutex_unlock(&lmu);
   }

   printf("EXIT\n");
   fflush(stdout);
   pthread_cleanup_pop(0);
}
