#include <iostream>
#include <thread>

using namespace std;
void do_something(int i) {
  ++i;
  cout << i << endl;
};

struct func {
  int &i;
  func(int &i_) : i(i_) {}
  void operator()() {
    for (unsigned j = 0; j < 100; ++j) {
      do_something(i);  // 潜在访问隐患：悬空引用
    }
  }
};

void oops() {
  int some_local_state = 0;
  func my_func(some_local_state);
  thread t(my_func);
  t.detach();  // 不等待新线程运行结束，当 oops
               // 结束后，新线程还会访问到局部变量some_local_state
}

int main() {
  oops();
  this_thread::sleep_for(std::chrono::milliseconds(500));  // 休眠 500 毫秒
}