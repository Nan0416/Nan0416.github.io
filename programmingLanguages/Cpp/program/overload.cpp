
#include <iostream>
using namespace std;


class F{};
class G: public F{};
void func(F x){cout << "p" << endl;}
void func(F & x){cout << "c" << endl;}

int main(){
    G g;
    func(g);
   
}

