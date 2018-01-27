#include <iostream>
using namespace std;

class F{
    friend void checkFriend(F & a);
protected:
    int pr_x = 2;
    static int pr_y;
    virtual void pr_Msg();
    void General();
};

class G: public F{
private:
    static int g_s; // Only g has
    int g_x = 5; // Only g has
protected:
    void pr_Msg(); // override parent method
};
void F::General(){
    cout << "F general" << endl;
}
void F::pr_Msg(){
    cout << "F protect msg" << endl;
}
int F::pr_y = -1;
void G::pr_Msg(){
    cout << "G protected msg" << endl;
}
int G::g_s = 11;

void checkFriend(F & a){
    a.General(); //
}
int main(){
    F g;
    checkFriend(g); // G protected msg
}
