#include <iostream>
using namespace std;


int (*f)(int);
int * b;
int inc(int a){
    static int counter = 0;
    cout << "static counter " <<  ++counter << endl;
    cout << "input value " << a << endl;
    return ++a;
}
int a = 1;

class F{
public:
    void callDataRef(int *);
    void callFunRef(int (*)(int));
    
    void callDataCopy(int);
    void callFunCopy(int (int));
    
};
void F::callDataRef(int *a){
    cout << "call data ref " <<  *a << endl;
}
void F::callFunRef(int (*f)(int)){
    //cout << "call func ref " << f(1) << endl;
    int x = (*f)(1);
    cout << "call func ref " << x << endl;
}
void F::callDataCopy(int a){
    cout << "call data copy (value) " << a << endl;
}
void F::callFunCopy(int f(int)){
    int x = f(1);
    cout << "call func val " << x << endl;
    
}
F e;
int main(){
    //assignment
    b = &a;
    f = &inc;
    e.callFunCopy(inc);
    e.callFunCopy(inc);
    e.callFunRef(inc);
    
    

}
