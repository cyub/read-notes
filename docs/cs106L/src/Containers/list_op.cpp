#include <iostream>
#include <list>

using namespace std;

void printList(list<int> &numbers) {
  for (int n : numbers) {
    cout << n << " ";
  }
  cout << endl;
}
int main() {
  list<int> numbers{1, 2, 3, 4, 5};

  printList(numbers);  // 输出内容为：1 2 3 4 5

  numbers.push_front(
      0);  //  在列表开始地方追加一个元素，此时列表内容为： 0 1 2 3 4 5
  numbers.push_back(
      6);  // 在列表末尾追加一个元素，此时列表内容为：0 1 2 3 4 5 6
  cout << "Size: " << numbers.size() << endl;  // 7

  cout << "第一个元素：" << numbers.front() << endl;
  cout << "最后一个元素：" << numbers.back() << endl;

  numbers.pop_front();  // 删除第一个元素，此时列表内容为：1 2 3 4 5 6
  numbers.pop_back();  // 删除最后一个元素，此时列表内容为：1 2 3 4 5

  numbers.reverse();  // 反转列表，此时列表内容为：5 4 3 2 1
  printList(numbers);

  numbers.sort();  // 排序列表，此时列表内容为：1 2 3 4 5
  printList(numbers);

  list<int>::iterator iter = numbers.begin();
  cout << "第一个元素：" << *iter << endl;  // 输出第一个元素
  cout << "最后一个元素：" << *--numbers.end() << endl;

  for (int &i : numbers) {
    i *= i;
  }
  printList(numbers);  // 输出：1 4 9 16 25
  return 0;
}
