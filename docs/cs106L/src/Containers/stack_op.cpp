#include <iostream>
#include <stack>
using namespace std;

void printStack(stack<string> &s) {
  while (!s.empty()) {
    cout << s.top() << ", ";
    s.pop();
  }
}
int main() {
  stack<string> languages;
  languages.push("C++");
  languages.push("Java");
  languages.push("Python");
  cout << "Top element: " << languages.top() << endl;
  cout << "堆栈大小：" << languages.size() << endl;
  printStack(languages);
  return 0;
}