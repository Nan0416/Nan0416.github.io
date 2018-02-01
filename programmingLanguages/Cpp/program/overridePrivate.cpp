#include <iostream>
using namespace std;
class F{
protected:
    int x = 0;
    void print(){cout << "P protected print!" << endl; }
public:
    virtual void draw(){cout << "P public draw!" << endl; }
    void showX(){ cout << x << endl;}
};
class E: public F{
public:
    int x;
    void print(){cout << "C public print!"  << endl; }
protected:
    void draw(){cout << "C protected draw!" << endl; }
};
int main(){
    E e;
   // e.draw();// ‘void E::draw()’ is protected
    e.print();
    e.x = 10;
    e.showX();
    F * f = new E();
    f->draw(); // invoke child protected...
    f->showX(); // invoke F's x
    return 0;
}