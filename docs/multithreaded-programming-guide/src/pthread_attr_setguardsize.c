#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
static void *thread_start(void *arg) {
  printf("Thread started\n");
  return NULL;
}
int main() {
  pthread_attr_t attr;
  size_t guardsize = 4096;  // 设置为 4 KiB

  // 初始化线程属性对象
  if (pthread_attr_init(&attr) != 0) {
    perror("pthread_attr_init failed");
    return 1;
  }

  // 设置警戒区大小
  int ret = pthread_attr_setguardsize(&attr, guardsize);
  if (ret != 0) {
    fprintf(stderr, "Error: %s\n", strerror(ret));
    pthread_attr_destroy(&attr);
    return 1;
  }

  // 创建线程时使用该属性对象
  pthread_t thread;
  if (pthread_create(&thread, &attr, thread_start, NULL) != 0) {
    perror("pthread_create failed");
    pthread_attr_destroy(&attr);
    return 1;
  }

  // 销毁属性对象
  pthread_attr_destroy(&attr);
  pthread_join(thread, NULL);
  return 0;
}