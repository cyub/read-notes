#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void cleanup_function(void* arg) {
  printf("Cleanup function called with argument: %s\n", (char*)arg);
}

void* thread_function(void* arg) {
  // 注册清理处理函数
  pthread_cleanup_push(cleanup_function, "Hello from cleanup");

  printf("Thread is running...\n");
  sleep(2);  // 模拟线程工作

  // 模拟线程被取消
  pthread_cancel(pthread_self());

  // 注册的清理函数会在这里被调用
  pthread_cleanup_pop(1);  // 执行清理函数

  return NULL;
}

int main() {
  pthread_t thread;

  // 创建线程
  if (pthread_create(&thread, NULL, thread_function, NULL) != 0) {
    perror("pthread_create");
    return 1;
  }

  // 等待线程结束
  if (pthread_join(thread, NULL) != 0) {
    perror("pthread_join");
    return 1;
  }

  printf("Thread has been terminated.\n");
  return 0;
}