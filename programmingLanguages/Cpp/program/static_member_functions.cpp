
#include <iostream>
using namespace std;

int global_x = 10;
void global_func(){
    cout << "global_func() is called" << endl;
}
class F{
public:
    static void f(){
        cout << "global_x is " << global_x << endl;
        cout << "f() is called" << endl;
        global_x ++;
        global_func();
    }
    static void G();
};
void F::G(){
    cout << "global_x is " << global_x << endl;
    cout << "g() is called" << endl;
    global_x ++;
    global_func();
}
int main(){
    F g;
    F::f();
    /*global_x is 10
    f() is called
    global_func() is called*/
   
    g.G();
    /*global_x is 11
    g() is called
    global_func() is called*/
}
