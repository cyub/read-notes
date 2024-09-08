#include <forward_list>
#include <iostream>

using namespace std;

void printforwardList(forward_list<int>& fwd_list) {
  for (const int& element : fwd_list) {
    cout << element << " ";
  }
  cout << endl;
}
int main() {
  forward_list<int> flist = {1, 2, 3, 4, 5};
  printforwardList(flist);

  cout << "第一个元素：" << flist.front() << endl;
  flist.push_front(6);
  printforwardList(flist);  // 输出：6 1 2 3 4 5

  auto itr = flist.begin();
  for (; itr != flist.end(); itr++) {
    cout << *itr << " ";
  }
  cout << endl;

  itr = flist.begin();
  advance(itr, 3);             // 跳过前3个元素
  flist.insert_after(itr, 7);  // 在第四个元素后面插入元素 7
  printforwardList(flist);

  flist.assign({3, 1, 2, 3});  // 赋值替换
  printforwardList(flist);     // 输出：1 2 3

  flist.remove(3);  // 删除所有值为3的元素
  printforwardList(flist);

  flist.clear();            // 清空列表
  printforwardList(flist);  // 输出：
  return 0;
}