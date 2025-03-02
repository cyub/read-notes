#include <pthread.h>
#include <stdio.h>

int main() {
  pthread_mutex_t mutex;
  pthread_mutexattr_t attr;
  int ceiling, old_ceiling;

  // 初始化互斥锁属性
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setprotocol(
      &attr, PTHREAD_PRIO_PROTECT);  // 必须设置协议为 PROTECT

  // 创建互斥锁
  pthread_mutex_init(&mutex, &attr);

// 设置优先级天花板为 10
#ifdef HAVE_PTHREAD_PRIORITY_CEILING
  pthread_mutex_setprioceiling(&mutex, 10, &old_ceiling);
  printf("Old ceiling: %d\n", old_ceiling);
#else
  printf("No priority ceiling support\n");
#endif

#ifdef HAVE_PTHREAD_PRIORITY_CEILING
  // 获取优先级天花板
  pthread_mutex_getprioceiling(&mutex, &ceiling);
  printf("Current ceiling: %d\n", ceiling);
#else
  printf("No priority ceiling support\n");
#endif

  // 清理
  pthread_mutex_destroy(&mutex);
  pthread_mutexattr_destroy(&attr);
  return 0;
}