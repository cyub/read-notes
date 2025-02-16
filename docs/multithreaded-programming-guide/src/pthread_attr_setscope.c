#include <pthread.h>
#include <stdio.h>

int main() {
  pthread_attr_t attr;
  int scope;

  pthread_attr_init(&attr);

  // 设置作用域为 SYSTEM
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
  pthread_attr_getscope(&attr, &scope);
  printf("作用域: %s\n",
         (scope == PTHREAD_SCOPE_SYSTEM) ? "SYSTEM" : "PROCESS");

  // 设置作用域为 PROCESS
  pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
  pthread_attr_getscope(&attr, &scope);
  printf("作用域: %s\n",
         (scope == PTHREAD_SCOPE_SYSTEM) ? "SYSTEM" : "PROCESS");

  pthread_attr_destroy(&attr);
  return 0;
}