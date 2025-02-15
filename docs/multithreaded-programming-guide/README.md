# 多线程编程指南

- 来自Oracle Solaris中[Multithreaded Programming Guide](https://docs.oracle.com/cd/E37838_01/html/E61057/index.html)官方文档。相比原文档增加了部分内容。

- 中文（官方旧文档的翻译版本）：[多线程编程指南](https://docs.oracle.com/cd/E19253-01/819-7051/index.html)



## Chap1 多线程基础介绍

### 定义多线程术语

术语 | 定义
--- | ---
Process（进程） | 通过 fork(2) 系统调用创建的 UNIX 环境（如文件描述符和用户 ID 等），为运行程序而设置。
Thread（线程）| 在进程上下文中执行的指令序列。 
POSIX pthread | 符合 POSIX 线程的线程接口。 
single-threaded（单线程）| 仅允许访问一个线程。 
Multithreading（多线程）| 允许访问两个或多个线程。 
User-level or Application-level thread（用户级线程或应用程序级线程）| 在用户空间（而非内核空间）中由线程调度例程管理的线程。 
Lightweight process（轻量进程）| 用来执行内核代码和系统调用的内核线程，又称作 LWP。每个线程都有一个专用的 LWP。 
Attribute object（属性对象） | 包含不透明数据类型和相关处理函数。这些数据类型和函数可以对 POSIX 线程一些可配置的方面，例如互斥锁 (mutex) 和条件变量，进行标准化。 
Mutual exclusion lock（互斥锁） | 用来锁定和解除锁定对共享数据访问的函数。 
Condition variable（条件变量）|用来阻塞线程直到状态发生变化的函数。 
Read-write lock（读写锁） | 可用于对共享数据进行多次只读访问的函数，但是要修改共享数据则必须以独占方式访问。 
Counting semaphore（计数信号量）| 一种基于内存的同步机制。 
Parallelism（并行性） | 如果至少有两个线程正在同时执行，则会出现此情况。
Concurrency（并发性） | 如果至少有两个线程正在进行，则会出现此情况。并发是一种更广义的并行性，其中可以包括分时这种形式的虚拟并行性。

### 多线程的益处

- 提高应用程序的响应
- 更有效地使用多处理器
- 改进程序结构
- 占用较少的系统资源(相比多进程)

### 多线程概念

#### 并发性和并行性

在单个处理器的多线程进程中，**处理器可以在线程之间切换执行资源，从而执行并发**。

在共享内存的多处理器环境内的同一个多线程进程中，进程中的 **每个线程都可以在一个单独的处理器上并发运行，从而执行并行**。如果进程中的线程数不超过处理器的数目，则线程的支持系统和操作环境可确保每个线程在不同的处理器上执行。例如，在线程数和处理器数目相同的矩阵乘法中，每个线程和每个处理器都会计算一行结果。

#### 多线程结构一览

传统的 UNIX 已支持多线程的概念。**每个进程都包含一个线程，因此对多个进程进行编程即是对多个线程进行编程。但是，进程同时也是一个地址空间，因此创建进程会涉及到创建新的地址空间**。

创建线程比创建新进程成本低，因为新创建的线程使用的是当前进程的地址空间。相对于在进程之间切换，**在线程之间进行切换所需的时间更少，因为后者不包括地址空间之间的切换**。

在进程内部的线程间通信很简单，因为这些线程会共享所有内容，特别是地址空间。所以，一个线程生成的数据可以立即用于其他所有线程。

##### 用户级线程

线程是多线程编程中的主编程接口。线程仅在进程内部是可见的，进程内部的线程会共享诸如地址空间、打开的文件等所有进程资源。

###### 用户级线程状态

以下状态对于每个线程是唯一的。

- 线程 ID
- 寄存器状态（包括 PC 和栈指针）
- 栈
- 信号掩码
- 优先级
- 线程专用存储

由于线程可共享进程指令和大多数进程数据，因此一个线程对共享数据进行的更改对进程内其他线程是可见的。一个线程需要与同一个进程内的其他线程交互时，该线程可以在不涉及操作系统的情况下进行此操作。

##### 内核级线程

#### 线程调度

POSIX 标准指定了三种调度策略：**先入先出策略 (SCHED_FIFO)**、**循环策略 (SCHED_RR)** 和 **自定义策略 (SCHED_OTHER)**。

SCHED_FIFO 是基于队列的调度程序，对于每个优先级都会使用不同的队列。SCHED_RR 与 FIFO 相似，不同的是前者的每个线程都有一个执行时间配额。

SCHED_FIFO 和 SCHED_RR 是对 POSIX Realtime 的扩展。SCHED_OTHER 是缺省的调度策略。

提供了两个调度范围：**进程范围 (PTHREAD_SCOPE_PROCESS)** 和 **系统范围 (PTHREAD_SCOPE_SYSTEM)**。具有不同范围状态的线程可以在同一个系统甚至同一个进程中共存。进程范围只允许这种线程与同一进程中的其他线程争用资源，而系统范围则允许此类线程与系统内的其他所有线程争用资源。

#### 线程取消

一个线程可以请求终止同一个进程中的其他任何线程。目标线程（要取消的线程）可以延后取消请求，并在该线程处理取消请求时执行特定于应用程序的清理操作。

通过 pthread 取消功能，可以对线程进行异步终止或延迟终止。异步取消可以随时发生，而延迟取消只能发生在所定义的点。延迟取消是缺省类型。

#### 线程同步

使用同步功能，可以控制程序流并访问共享数据，从而并发执行多个线程。

共有四种同步模型：互斥锁、读写锁、条件变量和信号。

- 互斥锁仅允许每次使用一个线程来执行特定的部分代码或者访问特定数据。
- 读写锁允许对受保护的共享资源进行并发读取和独占写入。要修改资源，线程必须首先获取互斥写锁。只有释放所有的读锁之后，才允许使用互斥写锁。
- 条件变量会一直阻塞线程，直到特定的条件为真。
- 计数信号量通常用来协调对资源的访问。使用计数，可以限制访问某个信号的线程数量。达到指定的计数时，信号将阻塞。

## Chap2 基本线程编程

### 创建缺省线程

如果未指定属性对象，则该对象为 NULL，系统会创建具有以下属性的缺省线程：

- 进程范围
- 非分离
- 缺省栈和缺省栈大小
- 零优先级

还可以用 `pthread_attr_init()` 创建缺省属性对象，然后使用该属性对象来创建缺省线程。

```c
#include <pthread.h>

int	pthread_create(pthread_t *tid, const pthread_attr_t *tattr,void*(*start_routine)(void *), void *arg);
```

- 当 pthread_create() 成功时，所创建线程的 ID 被存储在由 tid 指向的位置中。
- start_routine 是新线程最先执行的函数。当 start_routine 返回时，该线程将退出，其退出状态设置为由 start_routine 返回的值。请参见pthread_create 语法。
- 使用 NULL 属性参数或缺省属性调用 pthread_create() 时，pthread_create() 会创建一个缺省线程。在对 tattr 进行初始化之后，该线程将获得缺省行为。

```c
// 创建缺省线程
ret = pthread_create(&tid, NULL, start_routine, arg);

// 缺省线程属性
pthread_attr_t attr;
ret = pthread_attr_init(&attr);
ret = pthread_create(&tid, &attr, start_routine, arg);
```

### 等待线程终止

`pthread_join()` 函数会一直阻塞调用线程，直到指定的线程终止。

```c
#include <pthread.h>

int pthread_join(pthread_t thread, void **retval);
```

- 指定的线程 thread 必须位于当前的进程中，而且不得是分离线程。
- 当 status 不是 NULL 时，status 指向某个位置，在 `pthread_join()` 成功返回时，将该位置设置为已终止线程的退出状态。

### 分离线程

```c
#include <pthread.h>
int pthread_detach(pthread_t thread);
```

`pthread_detach()` 的主要作用是将线程设置为分离状态。当线程被设置为分离状态后，它在运行结束后会自动释放其所占用的资源，而不需要其他线程通过 `pthread_join()` 来回收资源。

除了使用 `pthread_detach()`，还可以在创建线程时通过线程属性（pthread_attr_t）将线程设置为分离状态。例如：

```c
pthread_attr_t attr;
pthread_attr_init(&attr);
pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
pthread_create(&thread, &attr, thread_function, NULL);
```

使用场景：

1. 避免资源泄漏：
    
    如果线程是可加入的（默认状态），线程结束后，其资源不会自动释放，直到其他线程调用 pthread_join()。如果忘记调用 pthread_join()，线程的资源（如栈空间、线程描述符等）将一直占用，可能导致资源泄漏。
        使用 pthread_detach() 可以让线程在结束时自动释放资源，避免资源泄漏。
2. 简化线程管理：
    
    对于一些不需要同步结果的线程，或者线程的生命周期较短，使用分离线程可以简化线程管理，无需手动调用 pthread_join()。

### 获取线程标识符

```c
#include <pthread.h>

pthread_t pthread_self(void);
```

`pthread_self()` 返回调用线程的 thread identifier。

### 比较线程 ID

```c
#include <pthread.h>

int pthread_equal(pthread_t t1, pthread_t t2);
```

如果 t1 和 t2 相等，`pthread_equal()` 将返回非零值，否则将返回零。如果 t1 或 t2 是无效的线程标识号，则结果无法预测。

### 停止执行线程

使用 `sched_yield()`，可以使当前线程停止执行，以便执行另一个具有相同或更高优先级的线程。

```c
#include <sched.h>

int sched_yield(void);
```

### 设置线程的调度策略

```c
#include <pthread.h>

int pthread_setschedparam(pthread_t thread, int policy,
                            const struct sched_param *param);
```

**参数说明**：

- pthread_t thread：要设置调度参数的线程的标识符，通常通过 pthread_create 创建线程后获得。
- int policy：指定线程的调度策略。常见的调度策略包括：
    - SCHED_FIFO：先进先出调度策略，实时调度策略，线程运行直到阻塞或主动放弃 CPU。
    - SCHED_RR：时间片轮转调度策略，实时调度策略，线程按时间片运行。
    - SCHED_OTHER：默认的非实时调度策略，通常用于普通应用程序。
- const struct sched_param *param：指向 sched_param 结构的指针，用于传递具体的调度参数，主要是线程的优先级。在 SCHED_FIFO 和 SCHED_RR 策略下，优先级范围通常是 1（最低）到 99（最高）。

**返回值**：
- 成功时返回 0。
- 失败时返回错误码（非零值），例如权限不足或参数无效。

### 获取线程的调度策略

```c
#include <pthread.h>

int pthread_getschedparam(pthread_t thread, int *policy,
                            struct sched_param *param);
```

### 向线程发送信号

向线程发送信号。

```c
#include <signal.h>

int pthread_kill(pthread_t thread, int sig);
```

`pthread_kill()` 将信号 sig 发送到由 tid 指定的线程。tid 所指定的线程必须与调用线程在同一个进程中。sig 参数必须来自 `signal(5)` 提供的列表。

如果 sig 为零，将执行错误检查，但并不实际发送信号。此错误检查可用来检查 tid 的有效性。

### 访问/设置线程的信号掩码

```c
#include <signal.h>

int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset);
```

**参数说明**：

- how：指定对信号掩码的操作方式，可能的值包括：
    - SIG_BLOCK：将 set 中的信号添加到当前线程的信号掩码中。
    - SIG_UNBLOCK：从当前线程的信号掩码中移除 set 中的信号。
    - SIG_SETMASK：将当前线程的信号掩码直接设置为 set 中的值。
- set：指向一个 sigset_t 类型的信号集，用于指定要操作的信号。
- oldset：用于存储操作前的信号掩码。如果不需要获取旧的信号掩码，可以将此参数设置为 NULL。

**返回值**： 

- 成功时返回 0。
- 失败时返回错误码，例如 EINVAL 表示 how 参数无效。

**使用场景**：

- 阻塞信号：在执行关键代码时，可以使用 SIG_BLOCK 阻塞某些信号，以防止信号中断。
- 解除阻塞信号：使用 SIG_UNBLOCK 可以解除之前阻塞的信号。
- 设置新的信号掩码：使用 SIG_SETMASK 可以直接设置线程的信号掩码。

**注意事项**：

- 不能阻塞 SIGKILL 和 SIGSTOP 等信号。
- 信号掩码的操作会影响整个线程，因此需要谨慎使用。
- 在多线程环境中，信号处理需要特别小心，以避免竞态条件。

### 终止线程

```c
#include <pthread.h>

void pthread_exit(void *retval);
```

- `pthread_exit()` 会立即终止调用它的线程。线程的资源（如栈空间、线程描述符等）会在适当的时候被释放。
    - 在线程终止之前，`pthread_exit()` 会触发线程的清理处理程序（cleanup handlers），这些处理程序可以通过 `pthread_cleanup_push()` 和 `pthread_cleanup_pop()` 设置。
    清理处理程序的执行顺序是后进先出（LIFO），即最后设置的清理处理程序会最先执行。
- 如果线程是可加入的（joinable），其他线程可以通过 `pthread_join()` 等待该线程结束并获取返回值。
    - `pthread_exit()` 允许线程返回一个状态值（retval），通常是一个指针或整数值。这个值可以被其他线程通过 `pthread_join()` 获取。
- 如果线程是分离的（detached），线程的资源会在线程终止时自动释放。

### 结束

线程可通过以下方法来终止执行：

- 从线程的第一个（最外面的）过程返回，使线程启动例程。
- 调用 pthread_exit()，提供退出状态。
- 使用 POSIX 取消函数执行终止操作。请参见 pthread_cancel()。

线程的缺省行为是拖延，直到其他线程通过 "joining" 拖延线程确认其已死亡。此行为与非分离的缺省 pthread_create() 属性相同，请参见 pthread_detach。join 的结果是 joining 线程得到已终止线程的退出状态，已终止的线程将消失。

有一个重要的特殊情况，即当初始线程（即调用 main() 的线程）从 main() 调用返回时或调用 exit() 时，整个进程及其所有的线程将终止。因此，一定要确保初始线程不会从 main() 过早地返回。

请注意，如果主线程仅仅调用了 pthread_exit()，则仅主线程本身终止。进程及进程内的其他线程将继续存在。所有线程都已终止时，进程也将终止。

### 取消线程

取消操作允许线程请求终止其所在进程中的任何其他线程。不希望或不需要对一组相关的线程执行进一步操作时，可以选择执行取消操作。

#### 取消点

取消点是线程检查取消请求的点。仅当取消操作安全时才应取消线程。pthreads 标准指定了几个取消点，其中包括：

- 通过 pthread_testcancel 调用以编程方式建立线程取消点。
- 线程等待 pthread_cond_wait 或 pthread_cond_timedwait(3C) 中的特定条件出现。
- 被 sigwait(2) 阻塞的线程。
- 一些标准的库调用。通常，这些调用包括线程可基于其阻塞的函数(如 sleep、select、read 等)。更多可取消点可以在 `man 7 pthreads` 中找到。

##### 取消状态和取消类型

线程的取消行为由其 **取消状态** 和 **取消类型** 决定。

**取消状态**：

- PTHREAD_CANCEL_ENABLE：线程允许响应取消请求（默认状态）。
- PTHREAD_CANCEL_DISABLE：线程不允许响应取消请求。

可以使用 pthread_setcancelstate 设置线程的取消状态：

```c
int pthread_setcancelstate(int state, int *oldstate);
```

取消类型：

- PTHREAD_CANCEL_DEFERRED：线程在到达取消点时响应取消请求（默认类型）。
- PTHREAD_CANCEL_ASYNCHRONOUS：线程在收到取消请求的瞬间立即响应。

可以使用 pthread_setcanceltype 设置线程的取消类型：

```c
int pthread_setcanceltype(int type, int *oldtype);
```

### 启用或禁用取消功能

创建线程时，缺省情况下线程取消功能处于启用状态。

```c
#include <pthread.h>

int pthread_setcancelstate(int state, int *oldstate);
```

### 设置取消类型

使用 `pthread_setcanceltype()` 可以将取消类型设置为延迟或异步模式。

```c
#include <pthread.h>

int pthread_setcanceltype(int type, int *oldtype);
```

### 创建取消点

```c
#include <pthread.h>

void pthread_testcancel(void);
```

请务必仅在线程取消操作安全的序列中插入 **pthread_testcancel()**。

### 清理处理函数压入清理栈

```c
#include <pthread.h>

void pthread_cleanup_push(void (*routine)(void *),
                                 void *arg);
```

### 从清理栈中弹出清理处理函数

```c
#include <pthread.h>
void pthread_cleanup_pop(int execute);
```

## Chap3 线程属性

