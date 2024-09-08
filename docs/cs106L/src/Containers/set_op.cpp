#include <iostream>
#include <set>

using namespace std;
int main() {
  set<int> s = {1, 2, 3, 3, 4, 5};
  for (int v : s) {
    cout << v << " ";  // 输出：1 2 3 4 5
  }
  cout << endl;

  s.insert(6);  // 插入一个元素6

  if (s.count(6)) {  // 查询6是否在集合中
    cout << "6 is in the set" << endl;
  } else {
    cout << "6 not in set" << endl;
  }

  s.erase(5);  // 删除5
  for (int v : s) {
    cout << v << " ";  // 输出：1 2 3 4 6
  }
  cout << endl;
  return 0;
}