#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  pthread_mutexattr_t attr;
  pthread_mutex_t mutex;

  // 初始化属性对象
  if (pthread_mutexattr_init(&attr) != 0) {
    perror("pthread_mutexattr_init failed");
    exit(EXIT_FAILURE);
  }

  // 设置优先级继承协议
  int protocol = PTHREAD_PRIO_INHERIT;
  if (pthread_mutexattr_setprotocol(&attr, protocol) != 0) {
    perror("pthread_mutexattr_setprotocol failed");
    exit(EXIT_FAILURE);
  }

  // 创建互斥锁
  if (pthread_mutex_init(&mutex, &attr) != 0) {
    perror("pthread_mutex_init failed");
    exit(EXIT_FAILURE);
  }

  // 使用互斥锁...
  pthread_mutex_lock(&mutex);
  printf("锁已获取，优先级继承生效\n");
  pthread_mutex_unlock(&mutex);

  // 清理资源
  pthread_mutex_destroy(&mutex);
  pthread_mutexattr_destroy(&attr);

  return 0;
}