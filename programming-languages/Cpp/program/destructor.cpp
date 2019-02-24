#include <iostream>
using namespace std;
class F1{
public:
    int * x;
    F1();
    ~F1();
};
F1::F1(){
    x = new int [10];
}
F1::~F1(){
    cout << "destructor" << endl;
    delete x;
}
int main(){
    F1 f;
    F1 f_ = f;
    /*destructor
    destructor
    a.out(5044,0x7fffe882c3c0) malloc: *** error for object 0x7fb10f500000: pointer being freed was not allocated
        *** set a breakpoint in malloc_error_break to debug
        Abort trap: 6*/
}
