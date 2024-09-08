#include <array>
#include <iostream>
#include <numeric>

using namespace std;

int main() {
  array<int, 5> numbers = {15, 3, 30, 20, 15};

  cout << "数组元素(排序前)：";
  for (auto const i : numbers) {
    cout << i << " ";
  }
  cout << endl;
  // 排序
  sort(numbers.begin(), numbers.end());
  cout << "数组元素(排序后)：";
  for (auto const i : numbers) {
    cout << i << " ";
  }
  cout << endl;

  // 搜索
  auto fund = find(numbers.begin(), numbers.end(), 30);
  if (fund != numbers.end()) {
    cout << "找到30" << endl;
  } else {
    cout << "未找到30" << endl;
  }

  cout << "数组元素(反转后)：";
  reverse(numbers.begin(), numbers.end());
  for (auto const i : numbers) {
    cout << i << " ";
  }
  cout << endl;

  cout << "数组元素(去重后)：";

  // 首先确保容器是排序的(不论正序还是反序)，因为 std::unique 要求元素是排序的
  auto last = unique(
      numbers.begin(),
      numbers
          .end());  // std::unique
                    // 并不保证保留哪个重复的元素，它只是简单地将重复的元素移到容器的末尾
  for (auto it = numbers.begin(); it != last; ++it) {
    // 由于 std::array是不能动态改变大小的，所以只能打印去重后的结果
    cout << *it << " ";
  }
  cout << endl;

  int sum = accumulate(numbers.begin(), numbers.end(), 0);
  cout << "数组元素和：" << sum << endl;

  return 0;
}