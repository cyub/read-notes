#include <iostream>
#include <string>
using namespace std;

int getInteger(const string& prompt);
void optimizeWelcomeProgram() {
  string name, response;
  int age;

  cout << "What is your name? ";
  getline(cin,
          name);  // 解决了姓名中间有空格情况时候，cin >> name只读取前一半的问题
  age = getInteger("What is your name? ");
  cout << "Hello " << name << " (age " << age << ")" << '\n';

  cout << "Do you want to try again? ";
  cin >> response;

  cout << "You said: " << response << endl;
}

int main() { optimizeWelcomeProgram(); }
