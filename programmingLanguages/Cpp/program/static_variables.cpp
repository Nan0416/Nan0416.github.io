#include <iostream>
using namespace std;

class F{
private:
    static int counter;  // initialize non-const static variable in definition is not allowed.
public:
    F(){
        counter++;
    }
    int getC(){
        return counter;
    }
    void inc(){
        counter ++;
    }
};
class G: public F{
    
};
int F::counter = 0; // this line must be included. It initialize/define this value, otherwise linker cannot found this variable.
int main(){
    F f; // constructor counter ++ => counter = 1;
    G g; // constructor counter ++ => counter = 2;
    cout << f.getC()  << endl; // 2
    g.inc();
    cout << f.getC() << endl; // 3
}
