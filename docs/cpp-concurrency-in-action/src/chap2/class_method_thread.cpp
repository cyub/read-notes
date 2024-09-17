#include <iostream>
#include <thread>

class X {
 public:
  void do_lengthy_work(int);
};

void X::do_lengthy_work(int num) {
  std::cout << "do_lengthy_work: " << num << std::endl;
}
int main() {
  X my_x;
  int num(0);
  std::thread t(&X::do_lengthy_work, &my_x, num);

  my_x.do_lengthy_work(10);
  t.join();
}