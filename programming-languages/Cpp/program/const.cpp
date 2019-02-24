#include <iostream>
using namespace std;
class G{
public:
    int x;
    G(){ // required by cascade initialization
        x = 1;
    }
};
class F{
public:
    int x = 1;
    int * y = new int(1);
    G g;
};
int main(){
    const F f;
    cout << f.x << *(f.y) << endl;
    *(f.y) = 2;
    // f.g.x = 10; cascade
    //f.y = new int(2); cannot assign to variable 'f' with const-qualified type 'const F'
    cout << f.x << *(f.y) << endl;
    return 0;
}
