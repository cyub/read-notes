#include <iostream>

struct func {
  int &i;
  func(int &i_) : i(i_) {};
};