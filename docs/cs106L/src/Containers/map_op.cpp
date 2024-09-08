#include <iostream>
#include <map>

using namespace std;
int main() {
  std::map<int, string> student;  // 创建一个map对象

  student[1] = "Jacqueline";  // 插入键值对
  student[2] = "Blake";

  student.insert(std::make_pair(3, "Denise"));  // 以 pair 形式插入键值对

  //  使用[] 访问元素
  for (size_t i = 1; i <= student.size(); ++i) {
    cout << "Student[" << i << "]: " << student[i] << endl;
  }

  // 使用迭代器遍历map
  for (auto iter = student.begin(); iter != student.end(); ++iter) {
    cout << "Student[" << iter->first << "]: " << iter->second << endl;
  }

  // 访问 key=1 的元素
  cout << "Student[1]: " << student.at(1) << endl;  // 如果不存在会抛出异常
  cout << "Student[1]: " << student[1] << endl;  // 如果不存在会返回垃圾值
  auto iter = student.find(1);
  cout << "Student[1]: " << iter->second << endl;

  student.erase(1);  // 删除 key=1 的元素
  for (size_t i = 1; i <= student.size(); ++i) {
    cout << "Student[" << i << "]: " << student[i] << endl;
  }

  for (iter = student.begin(); iter != student.end(); ++iter) {
    cout << iter->second << ", ";
    // 我们也可以通过解引用来访问元素, 如下
    // cout << (*iter).second << endl;
  }

  return 0;
}