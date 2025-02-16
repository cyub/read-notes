#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* thread_function(void* arg) {
  pthread_cleanup_push((void (*)(void*))printf,
                       "Thread resources cleaned up\n");
  while (1) {
    printf("Thread running...\n");
    sleep(1);
    pthread_testcancel();  // 显式设置取消点
  }
  pthread_cleanup_pop(1);  // 执行清理函数
  return NULL;
}

int main() {
  pthread_t thread;
  pthread_create(&thread, NULL, thread_function, NULL);

  sleep(3);                    // 等待几秒钟
  pthread_cancel(thread);      // 发送取消请求
  pthread_join(thread, NULL);  // 等待线程结束
  printf("Thread has been canceled.\n");
  return 0;
}