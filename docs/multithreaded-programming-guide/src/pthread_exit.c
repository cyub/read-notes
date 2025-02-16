#include <pthread.h>
#include <stdio.h>

void* thread_function(void* arg) {
  printf("Thread is running\n");
  // 模拟线程工作
  sleep(2);
  printf("Thread is exiting\n");
  // 使用 pthread_exit 终止线程并返回状态值
  pthread_exit((void*)42);  // 返回值为 42
}

int main() {
  pthread_t thread;
  void* status;

  // 创建线程
  if (pthread_create(&thread, NULL, thread_function, NULL) != 0) {
    perror("pthread_create");
    return 1;
  }

  // 等待线程结束并获取返回值
  if (pthread_join(thread, &status) != 0) {
    perror("pthread_join");
    return 1;
  }

  printf("Thread returned status: %ld\n", (long)status);

  return 0;
}