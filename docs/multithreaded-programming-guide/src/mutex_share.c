#include <pthread.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int shm_id = shmget(IPC_PRIVATE, sizeof(pthread_mutex_t), IPC_CREAT | 0666);
  pthread_mutex_t *mutex = (pthread_mutex_t *)shmat(shm_id, NULL, 0);

  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
  pthread_mutex_init(mutex, &attr);

  int pid = fork();
  if (pid == 0) {
    // 子进程
    printf("子进程尝试获取锁\n");
    pthread_mutex_lock(mutex);
    printf("子进程获取到锁\n");
    sleep(2);
    pthread_mutex_unlock(mutex);
    printf("子进程释放掉锁\n");
  } else {
    // 父进程
    printf("父进程尝试获取锁\n");
    pthread_mutex_lock(mutex);
    printf("父进程获取到锁\n");
    sleep(2);
    pthread_mutex_unlock(mutex);
    printf("父进程释放掉锁\n");
    wait(NULL);
  }

  pthread_mutex_destroy(mutex);
  shmdt(mutex);
  shmctl(shm_id, IPC_RMID, NULL);

  return 0;
}