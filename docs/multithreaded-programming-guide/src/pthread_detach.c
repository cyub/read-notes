#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *thread_function(void *arg) {
  printf("子线程开始运行\n");
  sleep(2);  // 模拟子线程工作
  printf("子线程结束运行\n");
  return NULL;
}

int main() {
  pthread_t thread;
  if (pthread_create(&thread, NULL, thread_function, NULL) != 0) {
    perror("pthread_create");
    return 1;
  }

  // 将线程设置为分离状态
  if (pthread_detach(thread) != 0) {
    perror("pthread_detach");
    return 1;
  }
  printf("主线程继续运行\n");
  sleep(3);
  printf("主线程结束\n");
  return 0;
}