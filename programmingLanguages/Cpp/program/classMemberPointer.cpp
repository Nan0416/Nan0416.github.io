
#include <iostream>
using namespace std;
class F{
public:
    int x;
    string s;
    F(string s, int x);
    void inc(void);
};
F::F(string s, int x){
    this->s = s;
    this->x = x;
}
void F::inc(void){
    static int temp = 0;
    ++x;
    cout << "call " << s << " " << ++temp << "th with x = " <<  x << endl;
}
void (F::*fp)(void);
int main(){
    F f("F", 0), g("G", 0);
    f.inc(); //call F 1th with x = 1
    f.inc(); //call F 2th with x = 2
    int * i = &(f.x);
    (*i)++;
    f.inc(); //call F 3th with x = 4
    g.inc(); //call G 4th with x = 1
    fp = &F::inc;
    (g.*fp)(); //call G 5th with x = 2
}
