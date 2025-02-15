/**
 * 源码来自 man pthread_create
 */
#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define handle_error_en(en, msg) \
  do {                           \
    errno = en;                  \
    perror(msg);                 \
    exit(EXIT_FAILURE);          \
  } while (0)

#define handle_error(msg) \
  do {                    \
    perror(msg);          \
    exit(EXIT_FAILURE);   \
  } while (0)

struct thread_info {
  pthread_t thread_id;
  int thread_num;
  char *argv_string;
};

static void *thread_start(void *arg) {
  char *p;  // 栈顶变量，原代码中没有定义这个变量
  struct thread_info *tinfo = arg;
  char *uargv;
  p = (char *)&p;  // p 指向栈顶变量（即它自己）的地址
  printf("Thread %d: top of stack near %p; argv_string=%s\n", tinfo->thread_num,
         &p, tinfo->argv_string);
  uargv = strdup(tinfo->argv_string);
  if (uargv == NULL) handle_error("strdup");

  for (char *p = uargv; *p != '\0'; p++) {
    *p = toupper((unsigned char)*p);
  }
  return uargv;
}

int main(int argc, char *argv[]) {
  int s, opt, num_threads;
  pthread_attr_t attr;
  size_t stack_size;
  void *res;

  // -s 选项用于指定栈大小
  stack_size = 0;  // 原代码中为-1，有问题。
  // 因为stack_size是size_t类型，它是无符号类型的，-1会被解释成无符号整数的最大值。
  while ((opt = getopt(argc, argv, "s:")) != -1) {
    switch (opt) {
      case 's':
        stack_size = strtoul(optarg, NULL, 0);
        break;
      default:
        fprintf(stderr, "Usage: %s [-s stack-size] arg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  num_threads = argc - optind;

  // 初始化线程属性
  s = pthread_attr_init(&attr);
  if (s != 0) handle_error_en(s, "pthread_attr_init");
  if (stack_size > 0) {  // 设置线程栈大小
    printf("stack_size=%lud\n", stack_size);
    s = pthread_attr_setstacksize(&attr, stack_size);
    if (s != 0) handle_error_en(s, "pthread_attr_setstacksize");
  }

  struct thread_info *tinfo = calloc(num_threads, sizeof(*tinfo));
  if (tinfo == NULL) handle_error("calloc");

  for (int tnum = 0; tnum < num_threads; tnum++) {
    tinfo[tnum].thread_num = tnum + 1;
    tinfo[tnum].argv_string = argv[optind + tnum];
    // 创建线程
    s = pthread_create(&tinfo[tnum].thread_id, &attr, &thread_start,
                       &tinfo[tnum]);
    if (s != 0) handle_error_en(s, "pthread_create");
  }

  // 不再需要线程属性，销毁线程属性
  s = pthread_attr_destroy(&attr);
  if (s != 0) handle_error_en(s, "pthread_attr_destroy");

  // join每个线程，并打印返回值
  for (int tnum = 0; tnum < num_threads; tnum++) {
    s = pthread_join(tinfo[tnum].thread_id, &res);
    if (s != 0) handle_error_en(s, "pthread_join");
    printf("Joined with thread %d; returend value was %s\n",
           tinfo[tnum].thread_num, (char *)res);
    free(res);
  }
  free(tinfo);
  exit(EXIT_SUCCESS);
}
