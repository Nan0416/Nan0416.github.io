
#include <iostream>
using namespace std;

class Testing{
private:
    int x;
public:
    Testing();
    int get();
    void set(int x);
};
Testing::Testing(){
    x = 0;
}
int Testing::get(){
    return x;
}
void Testing::set(int x){
    this->x = x;
}

//call by reference
void incByRef(Testing & t){
    t.set(t.get()+1);
}
//call by value
void incByVal(Testing t){
    t.set(t.get()+1);
}
int main(){
    Testing t;
    t.set(1);
    cout << "before call incByRef " << t.get() << endl;
    incByRef(t);
    cout << "after call incByRef " << t.get() << endl;
    cout << "before call incByVal " << t.get() << endl;
    incByVal(t);
    cout << "after call incByVal " << t.get() << endl;
    Testing & reft = t; // declare an alias. Usually it is not meaningful
    cout << "before call incByRef(reft) " << t.get() << endl;
    incByRef(reft);
    cout << "after call incByRef(reft) " << t.get() << endl;
    return 0;
}
