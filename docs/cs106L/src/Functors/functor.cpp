#include <iostream>
using namespace std;

class Greet {
 public:
  // overload function call/parentheses operator
  void operator()() { cout << "Hello World!\n"; }
};

int main() {
  // create an object of Greet class
  Greet greet;

  // call the object as a function
  greet();
  return 0;
}