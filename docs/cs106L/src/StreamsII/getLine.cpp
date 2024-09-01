#include <iostream>
#include <sstream>

using namespace std;
int main() {
  istringstream iss("16.9\n 24");
  double amount;
  iss >> amount;  // amount = 16.9，此时内布置指针指向\n的位置
  string line;
  getline(iss,
          line);  // line =""
  // getline读取时候会一直读取到\n位置(会消耗掉\n，但返回内容里面会把\n去掉)或者遇到eof。
  cout << "acount=" << amount << "\tline=" << line << endl;
  return 0;
}
