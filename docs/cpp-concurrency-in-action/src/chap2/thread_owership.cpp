#include <iostream>
#include <thread>

void some_function() { std::cout << "some function" << std::endl; }
void some_other_function() { std::cout << "some other function" << std::endl; }

int main() {
  std::thread t1(some_function);
  std::thread t2 = std::move(t1);  // t1所有权转移给了 t2
  t1 = std::thread(
      some_other_function);  // 上面的 t1
                             // 已经与线程没有关联了，所以可以重新与一个新线程进行关联

  std::thread t3;  // 默认构造方式创建了一个 thread，但未与任何执行线程进行关联

  t3 = std::move(t2);  // 将t2关联的线程所有权转移给 t3，因为 t2
                       // 是一个命名对象，这里面需要显示调用 std::move 进行移动

  t1 = std::move(t3);  // 由于 t1 已经关联了一个执行线程，此时再将 t3 转移到
                       // t1，此时 t1 会先调用std::teriminate()终止程序继续执行
  return 0;
}