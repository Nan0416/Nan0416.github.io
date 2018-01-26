#include <iostream>
using namespace std;
class F{
public:
    static void hello();
};
void F::hello(){
    cout << "hello" << endl;
}
void hu(){
    cout << "hu" << endl;
}
int main(){
    F * f = new F();
    f->hello;
    void (*foo)() = hu;
    foo();
    f->hello = foo;
}
