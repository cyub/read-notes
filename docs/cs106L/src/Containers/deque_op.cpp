#include <deque>
#include <iostream>

using namespace std;

void printDeque(deque<int> &dq) {
  for (auto it = dq.begin(); it != dq.end(); ++it) {
    cout << *it << " ";
  }
  cout << endl;
}
int main() {
  deque<int> dq{1, 2, 3, 4, 5};
  printDeque(dq);  // 输出：1 2 3 4 5

  cout << "第一个元素: " << dq.front() << endl;
  cout << "最后一个元素: " << dq.back() << endl;
  cout << "索引 0 处的元素: " << dq.at(0) << endl;  // 越界会抛出异常
  cout << "索引 0 处的元素: " << dq[0] << endl;     // 越界会返回垃圾值

  dq.at(2) *= dq.at(2);  // 更新索引 2 处的元素
  printDeque(dq);        // 输出：1 2 9 4 5

  dq.push_back(6);   // 尾部添加元素
  dq.push_front(0);  // 头部添加元素
  printDeque(dq);    // 输出：0 1 2 9 4 5 6

  deque<int>::iterator it = dq.begin();
  cout << "第二个元素：" << *(++it) << endl;

  it = dq.end() - 1;
  cout << "最后一个元素：" << *(it) << endl;
  return 0;
}