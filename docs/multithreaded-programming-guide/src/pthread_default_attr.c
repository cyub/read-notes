#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 将调度策略的整数值转换为字符串
const char* sched_policy_to_str(int policy) {
  switch (policy) {
    case SCHED_OTHER:
      return "SCHED_OTHER";
    case SCHED_FIFO:
      return "SCHED_FIFO";
    case SCHED_RR:
      return "SCHED_RR";
    default:
      return "Unknown";
  }
}

// 将分离状态转换为字符串
const char* detach_state_to_str(int state) {
  switch (state) {
    case PTHREAD_CREATE_JOINABLE:
      return "JOINABLE";
    case PTHREAD_CREATE_DETACHED:
      return "DETACHED";
    default:
      return "Unknown";
  }
}

// 将继承调度策略转换为字符串
const char* inherit_sched_to_str(int inherit) {
  switch (inherit) {
    case PTHREAD_INHERIT_SCHED:
      return "INHERIT";
    case PTHREAD_EXPLICIT_SCHED:
      return "EXPLICIT";
    default:
      return "Unknown";
  }
}

// 将作用域转换为字符串
const char* scope_to_str(int scope) {
  switch (scope) {
    case PTHREAD_SCOPE_SYSTEM:
      return "SYSTEM";
    case PTHREAD_SCOPE_PROCESS:
      return "PROCESS";
    default:
      return "Unknown";
  }
}

int main() {
  pthread_attr_t attr;
  int ret;

  // 初始化线程属性对象
  if ((ret = pthread_attr_init(&attr)) != 0) {
    fprintf(stderr, "pthread_attr_init error: %s\n", strerror(ret));
    exit(EXIT_FAILURE);
  }

  // 获取并打印各个属性值
  int detach_state;
  pthread_attr_getdetachstate(&attr, &detach_state);
  printf("分离状态 (detachstate): %s\n", detach_state_to_str(detach_state));

  int sched_policy;
  pthread_attr_getschedpolicy(&attr, &sched_policy);
  printf("调度策略 (schedpolicy): %s\n", sched_policy_to_str(sched_policy));

  struct sched_param sched_param;
  pthread_attr_getschedparam(&attr, &sched_param);
  printf("调度优先级 (schedparam): %d\n", sched_param.sched_priority);

  int inherit_sched;
  pthread_attr_getinheritsched(&attr, &inherit_sched);
  printf("继承调度策略 (inheritsched): %s\n",
         inherit_sched_to_str(inherit_sched));

  int scope;
  pthread_attr_getscope(&attr, &scope);
  printf("作用域 (scope): %s\n", scope_to_str(scope));

  size_t stack_size;
  pthread_attr_getstacksize(&attr, &stack_size);
  printf("栈大小 (stacksize): %zu bytes (%.2f MiB)\n", stack_size,
         (double)stack_size / (1024 * 1024));

  void* stack_addr;
  pthread_attr_getstackaddr(&attr, &stack_addr);
  printf("栈地址 (stackaddr): %s\n",
         (stack_addr == NULL) ? "系统自动分配" : "用户指定");

  size_t guard_size;
  pthread_attr_getguardsize(&attr, &guard_size);
  printf("警戒区大小 (guardsize): %zu bytes (%.2f KiB)\n", guard_size,
         (double)guard_size / 1024);

  // 销毁属性对象
  pthread_attr_destroy(&attr);

  return 0;
}