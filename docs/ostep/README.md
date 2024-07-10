# 操作系统导论

英文名称：Operating Systems: Three Easy Pieces，简称OSTEP。

- 本书开源：https://pages.cs.wisc.edu/~remzi/OSTEP/
- 中文版本：https://github.com/remzi-arpacidusseau/ostep-translations/tree/master/chinese
- 随书代码：https://github.com/remzi-arpacidusseau/ostep-code
- 家庭作业：https://github.com/remzi-arpacidusseau/ostep-homework
- 项目代码：https://github.com/remzi-arpacidusseau/ostep-projects

## 第二部分：并发

### chap30 条件变量

线程可以使用条件变量（condition variable），来等待一个条件变成真。条件变量是一个显式队列，当某些执行状态（即条件，condition）不满足时，线程可以把自己加入队列，等待（waiting）该条件。另外某个线程，当它改变了上述状态时，就可以唤醒一个或者多个等待线程（通过在该条件上发信号），让它们继续执行。Dijkstra最早在“私有信号量”中提出这种思想。Hoare后来在关于观察者的工作中，将类似的思想称为 **条件变量**。

条件变量的POSIX调用定义如下：

```c
#include <pthread.h>

// wait on a condition
int pthread_cond_wait(pthread_cond_t *restrict cond,
    pthread_mutex_t *restrict mutex);
// broadcast or signal a condition
int pthread_cond_broadcast(pthread_cond_t *cond);
// signal a condition
int pthread_cond_signal(pthread_cond_t *cond);
```

!!! note "注意"
    man手册默认没有POSIX调用文档，我们需要手动安装`apt-get install manpages-posix-dev`。

pthread_cond_wait和pthread_cond_signal，我们常简称为wait()和signal()。

我们可以使用条件变量实现pthread_join功能：

```c
#include <stdio.h>
#include <pthread.h>
int done = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void thr_exit() {
    pthread_mutex_lock(&m);
    done = 1;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
}

void *child(void *arg) {
    printf("child\n");
    thr_exit();
    return NULL;
}

void thr_join() {
    pthread_mutex_lock(&m);
    while (done==0)
        pthread_cond_wait(&c, &m);
    pthread_mutex_unlock(&m);
}

int main(int argc, char *argv[]) {
    printf("parent: begin\n");
    pthread_t p;
    pthread_create(&p, NULL, child, NULL);
    thr_join();
    printf("parent:end\n");
    return 0;
}
```
wait()调用有一个参数，它是互斥量。它假定在wait()调用时，这个互斥量是已上锁状态。wait()的职责是释放锁，并让调用线程休眠（原子地）。当线程被唤醒时（在另外某个线程发信号给它后），它必须重新获取锁，再返回调用者。这样复杂的步骤也是为了避免在线程陷入休眠时，产生一些竞态条件。

原则：**调用signal和wait时要持有锁（hold the lock when calling signal or wait)**。

#### 生产者/消费者(有界缓冲区)问题

假设有一个或多个生产者线程和一个或多个消费者线程。生产者把生成的数据项放入缓冲区；消费者从缓冲区取走数据项，以某种方式消费。因为有界缓冲区是共享资源，所以我们必须通过同步机制来访问它，以免产生竞态条件。

Mesa语义（Mesa semantic，由于最初是在Mesa编程语言中提出，所以叫Mesa语义），强调唤醒后不会立刻执行，而是重新检查一下执行条件，即将if条件改成while。

由于Mesa语义，我们要记住一条关于条件变量的简单规则：**总是使用while循环（always use while loop）**。虽然有时候不需要重新检查条件，但这样做总是安全的，做了就开心了。

信号显然需要，但必须更有指向性。消费者不应该唤醒消费者，而应该只唤醒生产者，反之亦然。

用pthread_cond_broadcast()代替上述代码中的pthread_cond_signal()，唤醒所有的等待线程。这样做，确保了所有应该唤醒的线程都被唤醒。当然，不利的一面是可能会影响性能，因为不必要地唤醒了其他许多等待的线程，它们本来（还）不应该被唤醒。这些线程被唤醒后，重新检查条件，马上再次睡眠。

Lampson和Redell把这种条件变量叫作 **覆盖条件（covering condition）**，因为它能覆盖所有需要唤醒线程的场景（保守策略）。成本如上所述，就是太多线程被唤醒。聪明的读者可能发现，在单个条件变量的生产者/消费者问题中，也可以使用这种方法。但是，在这个例子中，我们有更好的方法，因此用了它。一般来说，如果你发现程序只有改成广播信号时才能工作（但你认为不需要），可能是程序有缺陷，修复它！但在上述内存分配的例子中，广播可能是最直接有效的方案。

### chap31 信号量

信号量是与同步有关的所有工作的单一原语。因为它既可以锁实现互斥，也可以作为条件变量。

#### 定义

信号量是有一个整数值的对象，可以用两个函数来操作它。在POSIX标准中，是sem_wait()和sem_post()。因为信号量的初始值能够决定其行为，所以首先要初始化信号量，才能调用其他函数与之交互。

```c
#include <semaphore.h>
sem_t s;
sem_init(&s, 0, 1); // 0表示信号量为同一进程的多个线程共享的，
// 1表示信号量的初始值
```
首先，sem_wait()要么立刻返回（调用sem_wait()时，信号量的值大于等于1），要么会让调用线程挂起，直到之后的一个post操作。

其次，sem_post()并没有等待某些条件满足。它直接增加信号量的值，如果有等待线程，唤醒其中一个。

最后，**当信号量的值为负数时，这个值就是等待线程的个数**（比如：信号量为1时，当线程0调用 sem_wait()，它可以获得临界区操作权限，此时信号量变为 0.当线程1调用 sem_wait()，信号量变为-1，且线程1进入睡眠）。

#### 二值信号量（锁）

**当信号量的初始值是1时候，信号量可以来实现锁**。因为锁只有两个状态（持有和没持有），所以这种用法有时也叫作 **二值信号量（binary semaphore）**。

```c
sem_t m;
sem_init(&m, 0, 1);

sem_wait(&m);
// critical setion here
sem_post(&m);
```

#### 信号量用作条件变量

信号量也可以用在一个线程暂停执行，等待某一条件成立的场景。例如，一个线程要等待一个链表非空，然后才能删除一个元素。在这种场景下，通常一个线程等待条件成立，另外一个线程修改条件并发信号给等待线程，从而唤醒等待线程。因为等待线程在等待某些条件（condition）发生变化，所以我们 **将信号量作为条件变量（condition variable），此时的信号量初始值应该是0**。

```c
sem_t s;

void *child(void *arg) {
    printf("child\n");
    sem_post(&s); // 发送信号，子现场完成了
    return NULL;
}

int main(int argc, char *argv[]) {
    sem_init(&s, 0, 0);
    printf("parent: begin\n");
    pthread_t c;
    pthread_create(c, NULL, child, NULL);
    sem_wait(&s); // 等待子线程
    printf("parent: end\n");
    return 0;
}
```

#### 生产者/消费者（有界缓冲区）问题

#### 读者-写者锁

```c
#include <semaphore.h>
#include <stdio.h>

typedef struct rwlock_t {
  sem_t lock;       // binay semaphore(basic lock)
  sem_t writelock;  // used to allow ONE writer o MANY readers
  int readers;      // count of readers reaing in critical sectino
} rwlock_t;

void rwlock_init(rwlock_t *rw) {
  rw->readers = 0;
  sem_init(&rw->lock, 0, 1);
  sem_init(&rw->writelock, 0, 1);
}

void rwlock_acquire_readlock(rwlock_t *rw) {
  sem_wait(&rw->lock);
  rw->readers++;
  if (rw->readers == 1)
    sem_wait(&rw->writelock);  // first reader acquires writelock

  sem_post(&rw->lock);
}

void rwlock_release_readlock(rwlock_t *rw) {
  sem_wait(&rw->lock);
  rw->readers--;
  if (rw->readers == 0)
    sem_post(&rw->writelock);  // last reader releases writelock

  sem_post(&rw->lock);
}

void rwlock_acquire_writelock(rwlock_t *rw) { sem_wait(&rw->writelock); }

void rwlock_release_writelock(rwlock_t *rw) { sem_post(&rw->writelock); }
```

上面实现属于读者优先的读写锁，该实现没有达到公平性，读者很容易饿死写者。**读写锁减小了读操作的锁粒度，读写比越大，读操作越慢，性能越好**。
