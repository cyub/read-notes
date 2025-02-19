#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  pthread_mutexattr_t attr;
  pthread_mutex_t mutex;

  // 初始化互斥锁属性对象
  if (pthread_mutexattr_init(&attr) != 0) {
    perror("pthread_mutexattr_init failed");
    exit(EXIT_FAILURE);
  }

  // 设置互斥锁类型为递归锁
  if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0) {
    perror("pthread_mutexattr_settype failed");
    exit(EXIT_FAILURE);
  }

  // 使用属性初始化互斥锁
  if (pthread_mutex_init(&mutex, &attr) != 0) {
    perror("pthread_mutex_init failed");
    exit(EXIT_FAILURE);
  }

  // 第一次加锁（成功）
  if (pthread_mutex_lock(&mutex) == 0) {
    printf("第一次加锁成功\n");
  }

  // 第二次加锁（递归锁允许重复加锁）
  if (pthread_mutex_lock(&mutex) == 0) {
    printf("第二次加锁成功（递归特性生效）\n");
    pthread_mutex_unlock(&mutex);  // 解锁一次
  }

  pthread_mutex_unlock(&mutex);  // 解锁第二次

  // 清理资源
  pthread_mutex_destroy(&mutex);
  pthread_mutexattr_destroy(&attr);

  return 0;
}