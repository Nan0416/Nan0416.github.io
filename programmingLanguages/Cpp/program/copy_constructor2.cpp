#include <iostream>
using namespace std;
class F1{
public:
    F1();
    F1(const F1 & f);
};
F1::F1(const F1 & f){
    cout << "F1 copy constructor" << endl;
}
F1::F1(){
    cout << "F1 user defined default constructor" << endl;
}

class F2{
public:
    F1 f;
};
int main(){
    F2 f2; // F1 user defined default constructor // since F2 has a F1
    F2 a[10]; // invoke 10 times.
    F2 * b = new F2[10]; // invoke 10 times
    F2 f2_ = f2; // F1 copy constructor, F2 default copy constructor also invokes F1 copy constructor
}
