# cs106L: C++标准库编程

## 概要

斯坦福公开课：**cs106L: C++标准库编程(2020)**，对应课程地址：https://web.stanford.edu/class/archive/cs/cs106l/cs106l.1204/index.html。

配套在线观看地址：https://www.bilibili.com/video/BV19x4y1E79V/?vd_source=f3e532bab07bd39d7aa9e7d412b650c5。

## 简介 Intro

### CS106L课程目标 Goals of CS 106L

1. Learn what features are out there in C++ and why they exist. 了解 C++ 中有哪些功能以及它们存在的原因。
2. Become comfortable with reading C++ documentation. 习惯阅读 C++ 文档。
3. Become familiar with the design philosophy of modern C++. 熟悉现代C++的设计哲学。

![](./images/cs106l_landscape.png)

### C++语言历史 History of C++

从`Hello, world`说起。

=== "C++ 版本"
    ```cpp
    #include <iostream>

    int hello_cpp() {
        std::cout << "Hello, world!" << std::endl;
        return 0;
    }

    int main() {
        hello_cpp();
    }
    ```
=== "C 版本"
    ```c
    #include "stdio.h"
    #include "stdlib.h"

    int hello_c() {
        printf("%s", "Hello, world!\n");
        return EXIT_SUCCESS;
    }

    int main() {
        hello_c();
    }
    ```
=== "汇编版本(C语言嵌套汇编)"

    ```asm

    #include "stdio.h"
    #include "stdlib.h"

    /*
    * Wrapper function for convenience
    */
    void myputs(char *s) {
        puts(s);
    }

    /*
    * This will probably not work on your computer.
    * Assembly is not at all portable; a good
    * reason to avoid using it!
    *
    * Those of you who have taken 107 should
    * be able to somewhat see what is happening here.
    */
    int hello_as() {
        /* The assembly literally writes the hex representation
        * of as big a portion of the string as it can into the addresses
        * at range rsp to rsp + 0xd. That range is exactly 12 bytes long
        * as there are 12 characters in the "Hello, wordl!" string.
        */
        asm("sub    $0x20,%rsp\n\t"
            "movabs $0x77202c6f6c6c6548,%rax\n\t"   // moves "Hello, w" portion to mem at $rsp
            "mov    %rax,(%rsp)\n\t"
            "movl   $0x646c726f, 0x8(%rsp)\n\t"     // moves "orld"  portion to mem at $rsp + 8
            "movw   $0x21, 0xc(%rsp)\n\t"           // moves "!" portion to mem at $rsp + 12
            "movb   $0x0,0xd(%rsp)\n\t"             // moves string null terminator to mem at $rsp + 13
            "leaq    (%rsp),%rax\n\t"               // loads address of $rsp as first argument to puts
            "mov    %rax,%rdi\n\t"
            "call  __Z6myputsPc\n\t"                // calls puts
            "add    $0x20, %rsp\n\t"
        );
        return EXIT_SUCCESS;
    }

    int main() {
        hello_as();
    }
    ```
=== "纯汇编版本"

    ```asm
    section .text
    global _start ;must be declared for linker (ld)

    _start: ;tell linker entry point
        mov edx,len ;message length
        mov ecx,msg ;message to write
        mov ebx,1 ;file descriptor (stdout)
        mov eax,4 ;system call number (sys_write)
        int 0x80 ;call kernel
        mov eax,1 ;system call number (sys_exit)
        int 0x80 ;call kernel

    section .data
    msg db 'Hello, world!',0xa ;our dear string
    len equ $ - msg ;length of our dear string
    ```

### 汇编

好处(Benefits)：

- Unbelievably simple instructions 指令简单的令人难以置信
- Extremely fast (when well-written) 如果写得好的话，速度极快
- Complete control over your program 你可以完全控制你的程序

缺点：

- Requires lots of code to do simple tasks 需要大量代码来完成简单的任务
- Hard to understand other people’s code 很难理解别人的代码
- Extremely unportable 极其不具有可移植性

### C语言的诞生(Invention of C )

Writing assembly was too difficult but computers only
understood assembly.

编写汇编太难了，只有计算机才能了解汇编。于是C语言诞生了:

- 可以用更直观的方式编写源代码
- 通过可以将其转换为汇编语言

K&R(**K** en Thompson(肯·汤普森) and Dennis **R** itchie(丹尼斯·里奇))在 1972 年创建了 C，C 使编写代码变得容易: 

- 快速 Fast
- 简单 Simple
- 跨平台 Cross-platform

C语言缺点(weakness)：

- 无对象或类 No objects or classes
- 很难编写通用的代码 Difficult to write code that worked generically
- 编写大型程序时很乏味 Tedious when writing large programs

### C++语言的诞生

1983 年，C++ 的第一个遗迹由比亚恩·斯特鲁斯特鲁普(Bjarne Stroustrup)创造。他想要一种语言：

- 快速 Fast
- 简单易用 Simple to Use
- 跨平台 Cross-platform
- 具有高级功能 Had high level features

### C++的设计哲学 Design Philosophy of C++

- Allow the programmer full control, responsibility, and
choice if they want it. 允许程序员完全控制、负责和如果他们想要的话，可以选择。
- Express ideas and intent directly in code. 直接在代码中表达想法和意图。

    以下面求整数向量的和为例:

    === "方式1"
        ```c++
        vector<int> vec = {1, 2, 3};
        int sum = 0;
        for (auto val : vec) {
            sum += val;
        }
        ```
    === "方式2"
        ``` cpp
        vector<int> vec = {1, 2, 3};
        int sum = 0;
        for (const auto& val : vec) { // 相比方式1，表明只是访问向量的值
            sum += val;
        }
        ```

    === "方式3"
        ```cpp
        vector<int> vec = {1, 2, 3};
        int sum = std::accumulate(vec.begin(), vec.end(), 0); // 相比方式2，语义性更强，
        // 一眼可以看出来是向量求和
        ```
- Enforce safety at compile time whenever possible. 尽可能在编译时强制执行安全性。
- Do not waste time or space. 不浪费时间或空间。
- Compartmentalize messy constructs. 划分混乱的结构。

## 流 Streams

![io流类继承关系图](./images/ioclasses.gif)

### 字符串流 stringstream

#### 字符串流 stringstream 构造函数

```cpp
istringstream iss(“Initial”);
ostringstream oss(“Initial”);

istringstream iss(“Initial”, stringstream::bin); // 以二进制binary形式打开
ostringstream oss(“Initial”, stringstream::ate); // ate: start at end 打开之后，立刻移动到流的末尾
```

#### 字符串流格式的 I/O stringstream formatted i/o

```cpp
oss << var1 << var2; // 依次将var1，var2写入到oss的buffer中

iss >> var1 >> var2; // 将iss的buffer数据抽取到var1
```

#### 字符串流根据空白字符分割token是怎么回事？

**示例1**：

![](./images/stringstream_token.png)

```cpp
#include <iostream>
#include <sstream>

using namespace std;

int main() {
  istringstream iss("16.9 \n Ounces. . \t \n \n -38271");
  string token1, token2, token3, token4;
  iss >> token1 >> token2 >> token3 >> token4;
  cout << "token1: " << token1 << "\n";
  cout << "token2: " << token2 << "\n";
  cout << "token3: " << token3 << "\n";
  cout << "token4: " << token4 << endl;
}
```

上面代码输出以下内容：

```
token1: 16.9
token2: Ounces.
token3: .
token4: -38271
```

**示例2**：

流会在遇到任何空白处停止读取或该类型的任何无效字符处停止读取。

![](./images/stringstream_token2.png)

```cpp
#include <iostream>
#include <sstream>

using namespace std;

int main() {
  istringstream iss("16.9 \n Ounces. . \t \n \n -38271");
  int token1;
  string token2;
  char token3;
  bool token4;
  iss >> token1 >> token2 >> token3 >> token4;
  cout << "token1: " << token1 << "\n";
  cout << "token2: " << token2 << "\n";
  cout << "token3: " << token3 << "\n";
  cout << "token4: " << token4 << endl;
}
```

上面代码输出以下内容：

```
token1: 16
token2: .9
token3: O
token4: 0
```

**示例3**：

=== "ostringstream示例"
    ```cpp
    #include <iostream>
    #include <sstream>

    using namespace std;

    int main() {
    ostringstream oss("hello");
    cout << "oss buffer pos: " << oss.tellp() << endl;  // 输出oss buffer pos: 0
    cout << oss.str() << endl;
    ;  // 输出hello，str()以字符串形式返回oss的buffer

    oss << "hi";
    cout << oss.str() << endl;  // 输出hillo，oss的buffer初始指针指向起始位置，
                                // 所以写入时候会覆盖之前数据
    cout << "oss buffer pos: " << oss.tellp() << endl;  // oss buffer pos: 2
    oss << " world";
    cout << oss.str() << endl;  // 输出hi world

    ostringstream oss2("hello", ostringstream::ate);
    cout << "oss2 buffer pos: " << oss2.tellp()
        << endl;  // 输出oss2 buffer pos: 5
    cout << oss2.str() << endl;
    ;  // 输出hello

    oss2 << "hi";
    cout << oss2.str() << endl;  // 输出hellohi
    }
    ```
=== "istringstream示例"

    ```cpp
    #include <iostream>
    #include <sstream>

    using namespace std;

    int main() {
    istringstream iss("16.9 Ounces");
    double amount;
    string unit;

    iss >> amount >> unit;

    cout << "amount: " << amount << endl; // 输出amount: 16.9
    cout << "unit: " << unit << endl; // 输出unit: Ounces
    }
    ```

#### 字符串流的位置相关函数

```cpp
// 获取位置
oss.tellp();
iss.tellg();

// 设置位置
oss.seekp(pos);
iss.seekg(pos);
```

- ostringstream中的tellp()中的 **p**，来自"put position",表示输出位置，tellp()返回当前输出位置的偏移量，即从字符串流的开始到当前输出位置的字符数。这个位置用于跟踪你在字符串中写入了多少字符。
- istringsteam中的tellg()中的 **g**，来自 "get position"，表示输入位置。tellg() 返回当前输入位置的偏移量，即从字符串流的开始到当前输入位置的字符数。这个位置用于跟踪你在字符串中读取了多少字符。

#### 要点 Key Takeaways

- `>>` 运算符用于提取特定类型的下一个变量，直到遇到下一个空白。
- `>>` 和 `<<` 运算符返回对流的引用本身，因此在每个实例中流都是左侧操作数。
- 对字符串流。读写同时往往会导致微妙的错误，要小心！

### 状态位

C++的流中有四个状态位(state bits)，用来标志流的状态：

![](./images/four_bit_in_stream.png)

- Good bit: 表示准备好了，可以进行读写操作
- Fail bit: 表示之前的操作失败了，后面的所有操作都会冻结（都会操作失败），一般是由于输入数据的格式不正确或者违反了预期的逻辑
- EOF bit: 表示之前的操作已经达到了buffer的末尾，没有更多数据可读了。
- Bad bit: 表示发生流其他错了，意味着流遇到了严重错误，比如内存分配失败或者底层系统错误

#### 状态位相关成员函数

- bool ios::bad():

    当bad bit设置时候，返回true，否则返回false

- bool ios::eof():

    当检测到文件末尾(eof)时候，会设置eof bit，此时会返回true，否则返回false

- bool ios::fail():

    当bad()返回true或者设置fail bit设置时候，返回true，否则返回false

- bool ios::good():

    当没有设置bad bit, eof bit, fail bit时候，返回true，否则返回false

- void iso::clear():

    用于当流发生可以修复的错误时候，清除错误状态。clear(int state)是其重载版本，其先清除状态，然后设置状态state

- ios::iostate ios::rdstate():

    返回当前流对象当前的标志位值。我们可以通过按位与测试特定标志位

- void ios::setstate(ios::iostate state):

    用来设置标志位


**示例1：**

构造一个ostream对象，但没有提供streambuf对象，所以这个streambuf永远不会正常工作，所以它bad bit一开始就被置位。

```cpp
std::ostream error(0);
std::cout << std::boolalpha << error.bad() << std::endl; // 输出true
```

**示例2：**

当从cin中读取一行文本时候，如果该行文本不是由"\n"字符结尾，那么getline会达到文件末尾，从而触发eof bit，此时cin.eof()会返回true。

```cpp
getline(cin, str);
cout << cin.eof();
```

编译上面程序，生成二进制可执行文件`program`，然后执行下面命令:

```cpp
echo "hello, world" | ./program // 输出0
echo -n "hello, world" | ./program // 输出1
```

**示例3：**

```cpp
#include <iostream>
#include <string>

using namespace std;

void state() {
  cout << "\n"
          "Bad: "
       << cin.bad()
       << " "
          "Fail: "
       << cin.fail()
       << " "
          "Eof: "
       << cin.eof()
       << " "
          "Good: "
       << cin.good() << '\n';
}

int main() {
  string line;
  int x;

  cin >> x;
  state();

  cin.clear();
  getline(cin, line);
  state();

  getline(cin, line);
  state();
}
```

编译上面程序，生成二进制可执行文件`program`，然后执行下面命令:

```shell
echo -n "hello\nworld" | ./program
```

上面命令将输出一下内容：

```
Bad: 0 Fail: 1 Eof: 0 Good: 0

Bad: 0 Fail: 0 Eof: 0 Good: 1

Bad: 0 Fail: 0 Eof: 1 Good: 0
```

**示例4：**

通过`rdstate()`进行特定标志位测试：

```cpp
if (!(iosObject.rdstate() & ios::failbit))
{
    // last operation didn't fail
}

if (iosObject.rdstate() == ios::goodbit) // 由于goodbit值是0，所以不能用与运算判断，只能进行等值判断
{
    // state is `good'
}
```

#### 将流转换成bool值

流可以用在需要逻辑值的表达式中：

```cpp
if (cin)                // cin itself interpreted as bool
if (cin >> x)           // cin interpreted as bool after an extraction
if (getline(cin, str))  // getline returning cin
```
**当将流解释为逻辑值时候，实际上会被解释成`not fail()`**，因此上面例子等价于：

```cpp
if (not cin.fail())
if (not (cin >> x).fail())
if (not getline(cin, str).fail())
```





#### 字符串转整数演示位状态判断

=== "未进行任何检查版本"

    ```cpp
    #include <iostream>
    #include <sstream>
    using namespace std;

    int stringToInteger(const string& str);
    int main() {
            cout << stringToInteger("123") << endl; // 输出123
            cout << stringToInteger("123a") << endl; // 输出123
            cout << stringToInteger("a123") << endl; // 输出0
    }

    int stringToInteger(const string& str) {
            int result;
            istringstream iss(str);
            iss >> result; // 一直读取数字字符串，直到遇到非数字的字符串
            return result;
    }
    ```
=== "带检查的版本1"

    ```cpp
    #include <iostream>
    #include <sstream>
    #include <stdexcept>
    using namespace std;

    int stringToInteger(const string& str);
    int main() {
            cout << stringToInteger("123") << endl; // 输出123
            cout << stringToInteger("123a") << endl; // 输出123
            cout << stringToInteger("a123") << endl; // 抛出异常
    }


    int stringToInteger(const string& str) {
            int result;
            istringstream iss(str);
            iss >> result;
            if (iss.fail()) throw domain_error("invalid string");
            return result;
    }
    ```

    可以看到它能检查一开始不是数字的字符串，但对于`123a`这种后面包含非数字的字符串，无能为力。

=== "带检查的版本2"

    ```cpp
    #include <iostream>
    #include <sstream>
    #include <stdexcept>

    using namespace std;

    int stringToInteger(const string& str);
    int main() {
            cout << stringToInteger("123") << endl;
            cout << stringToInteger("123a") << endl;
            cout << stringToInteger("a123") << endl;
    }

    int stringToInteger(const string& str) {
            int result;
            istringstream iss(str);
            iss >> result;
            if (iss.fail()) throw domain_error("invalid string");

            char remain;
            iss >> remain; // 判断后面还有没有字符可读，如果有则说明字符串非法
            if (!iss.fail()) throw domain_error("invalid string");
            return result;
    }
    ```

    其中 `if(!iss.fail())` 等效于 `if((iss>>remain))`

=== "带检查的版本3：使用短路求值(short circuiting)优化代码"

    ```cpp
    #include <iostream>
    #include <sstream>
    #include <stdexcept>

    using namespace std;

    int stringToInteger(const string& str);
    int main() {
            cout << stringToInteger("123") << endl;
            cout << stringToInteger("123a") << endl;
            cout << stringToInteger("a123") << endl;
    }

    int stringToInteger(const string& str) {
            int result;
            istringstream iss(str);
            char remain;
            if (!(iss>>result) || iss>>remain) throw domain_error("invalid string");

            return result;
    }
    ```

### 格式化输出

#### 每次都endl，还是每次都\n，然后最后flush?

```cpp
// g++ -std=c++11 example.cpp
// 下面代码中默认用到auto，是c++11开始支持的特性，需要指定c++版本不低于c++11，这里使用c++11
#include <chrono>
#include <iostream>
#include <sstream>
using namespace std;

int endlEachTime();
int endlAtEnd();

int main() {
  int a = endlEachTime();
  int b = endlAtEnd();
  cout << "endl each time: " << a << endl;
  cout << "endl at end: " << b << endl;
}

// Prints the first 100 integers including an endl at the end of each line
// Returns the duration it took.
// Optional topic covered at end of lecture.
int endlEachTime() {
  auto startMove =
      std::chrono::high_resolution_clock::now();  // records start time

  for (int i = 0; i < 10000; ++i) {
    cout << i << endl;
  }

  auto stopMove =
      std::chrono::high_resolution_clock::now();  // records end time
  // don't worry too much about this syntax, you can google it easily
  // in fact, that's exactly what I did
  auto moveDuration = std::chrono::duration_cast<std::chrono::microseconds>(
      stopMove - startMove);
  return moveDuration.count();
}

// Prints the first 100 integers including only a newline character after each
// line Returns the duration it took. Optional topic covered at end of lecture
int endlAtEnd() {
  auto startMove = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < 10000; ++i) {
    cout << i << '\n';
  }
  cout << flush;

  auto stopMove = std::chrono::high_resolution_clock::now();
  auto moveDuration = std::chrono::duration_cast<std::chrono::microseconds>(
      stopMove - startMove);
  return moveDuration.count();
}
```

通过测试可以发现第二个性能会更好。