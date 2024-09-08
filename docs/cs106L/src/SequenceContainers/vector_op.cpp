#include <iostream>
#include <vector>

using namespace std;
int main() {
  vector<int> v{1};
  for (size_t i = 0; i < v.size(); i++) {  // 访问所有元素
    cout << v[i] << " ";                   // 输出：1
  }
  cout << endl;

  v.push_back(2);          // 将元素插入向量的末尾
  v.insert(v.begin(), 0);  // 将元素插入向量的头部

  for (int i : v) {
    cout << i << " ";  // 输出：0 1 2
  }
  cout << endl;

  cout << v.at(0) << endl;  // 访问索引为 0 的元素
  cout << v[5] << endl;     // []形式访问越界，会返回垃圾值
  try {
    cout << v.at(5) << endl;  // at 越界会抛出异常
  } catch (out_of_range &e) {
    cout << e.what() << " out of range" << endl;
  }

  v[1] = 100;
  cout << "v[1] = " << v[1] << endl;  // 输出：v[1] = 100
  v.pop_back();                       // 删除最后一个元素
  for (int i : v) {
    cout << i << " ";  // 输出：0 100
  }
  cout << endl;
  return 0;
}
