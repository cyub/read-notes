#include <iostream>
#include <thread>

using namespace std;

class backgroud_task {
 public:
  void operator()() const { std::cout << "background task\n"; };
};

int main() {
  backgroud_task f;
  thread my_thread(f);
  my_thread.join();

  // thread my_thread2(backgroud_task()); //
  // 注意此时传入的是函数声明，而不是函数对象
  //  my_thread2.join();

  thread my_thread2((backgroud_task()));
  my_thread2.join();

  thread my_thread3{backgroud_task()};
  my_thread3.join();

  class backgroud_task fn = backgroud_task();
  thread my_thread4(fn);
  my_thread4.join();

  // 使用闭包
  thread my_thread5([] { std::cout << "background task\n"; });
  my_thread5.join();
}