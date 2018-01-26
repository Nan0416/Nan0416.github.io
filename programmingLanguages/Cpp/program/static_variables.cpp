#include <iostream>
using namespace std;

class F{
private:
    static int counter;
public:
    F(){
        counter++;
    }
    int getC(){
        return counter;
    }
};
int F::counter = 0;
int main(){
    F f[10];
    cout << f[0].getC()  << endl; // 1010
}
