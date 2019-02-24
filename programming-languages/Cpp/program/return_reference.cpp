
#include <iostream>
using namespace std;

class F{
public:
    int x = 0;
};
//warning: reference to stack memory associated with local variable 'f' returned [-Wreturn-stack-address]
F& return_reference(){
    F f;
    f.x = 1;
    return f;
}
//warning: address of stack memory associated with local variable 'f' returned [-Wreturn-stack-address]
F* return_address(){
    F f;
    f.x = 1;
    return &f;
}
//allowed 
F* return_out(F& f){
    return &f;
}
int main(){
    F *fa = return_address();
    cout << fa->x << endl;
    F fr = return_reference();
    cout << fr.x << endl;
}

