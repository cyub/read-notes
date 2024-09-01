#include <iostream>
#include <sstream>

using namespace std;

int main() {
  string str = "hello, world";
  /*
  for (int i = 0; i < str.size(); i++) {    //
  i应该是size_t，无符号类型，否则会报错 cout << str[i] << endl;
  }
  */

  for (size_t i = 0; i < str.size(); i++) {
    cout << str[i] << endl;
  }
  return 0;
}
