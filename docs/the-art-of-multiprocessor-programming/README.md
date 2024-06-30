## 第三章：并发对象

基于锁的FIFO队列<sup>P47</sup>

单入队者/单出队者FIFO队列<sup>P48<sup>

Java双重校验上锁<sup>P58<sup>

java中volatile可以防止指令重排，另外volatile包含内存屏障(memory barrier)会强制写入会共享存储器中，使其他线程缓存失效，这种叫 {--可线性化--} :material-information-outline:{ title="笔者认为此处不够准确，可参见下面的【关于volatile是不是线性化的?】" } 的<sup>P58 3.8.2</sup>。


??? tip "关于volatile是不是线性化的?"
    在Java中，`volatile` 关键字用于确保变量的更新对所有线程都是可见的，并且每次读取变量都是从主内存中获取最新值。`volatile` 变量的写操作会立即刷新到主内存中，而读操作会直接从主内存中读取，而不是从线程的工作内存（或缓存）中读取。

    然而，`volatile` 并不保证操作的原子性，除了对 `long` 和 `double` 类型的非易失性变量的读写操作（这些操作在Java内存模型中是原子的）之外。这意味着如果一个复合操作由多个步骤组成，即使这些步骤单独是原子的，`volatile` 也无法保证整个复合操作的原子性。

    至于“可线性化”（Linearizability）这个概念，它是一个与并发控制有关的概念，用于描述一个对象在并发环境中的行为是否与该对象在串行环境中的行为相同。线性化是一种很强的一致性保证，它要求系统能够在某个时间点保证操作的原子性和一致性。

    Java内存模型（JMM）提供了一些同步机制，如 `synchronized` 和 `java.util.concurrent` 包中的原子类，这些机制可以提供线性化的保证。但是，`volatile` 本身并不提供线性化的保证，因为它不保证复合操作的原子性，也无法保证在多步骤操作中的一致性。

    例如，一个简单的 `volatile` 变量的读写可以保证可见性，但如果你需要执行一个复合操作，比如增加计数器然后检查一个阈值，你就需要额外的同步机制来确保这个操作是线性化的。

    总结来说，**Java中的 `volatile` 关键字确保了可见性，但它并不提供线性化的保证**。要实现线性化的保证，通常需要使用更高级的同步机制。


## 第五章：同步原子操作的相对能力

如果对一个并发对象的每一次方法调用都能在有限步内完成，则称该并发对象的实现是无等待的。
如果能保证某个方法的无限次调用都能在有限步内完成，则称为该方法是无锁的。

### 读-改-写操作

由多处理器硬件所提供的大多数经典同步操作可以表示为读-改-写（RMW）操作。按照它们的对象术语，称为读-改-写寄存器。

支持RMW方法：

- getAndSet(v)，或者称为swap。用v原子地替换寄存器的当前值，并返回先前值。
- getAndIncrement： 将寄存器的当前值原子地加1并发返回先前值
- getAndAdd(k)： 将寄存器的当前值原子地加k并返回先前只
- compareAndSet： 该方法使用两个参数值：期望值e和更新值u。如果寄存器值等于e，则u原子地替换它，否则不改变。同时，该方法返回一个布尔值以说明寄存器是否被改变。
- get：返回寄存器值

## 第七章：自旋锁与争用

任何互斥协议都会产生这样的问题：如果不能获得锁，应该怎么做？对此有两种选择。

一种方案是让其继续进行尝试，这种锁称为自旋锁，对锁的反复测试过程称为旋转或忙等待。
Filter和Bakery算法都属于自旋锁。在希望 **锁延迟较短** 的情形下，选择旋转的方式比较合乎情
理。显然，只有 **在多处理器中旋转** 才有实际意义。

另一种方案就是挂起自己，请求操作系统调度器在处理器上调度另外一个线程，这种方式称为阻塞。由于从一个线程切换到另一个线程的代价比较大，所以只有在允许锁延迟较长的情形下，阻塞才有意义。许多操作系统将这两种策略综合起来使用，先旋转一个小的时间段然后再阻塞。旋转和阻塞都是重要的技术。

### 测试-设置锁
testAndSet()操作是大多数早期的多处理器系统结构所提供的主要同步指令。

testAndSet()操作将true值原子地存在内存中，然后返回这个字的先前值。传统的testAndSet()指令，就如同是对getAndSet(true)的一次调用。使用属于测试-设置为了与习惯用法保持一致。

```java
public class TASLock implements Lock{
    AtomicBoolean state = new AtomicBoolean(false);
    public void lock() {
        while (state.getAndSet(true)) {}
    }

    public void unlock() {
        state.set(false);
    }
}
```

### 测试-测试-设置锁

测试-测试-设置（TTASLock）：线程反复读取锁，直到该锁看起来是空闲的(即get()返回false)，这时候才使用testAndSet()。

```java
public class TTASLock implement Lock {
    AtomicBoolean state = new AtomicBoolean(false);
    public void lock() {
        while(true) {
            while(state.get()) {};
            if (!state.getAndSet(true))
                return;
        }
    }
}

public void unlock() {
    state.set(false);
}
```

为简单起见，考虑一种典型的多处理器系统结构，其中处理器之间是通过一种称为总线(类似一个微型以太网）的共享广播媒介进行通信的。处理器和存储控制器都可以在总线上广播，但在一个时刻只能有一个处理器（或存储控制器）在总线上广播。所有的处理器（存储控制器）都可以监听。尽管基于总线的系统结构在处理器数量很多的情形下可扩展性很差，但这种系统结构在今天非常普遍，其原因在于它们易于构建。

每个处理器都有一个cache，它是一种高速的小容量存储器，用来存放处理器感兴趣的数据。对内存的访问通常要比对cache的访问多出几个数量级的机器周期。目前技术的发展对此问题的解决效果并不理想：内存访问时间在近期内不太可能赶上处理器的时间周期，因此cache的性能对于多处理器系统结构的整体性能具有至关重要的影响。

当处理器从内存地址中读数据时，首先检查该地址及其所存储的数据是否已在它的cache中。如果在cache中，那么处理器产生一个cache命中，并可以立即加载这个值。如果不在，则产生一个cache铁失，且必须在内存或另一个处理器的cache中查找这个数据。接着，处理器在总线上广播这个地址。其他的处理器监听总线。如果某个处理器在自己的cache中发现这个地址，则广播该地址及其值来做出响应。如果所有处理器中都没有发现此地址，则以内存中该地址所对应的值来进行响应。

从正确性的角度来看，TASLock和TTASLock算法是等价的：每一个算法都保证了无死锁的互斥。但TTASLock性能要好于TASLock。因为getAndSet()调用能够迫使其他处理器对视它们自己的cached的锁副本，这个每个正在自旋的线程几乎每次都会遇到一个cache确实，并且必须通过总线来获取的新的修改的值。而且比这个更糟糕的是当持有锁的线程视图释放锁时，由于总线被正在自旋的线程所独占，该线程有可能会被延迟。

**分析TTASLOck算法**：线程B第一次读取锁时候发生cache缺失，从而阻塞等待值被载入它的cache中。只要A持有锁，B就不断重复读该值，且每次都命中cache，这样B不产生总线流量，而且也不会降低其他线程的内存访问速度。此外释放锁的线程也不会被正在该锁上旋转的线程所延误。

然而，当锁被释放时情况却并不理想。锁的持有者将false值写入锁变量来释放锁，该操作将会使自旋线程的cache副本立刻失效。每个线程都将发生一次cache缺失并重读新值，它们都（几乎是同时）调用getAndSet()以获取锁。第一个成功的线程将使其他线程失效，这些失效线程接下来又重读那个值，从而引|起一场总线流量风暴。最终，所有线程再次平静，进入本地旋转。

**本地旋转指线程反复地重读被缓存的值而不是反复地使用总线**，这个概念是一个重要的原则，对设计高效的自旋锁非常关键。

## 第八章：管程和阻塞同步

管程是一种能将同步和数据结合在一起的结构化方法。与类将数据和方法封装为一个整体的概念相类似，管程将数据、方法和同步封装在一个模块包中。

可重入锁：持有这种锁的线程能够不用阻塞而重新获取它。

```java
class LockedQueue<T> {
    final Lock lock = new ReentrantLock();
    final Condition notFull = lock.newCondition();
    final Condition notEmpty = lock.newCondition();
    final T[] items;
    int tail, head, count;

    public LockedQueue(int capacity) {
        items = (T[])new Object[capacity];
    }

    public void enq(T x) {
        lock.lock();
        try {
            while (count == items.length)
                notFull.Wait();
            items[tail] =x;
            if (++tail == items.length)
                tail = 0;
            ++count;
            notEmpty.sinal();
        } finally {
            lock.unlock();
        }
    }

    public T deq() {
        lock.lock();
        try {
            while (count == 0)
                notEmpty.await();
            T x = items[head];
            if (++head == items.length)
                head = 0;
            --count;
            notFull.sinal();
            return x;
        } finally {
            lock.unlock();
        }
    }
}
```

LockedQueue类： 使用锁和条件的FIFO队列。其中有两个条件变量，一个用于检测队列为空的情形，而另外一个用于检测队列变为非满的情形。像这种将方法，互斥锁和条件对象组合在一起的整体称为管程。

### 唤醒丢失问题

正如锁本身容易产生死锁一样，条件变量Condition对象本身非常容易出现唤醒丢失问题，当发生唤醒丢失时，一个或者多个线程一直在等待，而没有意识到他们所等待的条件已变为true。

唤醒丢失问题代码，在这个实现中，不是采用每当enq()从队列中入队一个数据项时都给notEmpty条件产生一个信号的方式，而是采用了仅当队列实际上从空变为非空时才给条件发出信号的方式，这样做是否更加高效呢？如果只有一个生产者和一个消费者，那么这种优化能够产生预期的效果，但如果有多个生产者或多个消费者，这样的优化并不正确。

```java
public void enq(T x) {
    lock.lock();
    try {
        while (count == items.length)
            notFull.Wait();
        items[tail] =x;
        if (++tail == items.length)
            tail = 0;
        ++count;
        if (count == 1) { // Wrong!
            notEmpty.sinal();
        }
        
    } finally {
        lock.unlock();
    }
}
```

分析：消费者A和B都试图从一个空队列中出队元素，他们检测到队列为空，于是都在notEmpty条件上阻塞。生产者C将缓冲区中一个数据项入队，给notEmpty发送信号，唤醒了A。然而，在A获得锁之前，另外一个生产者把第二个数据项放入队列中，由于队列为非空，所以它不对notEmpty产生信号。于是A将获得锁，移走了数据项，而B却成为唤醒丢失的受害者，此时缓冲区中有一个等待消费的数据项，B却要永远地等待。

有一些简单的编程技术能够使唤醒丢失最小化：

- 总是通知所有等待条件的进程，而不是仅仅通知一个。
- 等待时制定一个超时时限。

唤醒丢失场景：

- 在wait之前就signal

### 虚假唤醒问题

```java
public T deq() {
    lock.lock();
    try {
        if (count == 0)
            notEmpty.await();
        T x = items[head];
        if (++head == items.length)
            head = 0;
        --count;
        notFull.sinal();
        return x;
    } finally {
        lock.unlock();
    }
}
```

### 读者-写者锁

许多共享对象都具有下述特性：大多数读者调用只返回对象的状态而不修改对象，只有少数写者才真正修改对象。

读者之前没有必要相互同步；它们对对象的并发访问完全是安全的，然而写者必须锁住读者和其他写者。读者-写者锁允许多个读者或单个写者并发地进入临界区。其接口如下：

```java
public interface ReadWriteLock {
    Lock readLock();
    Lock writeLock();
}
```

该接口产生两个锁对象：读锁和写锁。它们满足下面的安全特性：

- 当任一线程持有写锁或读锁时候，其他线程不能获得写锁。
- 当任一线程持有写锁时，其他线程不能获得读锁。
- 多个线程可以同时持有读锁。

#### 简单的读者-写者锁

```java
public class SimpleReadWriteLock implements ReadWriteLock {
    int readers;
    boolean writer;
    Lock lock;
    Condition condition;
    Lock readLock, writeLock;

    public SimpleReadWriteLock() {
        write = false;
        readers = 0;
        lock = new ReentrantLock();
        readLock = new ReadLock();
        writeLock = new WriteLock();
        condition = lock.newConditon();
    }

    public Lock readLock() {
        return readLock;
    }

    public Lock writeLock() {
        return writeLock;
    }
}
```

```java
class ReadLock implements Lock {
    public void lock() {
        lock.lock();
        try {
            while (writer) {
                condition.await();
            }
            readers++;
        } finally {
            lock.unlock();
        }
    }

    public void unlock() {
        lock.lock();
        try {
           readers--;
           if (readers == 0)
                condition.signalAll(); 
        } finally {
            lock.unlock();
        }
    }
}

protected class WriteLock implements Lock {
    public void lock() {
        lock.lock();
        try {
            while(readers > 0 || writer) {
                condition.await();
            }
            writer = true;
        } finally {
            lock.unlock();
        }
    }

    public void unlock() {
        lock.lock();
        try {
            writer = false;
            condition.signalAll();
        } finally {
            lock.unlock();
        }
    }
}
```

#### 公平的读者-写者锁

尽管SimpleReadriteLock算法是正确的，但并不是令人满意的。通常情况下，读者要比写者频繁得多，这样的话，写者有可能被一系列连续的读者锁在外面很长时间。下面的FifoReadlriteLock类描述了一种给写者赋子优先级的方法。该类能保证一旦一个写者调用了写锁的lock()方法，则不允许有更多的读者能获得读锁，直到该写者获取并释放了该写锁为止。由于不再让读者进入，持有读锁的读者最终都将结束，写者将获得写锁。

```java
public class FifoReadWriteLock implements ReadWriteLock {
    int readAcquires, readReleases;
    boolean writer;
    Lock lock;
    Condition condition;
    Lock readLock, writeLock;

    public FifoReadWriteLock() {
        readAcquires = readReleases = 0;
        writer = false;
        lock = new ReentranLock(true);
        conditon = lock.newCondition();
        readLock = new ReadLock();
        writeLock = new WriteLock();
    }

    public Lock readLock() {
        return readLock;
    }

    public Lock writeLock() {
        return writeLock;
    }
}

private class ReadLock implements Lock {
    public void lock() {
        lock.lock();
        try {
            while (writer) {
                condition.await();
            }
            readAcquires++;
        } finally {
            lock.unlock();
        }
    }

    public void unlock() {
        lock.lock();
        try {
            readReleases++;
            if (readAcquires == readReleases)
                condition.singalAll();
        } finally {
            lock.unlock();
        }
    }
}

private  class WriteLock implements Lock {
    public void lock() {
        lock.lock();
        try {
            while (writer) {
                condition.await();
            }
            writer = true;
            while (readAcquires != readReleases) {
                condition.await();
            }
        } finally {
            lock.unlock();
        }
    }

    public void unlock() {
        writer = false;
        condition.signalAll();
    }
}
```

readAcquires域记录了请求读锁的总次数，readReleases域记录了释放读锁的总次数。当这两个数量相等时，没有线程持有读锁。（当然，这里忽略了潜在的整数溢出和环绕问题。）该类有一个私有的lock域，该锁由读者持有一段较短的时间：它们获得锁，把readAcquires加1，然后释放锁。写者则从它们试图获得写锐直到释放写锁这段时间内都一直持有该锁。这种锁协议能保证一且一个写者获得lock，则新增的读者都不能将readAcquires加1，所以其他新增的读者不能获得读锁，最终当前持有读锁的所有读者都将释放它，从而让写者继续前进。

当最后一个读者释放它的锁时如何通知正在等待的写者呢？当一个写者试图获取写锁时，它获得了FifoReadMriteLock对象的lock。一个释放读锁的读者也获得了那个lock，如果所有读者已经释放了它们的锁，释放读锁的读者则调用相关条件的signal()方法。

### 可重入锁

对于普通的锁，一个试图重新获取它自己已持有的锁的线程将会使自己陷人死锁。当一个获取锁的方法嵌套调用另一个获取同一个锁的方法时，这种情形就会发生。

**如果一个锁能被同一个线程多次获得，则称该锁是可重入的。**

SimpleReentrantLock类实现通过不可重入锁来构造可重入锁。Owner域保存着最后一个获得锁的线程的ID，每当获取锁时将holdCount域加1，每当释放锁时将holdCount域减1。当holdCount为零时锁为空闲。由于对这两个域的操作都是原子的，所以需要一个内部的短期锁。Lock域是由lock(）和unlock()对域进行操作时所使用的锁，正在等待该锁变为空闲的线程则使用condition域。下面代码中内部lock域被初始化为SimpleLock类（幻影）的一个对象，该SimpleLock类被假定为不可重入的<sup>L6</sup>。

lock()方法获取内部锁<sup>L13</sup>。如果当前线程已经是锁的拥有者，那么它把保存的计数器加1并返回<sup>L15</sup>。否则，如果保存的计数器不为零，该锁则被另一个线程所持有，调用者将释放锁并等待，直到给条件发出信号为止<sup>L20</sup>。当调用者被唤醒时，它仍必须继续检查保存的计数器是否为零。当保存的计数器为零时，调用线程则使它自己成为拥有者并将保存的计数器设置为1。

unlock()方法获取内部锁<sup>L29</sup>。如果锁空闲或调用者不是拥有者，那么该方法产生一个异常<sup>L31</sup>。否则，它把保存的计数器减1。如果保存的计数器为零，那么锁是空闲的，于是调用者用信号通知条件来唤醒一个正在等待的线程。

```java
public class SimpleReentrantLock implements Lock {
    Lock lock;
    Condition condition;
    int owner, holdCount;
    public SimpleReentrantLock() {
        lock = new SimpleLock();
        condition = lock.newCondition();
        owner = 0;
        holdCount = 0;
    }
    public void lock() {
        int me = ThreadID.get();
        lock.lock();
        try {
            if (owner == me) {
                holdCount++;
                return;
            }
            while (holdCount != 0) {
                condition.await();
            }
            owner = me;
            holdCount = 1;
        } finally {
            lock.unlock();
        }
        public void unlock() {
            lock.lock();
            try {
                if (holdCount==0 || owner != ThreadID.get())
                    throw new IllegalMonitorStateException();
                holdCount--;
                if (holdCount == 0) {
                    condition.signal();
                }
            } finally {
                lock.unlock();
            }
        }

        public Condition newCondition() {
            throw new UnsupportedOperationException("Not supported yet.");
        }
    }
}
```

### 信号量

互斥锁能够保证在一个时刻只能有一个线程进入临界区。如果在临界区被占用期间任何一个线程想进入，那么该线程则阻塞，将自己挂起直到另一个线程通知它去重新尝试获得锁。Semaphore是互斥锁的一般化形式。每个Semaphore有一个容量，简记为c。一个Semaphore并不是每次只让一个线程进人临界区，而是让至多c个线程进入，其中容量c是在Semaphore被初始化时所确定的。信号量是最早的同步形式之一。

```java
public class Semaphore {
    final int capacity;
    int state;
    Lock lock;
    Condition condition;
    public Semaphore(int c) {
        capacity = c;
        state = 0;
        lock = new ReentrantLock();
        condition = lock.newCondition();
    }
    public void acquire() {
        lock.lock();
        try {
            while (state == capacity) {
                condition.await();
            }
            state++;
        } finally {
            lock.unlock();
        }
    }

    public void release() {
        lock.lock();
        try {
            state--;
            condition.singalAll();
        } finally {
            lock.unlock();
        }
    }
}
```