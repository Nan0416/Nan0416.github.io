#include <iostream>
using namespace std;

class F{
public:
    int x = 11;
    void get(int &y) const;
};
void F::get(int &y) const{
    y = x; // y is still mutable
    // x++; //cannot assign to non-static data member within const member function 'get'
}

int main(){
    F f;
    int y;
    f.get(y);
    cout << y << endl;
    return 0;
}
