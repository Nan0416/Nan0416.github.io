#include <iostream>
using namespace std;
class BankAccount{
public:
    BankAccount(); // no return type
    BankAccount(int x);
    BankAccount(double balance, int x);
    ~BankAccount();
    int getX();
private:
    int x;
    double balance;
};

BankAccount::BankAccount(){
    x = 0;
    cout << "H" << endl;
    // this->BankAccount(11); // wrong, cannot call self class constructor inside a constrcutor
    //BankAccount::BankAccount(11);
}
BankAccount::BankAccount(int x){
    this->x = x;
}
BankAccount::BankAccount(double balance, int x):balance(balance), x(x){
    cout << this->balance << endl;
}
int BankAccount::getX(){
    // this->BankAccount(11); // wrong, cannot call self class constructor inside a member function
    return x;
}
int main(){
    BankAccount ba; // BankAccount ba(); is wrong
    cout << ba.getX() << endl;
    BankAccount ba3(1.01, 110);
    cout << ba3.getX() << endl;
    //BankAccount bc = BankAccount();
    //BankAccount * baP = new BankAccount(10);
    //cout << baP->getX() << endl;
    //delete baP;
}
