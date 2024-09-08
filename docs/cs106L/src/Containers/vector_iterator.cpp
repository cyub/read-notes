#include <iostream>
#include <vector>

using namespace std;
int main() {
  vector<int> v = {1, 2, 3, 4, 5};

  for (auto it = v.begin(); it != v.end(); ++it) {
    std::cout << *it << " ";  // 通过迭代去访问向量元素 输出：1 2 3 4 5
  }
  std::cout << std::endl;

  vector<int>::iterator iter;

  iter = v.begin();                    // 访问向量的第一个元素
  cout << "v[0] = " << *iter << endl;  // 输出： 1
  iter = v.end() - 1;                  // 访问向量的最后一个元素

  cout << "v[" << v.size() - 1 << "] = " << *iter
       << endl;  // 输出： 5，等同于 v.back()

  for (auto it = v.begin(); it != v.end(); ++it) {
    *it = *it * *it;
  }

  for (auto it = v.cbegin(); it != v.cend(); ++it) {  // const_iterator
    std::cout << *it << " ";                          // 输出：1 4 9 16 25
  }
  std::cout << endl;
  return 0;
}