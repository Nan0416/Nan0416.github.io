#include <iostream>
using namespace std;

class F{
public:
    F(int x){cout << "F(int x) constructor" << endl; }
};
class E: public F{
public:
    E():F(1){}
};

int main(){
    E e; //no matching function for call to ‘F::F()’ class E: public F{

    return 0;
}