#include <iostream>
using namespace std;
class G{
public:
    G(int x){
        cout << "G user-defined non-default constructor " << x << endl;
    }
};
class E{
public:
    // G g; because G does not have a default constructor, this is not allowed.
    G g = G(0);
    // G g(0); it is also not allowed.
    E(){
        cout << "E user-defined default constructor" << endl;
    }
    E(int x){
        cout << "E user-defined non-default constructor" << endl;
    }
};
class F{
public:
    E e; // invoke a G(0) and E()
    F(){
        e = E(11); // invoke a G(0) and E(11) [and assignment operator].
    }
};
int main(){
    F f;
    /*G user-defined non-default constructor 0
    E user-defined default constructor
    G user-defined non-default constructor 0
    E user-defined non-default constructor*/
    return 0;
}
