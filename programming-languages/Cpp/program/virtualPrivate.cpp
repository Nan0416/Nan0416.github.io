#include <iostream>
using namespace std;
class F{
private:
    virtual void func(){cout << "Parent Private virtual function" << endl;}

};
class E: public F{
public:
    void func(){cout << "Child Public virtual function" << endl;}

};
int main(){
    E e;
    F * f = &e;
    e.func();
    //f->func(); private modifer static bind
}