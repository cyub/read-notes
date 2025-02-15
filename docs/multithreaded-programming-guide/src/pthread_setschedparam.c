#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>

void *thread_function(void *arg) {
  printf("Thread is running with new scheduling parameters.\n");
  pthread_exit(NULL);
}

void print_schedparam(pthread_t thread) {
  struct sched_param get_param;
  int get_policy;
  if (pthread_getschedparam(thread, &get_policy, &get_param) != 0) {
    perror("pthread_getschedparam");
  }

  // 打印获取的调度策略和优先级
  printf("线程调度策略: %d\n", get_policy);
  printf("线程调度优先级: %d\n", get_param.sched_priority);
}

int main() {
  pthread_t thread;
  struct sched_param param;
  int policy = SCHED_RR;     // 设置为时间片轮转调度策略
  param.sched_priority = 50; // 设置优先级为 50

  // 创建线程
  if (pthread_create(&thread, NULL, thread_function, NULL) != 0) {
    perror("pthread_create");
    return 1;
  }

  // 获取线程的默认调度策略和参数
  print_schedparam(thread);

  // 设置线程的调度策略和参数
  if (pthread_setschedparam(thread, policy, &param) != 0) {
    perror("pthread_setschedparam");
    return 1;
  }

  // 获取线程的调度策略和参数
  print_schedparam(thread);

  // 等待线程结束
  if (pthread_join(thread, NULL) != 0) {
    perror("pthread_join");
    return 1;
  }

  return 0;
}