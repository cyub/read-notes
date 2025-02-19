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

示例代码：

```c
--8<-- "docs/multithreaded-programming-guide/src/pthread_create.c"
```

### 等待线程终止

`pthread_join()` 函数会一直阻塞调用线程，直到指定的线程终止。

```c
#include <pthread.h>

int pthread_join(pthread_t thread, void **retval);
```

- 指定的线程 thread 必须位于当前的进程中，而且不得是分离线程。
- 当 status 不是 NULL 时，status 指向某个位置，在 `pthread_join()` 成功返回时，将该位置设置为已终止线程的退出状态。

线程通过 `pthread_create()` 创建后，有可能会立即执行，此后我们依旧可以通过 `pthread_join()` 来获取线程状态信息。`pthread_join()` 的行为如下：

- 如果目标线程还在运行，`pthread_join()` 会 阻塞，直到该线程结束。
- 如果目标线程已经结束，`pthread_join()` 仍然可以成功回收它的资源（因为线程的状态会保留，直到 `pthread_join()` 调用）。
- 线程的资源不会 立即 被销毁，而是一直保持，直到 `pthread_join()` 被调用（或使用 pthread_detach() ）。
- 只有 `pthread_join()`（或者 `pthread_detach()`）才能回收线程的资源，否则可能导致 内存泄漏（此时线程变成了变为“僵尸线程”）。

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

    如果线程是可加入的（默认状态），线程结束后，其资源不会自动释放，直到其他线程调用 `pthread_join()`。如果忘记调用 `pthread_join()`，线程的资源（如栈空间、线程描述符等）将一直占用，可能导致资源泄漏。
        - 使用 `pthread_detach()` 可以让线程在结束时自动释放资源，避免资源泄漏。

2. 简化线程管理：
    
    对于一些不需要同步结果的线程，或者线程的生命周期较短，使用分离线程可以简化线程管理，无需手动调用 `pthread_join()`。

代码示例：

```c
--8<-- "docs/multithreaded-programming-guide/src/pthread_detach.c"
```

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

示例：

```c
--8<-- "docs/multithreaded-programming-guide/src/pthread_setschedparam.c"
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

### 初始化属性

`pthread_attr_init()` 将对象属性初始化为其缺省值。存储空间是在执行期间由线程系统分配的。

```c
#include <pthread.h>

int pthread_attr_init(pthread_attr_t *attr);
```

以下是使用 `pthread_attr_init` 初始化线程属性时的缺省值表格。这些值基于 **POSIX 标准** 和常见实现（如 Linux），但具体默认值可能因操作系统而异：

| 属性               | 默认值                          | 说明                                                                 |
|------------------------|------------------------------------|--------------------------------------------------------------------------|
| 分离状态 (`detachstate`)         | `PTHREAD_CREATE_JOINABLE`         | 线程默认可被连接（需调用 `pthread_join` 回收资源）。                       |
| 调度策略 (`schedpolicy`)         | `SCHED_OTHER`                     | 分时调度策略（普通线程的默认策略）。                                         |
| 调度参数 (`schedparam`)          | `sched_priority = 0`              | 对于 `SCHED_OTHER` 策略，优先级为 0（用户不可调整）。                       |
| 继承调度策略 (`inheritsched`)    | `PTHREAD_INHERIT_SCHED`           | 继承创建线程的调度策略，而非使用属性对象中设置的值。                           |
| 作用域 (`scope`)                | `PTHREAD_SCOPE_SYSTEM`            | 线程在系统范围内竞争 CPU 资源（与进程外线程竞争）。                           |
| 栈大小 (`stacksize`)            | 系统定义（如 Linux 默认 8 MiB）    | 栈大小由实现决定，通常可通过 `PTHREAD_STACK_MIN` 查询最小值。                 |
| 栈地址 (`stackaddr`)            | `NULL`                            | 系统自动分配栈内存，用户通常无需指定。                                       |
| 警戒区大小 (`guardsize`)        | 系统页大小（如 4 KiB）             | 用于检测栈溢出的保护区域大小，设为 0 表示禁用保护。                           |

**注意事项**：

1. 系统差异性：

    默认值可能因操作系统或版本不同而变化（如栈大小在 32/64 位系统中的差异）。

2. 动态查询：
   
    可通过 `pthread_attr_getxxx` 系列函数（如 `pthread_attr_getdetachstate`）动态获取当前属性值。

3. 优先级限制：

    `SCHED_OTHER` 策略下优先级固定为 0，仅 `SCHED_FIFO`/`SCHED_RR` 支持调整优先级。

4. 栈管理：

    手动设置 `stackaddr` 和 `stacksize` 需谨慎，避免栈溢出或内存冲突。

下面是获取线程默认属性的示例：

```c
--8<-- "docs/multithreaded-programming-guide/src/pthread_default_attr.c"
```

上面代码输出以下内容：

```bash
分离状态 (detachstate): JOINABLE
调度策略 (schedpolicy): SCHED_OTHER
调度优先级 (schedparam): 31
继承调度策略 (inheritsched): INHERIT
作用域 (scope): SYSTEM
栈大小 (stacksize): 524288 bytes (0.50 MiB)
栈地址 (stackaddr): 系统自动分配
警戒区大小 (guardsize): 16384 bytes (16.00 KiB)
```

### 销毁属性

```c
#include <pthread.h>

int pthread_attr_destroy(pthread_attr_t *attr);
```

### 设置分离状态

如果创建分离线程 (PTHREAD_CREATE_DETACHED)，则该线程一退出，便可重用其线程 ID 和其他资源。

```c
#include <pthread.h>

int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
```

### 获取分离状态

使用 `pthread_attr_getdetachstate()` 可以检索线程创建状态（可以为分离或连接）。

```c
#include <pthread.h>

int pthread_attr_getdetachstate(const pthread_attr_t *attr,
                                       int *detachstate);
```

### 设置栈溢出保护区大小

```c
 #include <pthread.h>

int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);
```

`pthread_attr_setguardsize()` 用于设置线程属性对象中的 警戒区大小（guard size）。警戒区是线程栈末尾预留的一段特殊内存区域，其核心目的是：

- 防止栈溢出破坏内存
    
    当线程的栈空间耗尽时（例如递归过深或局部变量过大），警戒区会作为“缓冲地带”被优先覆盖。操作系统或线程库可以通过内存保护机制（如触发 SIGSEGV 信号）检测到对警戒区的非法访问，从而避免栈溢出破坏其他内存区域（如堆或全局变量）。

- 提高程序健壮性
    
    通过提前发现栈溢出，开发者可以调试代码或采取恢复措施，避免程序因内存损坏而崩溃。

注意事项：

- 警戒区大小的对齐
    
    警戒区大小通常会被系统自动调整为 内存页大小的整数倍。例如：

    - 若系统页大小为 4 KiB（4096 字节），设置 guardsize=5000 会被调整为 8 KiB（8192 字节）。

    - 可通过 sysconf(_SC_PAGESIZE) 获取系统页大小。

2. 设置为 0 的含义

    ```c
    pthread_attr_setguardsize(&attr, 0); // 禁用警戒区
    ```
    - 禁用警戒区后，栈溢出可能无法被检测，程序可能直接覆盖其他内存区域，导致不可预知的崩溃或数据损坏。

    - 仅在需要精确控制栈内存时使用（如实时系统），并确保代码不会发生栈溢出。

3. 默认警戒区大小

    - 默认值通常为系统页大小（如 4 KiB），可通过 pthread_attr_getguardsize 查询。

    - 默认值可能因操作系统或线程库版本不同而变化。

4. 与栈地址和栈大小的关系

    - 若手动指定了线程栈地址（pthread_attr_setstackaddr），警戒区可能被自动禁用。

    - 设置的栈大小（pthread_attr_setstacksize）应远大于警戒区，否则可能导致可用栈空间不足。

示例代码：

```c
--8<-- "docs/multithreaded-programming-guide/src/pthread_attr_setguardsize.c"
```

### 获取栈溢出保护区大小

```c
#include <pthread.h>

int pthread_attr_getguardsize(const pthread_attr_t *attr,
                                     size_t *guardsize);
```

### 设置线程的争用范围

`pthread_attr_setscope()` 用于设置线程的 作用域（scope），即线程在竞争 CPU 资源时的调度范围。它决定了线程是与系统中所有线程竞争 CPU（系统级），还是仅与同一进程内的线程竞争 CPU（进程级）。具体参数如下：

作用域类型 | 说明
--- | ---
PTHREAD_SCOPE_SYSTEM | 线程在系统范围内竞争 CPU（与所有进程的线程竞争，依赖操作系统的全局调度）
PTHREAD_SCOPE_PROCESS | 线程仅在进程内竞争 CPU（由进程自行管理线程调度，需用户态调度器支持）

```c
#include <pthread.h>

int pthread_attr_setscope(pthread_attr_t *attr, int scope);
```

示例代码：

```c
--8<-- "docs/multithreaded-programming-guide/src/pthread_attr_setscope.c"
```

### 获取线程的争用范围

```c
#include <pthread.h>

int pthread_attr_getscope(const pthread_attr_t *attr, int *scope);
```

### 设置线程并行级别

```c
#include <pthread.h>

int pthread_setconcurrency(int new_level);
```

### 获取线程并行级别

```c
#include <pthread.h>

int pthread_getconcurrency(void);
```

### 设置调度策略

POSIX 规定了 3 种调度策略：
调度策略 | 说明
--- | ---
SCHED_OTHER	| 默认调度策略，适用于大多数普通线程，时间片轮转（time-sharing）。
SCHED_FIFO	| 实时调度策略，先来先执行，高优先级线程不主动让出 CPU，适用于低延迟任务。
SCHED_RR	| 实时轮转调度，与 SCHED_FIFO 类似，但线程按固定时间片轮转。

- SCHED_OTHER 是 **非实时** 调度策略，而 SCHED_FIFO 和 SCHED_RR 是 **实时** 调度策略。
- 使用 SCHED_FIFO 和 SCHED_RR 需要 root 权限（普通用户可能会失败）。

```c
#include <pthread.h>

int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
```

参数说明：

- attr：指向一个 pthread_attr_t 结构体，表示线程的属性。
- policy：指定线程的调度策略，常见值：
    - SCHED_OTHER（默认）
    - SCHED_FIFO（先来先服务调度）
    - SCHED_RR（轮转调度）

返回值：

- 成功 返回 0。
- 失败 返回 EINVAL（无效参数）等错误码。


### 获取调度策略

```c
#include <pthread.h>

int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);
```

### 设置继承的调度策略

```c
#include <pthread.h>

int pthread_attr_setinheritsched(pthread_attr_t *attr,
                                        int inheritsched);
int pthread_attr_getinheritsched(const pthread_attr_t *attr,
                                        int *inheritsched);
```

### 设置继承的调度策略

```c
#include <pthread.h>

int pthread_attr_getinheritsched(const pthread_attr_t *attr,
                                        int *inheritsched);
```

### 设置调度参数

```c
#include <pthread.h>

int pthread_attr_setschedparam(pthread_attr_t *attr,
                                      const struct sched_param *param);
```

### 获取调度参数

```c
#include <pthread.h>

int pthread_attr_getschedparam(const pthread_attr_t *attr,
                                      struct sched_param *param);
```

### 设置线程的 CPU 亲和性

```c
#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <pthread.h>

int pthread_setaffinity_np(pthread_t thread, size_t cpusetsize,
                                  const cpu_set_t *cpuset);
```

`pthread_setaffinity_np` 是 POSIX 线程库中的一个非可移植（_np 表示 "non-portable"）函数，用于设置线程的 CPU 亲和性（CPU Affinity），即将线程绑定到特定的 CPU 核心上运行。通过限制线程在指定核心上执行，可以减少上下文切换的开销、提高缓存命中率，从而优化性能（尤其在多核 CPU 的密集计算场景）。

作用: 

- 减少上下文切换

    线程固定在某个核心上运行，避免频繁切换核心导致的缓存失效。

- 隔离资源竞争

    在多线程/多进程环境中，绑定核心可以避免不同线程争抢同一核心的资源。

- 实时性要求

    实时系统中，确保关键线程独占核心，减少调度延迟。

示例代码：

```c
--8<-- "docs/multithreaded-programming-guide/src/pthread_setaffinity_np.c"
```

上面代码是否生效验证方式：

- 查看线程的 CPU 亲和性

使用 taskset 命令查看线程的绑定情况：

```bash
# 获取线程 PID（假设主程序名为 affinity_demo）
ps -eLf | grep affinity_demo

# 查看线程的 CPU 亲和性（替换为实际 PID）
taskset -cp <PID>
```

- 实时监控 CPU 占用

使用 htop 或 top 观察线程的 CPU 使用情况：

1. 运行程序后，打开 htop。
2. 按 `F2` 进入进程视图，启用 “显示自定义线程名称”。
3. 观察线程是否固定在指定核心（如 CPU 1）运行。

注意事项：

- 权限要求

    - 在 Linux 中，普通用户可能需要 CAP_SYS_NICE 权限才能修改 CPU 亲和性。

    - 以 sudo 运行程序或调整权限：

        ```shell
        sudo setcap cap_sys_nice+ep ./affinity_demo
        ```

- 核心编号范围

    - CPU 编号从 0 开始，最大值可通过 nproc --all 或 lscpu 查询。

    - 若设置超出范围的 CPU 编号，pthread_setaffinity_np 会返回 EINVAL。

### 获取线程的 CPU 亲和性

```c
#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <pthread.h>

int pthread_getaffinity_np(pthread_t thread, size_t cpusetsize,
                                  cpu_set_t *cpuset);
```

### 关于栈

通常，线程栈是从页边界开始的。任何指定的大小都被向上舍入到下一个页边界。不具备访问权限的页将被附加到栈的溢出端。大多数栈溢出都会导致将 SIGSEGV 信号发送到违例线程。将直接使用调用方分配的线程栈，而不进行修改。

指定栈时，还应使用 PTHREAD_CREATE_JOINABLE 创建线程。在该线程的 `pthread_join()` 调用返回之前，不会释放该栈。在该线程终止之前，不会释放该线程的栈。了解这类线程是否已终止的唯一可靠方式是使用 `pthread_join()`。 

#### 为线程分配栈空间

一般情况下，不需要为线程分配栈空间。系统会为每个线程的栈分配 默认的虚拟内存，而不保留任何交换空间。系统将使用 mmap() 的 MAP_NORESERVE 选项来进行分配。

系统创建的每个线程栈都具有红色区域。系统通过将页附加到栈的溢出端来创建红色区域，从而捕获栈溢出。此类页无效，而且会导致内存（访问时）故障。红色区域将被附加到所有自动分配的栈，无论大小是由应用程序指定，还是使用缺省大小。 

#### 生成自己的栈

指定线程栈大小时，必须考虑被调用函数以及每个要调用的后续函数的分配需求。需要考虑的因素应包括调用序列需求、局部变量和信息结构。

有时，您需要与缺省栈略有不同的栈。典型的情况是，线程需要的栈大小大于缺省栈大小。而不太典型的情况是，缺省大小太大。您可能正在使用不足的虚拟内存创建数千个线程，进而处理数千个缺省线程栈所需的数千兆字节的栈空间。

对栈的最大大小的限制通常较为明显，但对其最小大小的限制如何呢？必须存在足够的栈空间来处理推入栈的所有栈帧，及其局部变量等。

要获取对栈大小的绝对最小限制，请调用宏 PTHREAD_STACK_MIN。PTHREAD_STACK_MIN 宏将针对执行 NULL 过程的线程返回所需的栈空间量。有用的线程所需的栈大小大于最小栈大小，因此缩小栈大小时应非常谨慎。 

### 设置栈大小

```c
#include <pthread.h>

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
```

### 获取栈大小

```c
#include <pthread.h>

int pthread_attr_getstacksize(const pthread_attr_t *attr,
                                     size_t *stacksize);
```

### 设置栈地址和大小

```c
 #include <pthread.h>

int pthread_attr_setstack(pthread_attr_t *attr,
                                 void *stackaddr, size_t stacksize);
```

stackaddr 属性定义线程栈的基准（低位地址）。stacksize 属性指定栈的大小。如果将 stackaddr 设置为非空值，而不是缺省的 NULL，则系统将在该地址初始化栈，假设大小为 stacksize。

### 获取栈地址和大小

```c
#include <pthread.h>

int pthread_attr_getstack(const pthread_attr_t *attr,
                                 void **stackaddr, size_t *stacksize);
```

## Chap4 用同步对象编程

**同步对象** 是内存中的变量，可以按照与访问数据完全相同的方式对其进行访问。不同进程中的线程可以通过放在由线程控制的共享内存中的同步对象互相通信。尽管不同进程中的线程通常互不可见，但这些线程仍可以互相通信。同步对象还可以放在文件中。同步对象可以比创建它的进程具有更长的生命周期。

同步对象具有以下可用类型：

- 互斥锁

- 条件变量

- 读写锁

- 信号

同步的作用包括以下方面：

- **同步是确保共享数据一致性的唯一方法**。

- 两个或多个进程中的线程可以合用一个同步对象。由于重新初始化同步对象会将对象的状态设置为解除锁定，因此应仅由其中的一个协作进程来初始化同步对象。

- 同步可确保可变数据的安全性。

- 进程可以映射文件并指示该进程中的线程获取记录锁。一旦获取了记录锁，映射此文件的任何进程中尝试获取该锁的任何线程都会被阻塞，直到释放该锁为止。

- 访问一个基本类型变量（如整数）时，可以针对一个内存负荷使用多个存储周期。如果整数没有与总线数据宽度对齐或者大于数据宽度，则会使用多个存储周期。

### 互斥锁

使用互斥锁（互斥）可以使线程按顺序执行。通常，互斥锁通过确保一次只有一个线程执行代码的临界段来同步多个线程。互斥锁还可以保护单线程代码。

要更改缺省的互斥锁属性，可以对属性对象进行声明和初始化。通常，互斥锁属性会设置在应用程序开头的某个位置，以便可以快速查找和轻松修改。

#### 初始化互斥锁属性对象

```c
#include <pthread.h>

int pthread_mutexattr_init(pthread_mutexattr_t *attr);
```

#### 销毁互斥锁属性对象

```c
#include <pthread.h>

int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
```

#### 设置互斥锁的范围

```c
#include <pthread.h>

int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr,
                                        int pshared);
```

参数说明：

-   attr：指向互斥锁属性对象的指针，该对象通过 pthread_mutexattr_init 初始化。
-   pshared：指定互斥锁的共享属性，取值可以是：
    - PTHREAD_PROCESS_PRIVATE：互斥锁仅在创建它的进程内有效（默认值）。
    - PTHREAD_PROCESS_SHARED：互斥锁可以在多个进程之间共享（需配合共享内存使用）。并不是所有的系统都支持 PTHREAD_PROCESS_SHARED 属性。在使用之前，需要检查系统是否支持该特性。

返回值：

- 成功时返回 0。
- 失败时返回错误码。

示例：不同进程间使用共享锁

```c
--8<-- "docs/multithreaded-programming-guide/src/mutex_share.c"
```

#### 获取互斥锁的范围

```c
#include <pthread.h>

int pthread_mutexattr_getpshared(const pthread_mutexattr_t *attr,
                                        int *pshared);
```

#### 设置互斥锁类型的属性

```c
#include <pthread.h>

int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);
```

参数说明：

- attr：指向互斥锁属性对象的指针（需已初始化）。

- type：要设置的互斥锁类型（见下文）。

返回值：

- 成功返回 0；

- 失败返回错误码（如 EINVAL 表示无效参数）。

**互斥锁类型（type 可选值）** 如下：

类型值	|  说明
--- | ---
PTHREAD_MUTEX_NORMAL | 默认类型：无死锁检测和错误检查。重复加锁会导致未定义行为（通常死锁）。
PTHREAD_MUTEX_RECURSIVE	| 递归锁：允许同一线程多次加锁，每次加锁必须对应相同次数的解锁。
PTHREAD_MUTEX_ERRORCHECK | 错误检查锁：禁止同一线程重复加锁，尝试重复加锁会立即返回 EDEADLK 错误。
PTHREAD_MUTEX_DEFAULT | 系统默认实现：行为可能因平台而异（通常等同于 PTHREAD_MUTEX_NORMAL）。

**锁类型说明**：

- 递归锁（PTHREAD_MUTEX_RECURSIVE）
    - 同一线程可重复加锁，但必须解锁相同次数才能真正释放锁。
    - 适用于函数递归调用或嵌套访问共享资源的场景。

- 错误检查锁（PTHREAD_MUTEX_ERRORCHECK）
    - 若同一线程尝试重复加锁，会立即返回 EDEADLK 错误，避免死锁。
    - 调试时非常有用，可快速定位代码逻辑错误。

- 普通锁（PTHREAD_MUTEX_NORMAL）
    - 无任何检查，重复加锁会导致未定义行为（通常死锁）。
    - 性能较高，但需开发者自行确保逻辑正确。

示例：

```c
--8<-- "docs/multithreaded-programming-guide/src/pthread_mutex_recursive.c"
```

#### 获取互斥锁的类型属性

```c
#include <pthread.h>

int pthread_mutexattr_gettype(const pthread_mutexattr_t *restrict attr,
           int *restrict type);
```

#### 设置互斥锁的优先级协议

`pthread_mutexattr_setprotocol()` 是 POSIX 线程库中用于设置互斥锁（mutex）的 优先级协议（Priority Protocol） 的函数，主要目的是在多线程实时系统中防止 优先级反转（Priority Inversion）。

**优先级反转问题**：

当低优先级线程持有锁时，高优先级线程可能被阻塞，而中等优先级线程可能抢占低优先级线程，导致高优先级线程长时间无法执行。这种问题在实时系统中可能导致严重后果。

**解决方案**：

通过设置互斥锁的优先级协议，调整线程的优先级，确保高优先级线程能尽快获得锁。

```c
#include <pthread.h>

int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr,
           int protocol);
```

参数说明：

- attr：已初始化的互斥锁属性对象。
- protocol：协议类型

返回值：

- 成功返回 0；
- 失败返回错误码（如 ENOTSUP 表示系统不支持该协议）。

protocol 可选值如下：

协议类型	|  说明
--- | ---
PTHREAD_PRIO_NONE	| 默认：不启用优先级继承或保护（可能发生优先级反转）。
PTHREAD_PRIO_INHERIT	| 优先级继承：低优先级线程持有锁时，继承高优先级线程的优先级。
PTHREAD_PRIO_PROTECT	|  优先级天花板：线程持有锁时，优先级提升到预设的固定天花板值。

代码示例：

```c
--8<-- "docs/multithreaded-programming-guide/src/pthread_prio_inherit.c"
```
