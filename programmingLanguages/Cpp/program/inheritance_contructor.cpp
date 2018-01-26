
#include <iostream>
using namespace std;
class Parent{
public:
    Parent();
    Parent(int x, int y);
};
Parent::Parent(){
    cout << "Parent constructor" << endl;
}
Parent::Parent(int x, int y){
    cout << "Parent (int, int) constructor" << endl;
}
class Child: public Parent{
private:
    int x;
    int y;
public:
    Child();
    Child(int x, int y);
    Child(int x, int y, int z);
};
Child::Child(){
    Parent();
    cout << "Child constructor" << endl;
}
Child::Child(int x, int y):y(y),x(x){
    cout << "Child (int, int) constructor" << endl;
}
Child::Child(int x, int y, int z):Parent(x,y){
    cout << "Child (int, int, int) constructor" << endl;
}
int main(){
    Parent p; // Parent constructor
    Child c; //  Parent constructor /n Child constructor
    Child c1(1,1); //Parent constructor /n Child (int, int) constructor
    Child c2(1,1,1); // Parent (int, int) constructor /n Child (int, int, int) constructor
    return 0;
}
