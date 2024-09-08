#include <iostream>
#include <queue>

using namespace std;

int main() {
  queue<string> animals;
  animals.push("Dog");  // 添加元素
  animals.push("Cat");

  cout << "队列内容：";
  while (!animals.empty()) {
    cout << animals.front() << ", ";  // 访问第一元素
    animals.pop();                    // 删除元素
  }
  cout << endl;

  cout << "队列大小：" << animals.size() << endl;  // 队列大小
  animals.push("Rabbit");
  animals.push("Lion");
  cout << "队列中最后一个元素：" << animals.back() << endl;  // 访问最后一个元素
  cout << "队列大小：" << animals.size() << endl;
  return 0;
}