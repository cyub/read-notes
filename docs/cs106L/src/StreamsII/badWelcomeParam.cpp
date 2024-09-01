#include <iostream>
#include <string>
using namespace std;

void badWelcomeProgram() {
  string name, response;
  int age;

  cout << "What is your name? ";  // sidenote: no flush needed! why?
  cin >> name;

  cout << "What is your age? ";
  cin >> age;

  cout << "Hello " << name << " (age " << age << ")" << '\n';

#ifdef CLEAN_CIN_STATE
  cin.clear();
#endif
  cout << "Do you want to try again? ";
  cin >> response;

  cout << "You said: " << response << endl;
}

int main() { badWelcomeProgram(); }
