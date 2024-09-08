#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  // initialize vector of int
  vector<int> nums = {1, 20, 3, 89, 2};

  // sort the vector in descending order
  sort(nums.begin(), nums.end(), greater<int>());

  for (auto num : nums) {
    cout << num << ", ";
  }
  cout << endl;
  return 0;
}