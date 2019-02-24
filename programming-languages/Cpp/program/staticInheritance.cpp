#include <iostream>
using namespace std;

class F{
public:
    static int x;
};
int F::x = 10;
class G: public F{
public:
    static int x;
};
int G::x =11;
int main(){
    cout << F::x << endl;
    cout << G::x<< endl;
}
