#include <iostream>
using namespace std;

class F{
public: //protected but no private
    virtual void pureVirtual() = 0;

};

class D: public F{
private:
    void pureVirtual(){
        cout << "D" << endl;
    }
};
int main(){
   // F f;
   F * f = new D();
    f->pureVirtual();
    return 0;
}