#include <iostream>
using namespace std;
class Parent{
public:
    Parent();
    void display();
private:
    virtual void virtualDisplay();
};
Parent::Parent(){
    cout << "Parent constructor" << endl;
}
void Parent::display(){
    // do something
    virtualDisplay();
    // do something
}
void Parent::virtualDisplay(){
    cout << "Parent virutal display" << endl;
}
//========
class Child: public Parent{
public:
    Child();
protected:
    void virtualDisplay();
};
Child::Child(){
    cout << "Child Constructor" << endl;
}

void Child::virtualDisplay(){
    cout << "Child virtual display "  << endl;
}
void callByReference(Parent & p){
    p.display();
}
void callByValue(Parent p){
    p.display();
}
int main(){
    Child c; // Parent constructor /n Child constructor
    Parent p2 = c;
    Parent * p = &c;
    p->display(); // Child virtual display
    callByReference(c); //Child virtual display
    callByValue(c); //Parent virtual display
    return 0;
}

