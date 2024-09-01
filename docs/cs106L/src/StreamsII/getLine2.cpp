#include <iostream>
#include <sstream>

using namespace std;
int main() {
  istringstream iss("  16.9\n 24");
  double amount;
  iss >> amount;  // amount = 16.9，此时内布置指针指向\n的位置
  iss.ignore();  // 忽略掉一个字符串，好跳过\n。或者使用getline(iss, line)
                 // 进行跳过
  string line;
  getline(iss, line);  // line =" 24"
  cout << "acount=" << amount << "\tline=" << line << endl;
  return 0;
}
