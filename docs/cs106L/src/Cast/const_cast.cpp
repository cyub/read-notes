#include <iostream>
using namespace std;

// function that takes a non-const pointer
void modify_data(int* data) {
  // modify the data
  *data *= 2;
}

int main() {
  int x = 10;

  // a const pointer for variable x
  const int* ptr = &x;

  // use const_cast to
  // remove const qualifier and allow modification
  int* mutable_ptr = const_cast<int*>(ptr);

  // call the function
  modify_data(mutable_ptr);

  // value is modified successfully
  cout << "Modified value: " << x << endl;
  return 0;
}