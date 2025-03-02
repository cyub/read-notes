#include <errno.h>
#include <pthread.h>
#include <stdio.h>

void* thread_func(void* arg) {
  pthread_mutex_t* mutex = (pthread_mutex_t*)arg;

  // 尝试获取锁
  int ret = pthread_mutex_lock(mutex);
  if (ret == EOWNERDEAD) {
    printf("Previous owner died. Recovering...\n");
    pthread_mutex_consistent(mutex);  // 标记状态一致
  } else if (ret != 0) {
    perror("pthread_mutex_lock");
    return NULL;
  }

  // 操作共享资源
  printf("Lock acquired. Working...\n");
  sleep(2);  // 模拟耗时操作

  // 释放锁
  pthread_mutex_unlock(mutex);
  return NULL;
}

int main() {
  pthread_mutexattr_t attr;
  pthread_mutex_t mutex;

  // 初始化健壮互斥锁
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);
  pthread_mutex_init(&mutex, &attr);

  // 创建线程
  pthread_t thread;
  pthread_create(&thread, NULL, thread_func, &mutex);

  // 主线程等待子线程结束
  pthread_join(thread, NULL);

  // 清理资源
  pthread_mutex_destroy(&mutex);
  pthread_mutexattr_destroy(&attr);
  return 0;
}