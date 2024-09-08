#include <iostream>
using namespace std;

class Base {
 public:
  // base class print function
  virtual void print() { cout << "Base class" << endl; }
};

class Derived : public Base {
 public:
  // derived class print function overriding the base class
  void print() override { cout << "Derived class" << endl; }
};

int main() {
  // create a Base class pointer
  // pointing to a Derived object
  Base *base_ptr = new Derived();

  // use dynamic_cast to cast the
  // Base pointer to a Derived pointer
  Derived *derived_ptr = dynamic_cast<Derived *>(base_ptr);

  // call the print function through the derived pointer
  if (derived_ptr) {
    derived_ptr->print();
  }

  // delete the dynamically allocated object
  // delete base_ptr;
  return 0;
}