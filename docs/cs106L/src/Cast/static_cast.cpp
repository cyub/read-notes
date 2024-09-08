#include <iostream>
using namespace std;

int main() {
  float my_float = 3.14;

  // convert float to int
  int my_int = static_cast<int>(my_float);
  cout << "Float: " << my_float << " -> Int: " << my_int << endl;
  return 0;
}