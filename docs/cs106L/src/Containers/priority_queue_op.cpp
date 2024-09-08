#include <iostream>
#include <queue>

using namespace std;
int main() {
  priority_queue<int> pq;
  pq.push(1);  // 添加元素
  pq.push(10);
  pq.push(100);

  cout << "队列大小：" << pq.size() << endl;
  cout << "队列是否为空：" << pq.empty() << endl;
  while (
      !pq.empty()) {  // 打印其顶部，然后在循环内重复弹出该元素，直到队列为空。
    cout << pq.top() << endl;  // 输出最大元素
    pq.pop();                  // 删除最大元素
  }

  cout << "队列大小：" << pq.size() << endl;
  cout << "队列是否为空：" << pq.empty() << endl;
  return 0;
}