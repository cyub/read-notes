#include <array>
#include <iostream>

using namespace std;

int main() {
  array<int, 5> numbers = {1, 2, 3, 4, 5};
  for (const int num : numbers) {
    cout << num << " ";  // 输出：1 2 3 4 5
  }
  cout << endl;

  cout << "numbers[0] = " << numbers[0] << endl;  // 输出：1
  cout << "numbers[" << numbers.size() - 1 << "] = " << numbers.back()
       << endl;                              // 输出：5
  cout << "numbers[100] = " << numbers[100]  // 输出结果为止
       << endl;  // []访问时候不会进行越界检查
  try {
    cout << "numbers[100] = " << numbers.at(100)  // 会抛出异常
         << endl;  // at 访问时候会进行越界检查
  } catch (out_of_range& e) {
    cout << "Exception: " << e.what() << endl;
  }

  numbers[0] += 10;      // 修改数组元素
  numbers.at(0) += 100;  // 修改数组元素，会进行越界检查
  cout << "numbers[0] = " << numbers[0] << endl;  // 输出：111

  numbers.fill(500);  // 使用 500填充数组
  for (const int num : numbers) {
    cout << num << " ";  // 输出：500 500 500 500 500
  }
  cout << endl;
  return 0;
}