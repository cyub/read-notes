#include <iostream>
using namespace std;

int main() {
  // create an integer variable
  int x = 67;

  // pointer to an integer
  int* ptr_to_int = &x;

  // reinterpret the pointer to an integer
  // as a pointer to char
  char* ptr_to_char = reinterpret_cast<char*>(ptr_to_int);
  // dereference the double pointer
  // originally holding an integer as if it contains a double
  cout << "Dereferencing ptr_to_char: " << *ptr_to_char << endl;

  return 0;
}