#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>

// 线程函数：打印当前运行的 CPU 核心
void* thread_func(void* arg) {
  int cpu_id = *((int*)arg);
  printf("线程启动，预期绑定到 CPU %d\n", cpu_id);

  // 获取当前 CPU 核心编号
  int current_cpu = sched_getcpu();
  printf("线程实际运行在 CPU %d\n", current_cpu);

  // 模拟计算密集型任务
  while (1) {
  }  // 死循环，便于观察 CPU 占用
  return NULL;
}

int main() {
  pthread_t tid;
  int cpu_id = 1;  // 绑定到 CPU 1

  // 初始化 CPU 亲和性掩码
  cpu_set_t cpuset;
  CPU_ZERO(&cpuset);         // 清空集合
  CPU_SET(cpu_id, &cpuset);  // 添加 CPU 1 到集合

  // 创建线程属性对象并设置亲和性
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  if (pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuset) != 0) {
    perror("pthread_attr_setaffinity_np 失败");
    return 1;
  }

  // 创建线程
  if (pthread_create(&tid, &attr, thread_func, &cpu_id) != 0) {
    perror("pthread_create 失败");
    return 1;
  }

  // 等待线程（实际不会结束）
  pthread_join(tid, NULL);
  pthread_attr_destroy(&attr);
  return 0;
}