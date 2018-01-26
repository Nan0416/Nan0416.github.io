
#include <iostream>
using namespace std;
class Parent{
public:
    Parent();
    void display();
};
Parent::Parent(){
    cout << "Parent constructor" << endl;
}
void Parent::display(){
    cout << "Parent display" << endl;
}

//========
class Child: public Parent{
public:
    Child();
    void display(); // if child wants to override a function, it has to explicitly declare the function.
};
Child::Child(){
    cout << "Child Constructor" << endl;
}
void Child::display(){
    Parent::display(); // invoke parent's display
    // display();// this causes infinite recursion
    cout << "Child display "  << endl;
}
int main(){
    Child c; // Parent constructor /n Child constructor
    c.display(); // Parent display /n Child display
    return 0;
}
