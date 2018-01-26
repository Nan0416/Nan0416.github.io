#include <iostream>
using namespace std;
class Complex{
public:
    int x = 0;
    int y = 0;
    Complex();
    Complex(int x, int y);
    Complex(const Complex &c);
    // const means c cannot be modified, & means call by reference,
    // it cannot use call by value, since the call by value will use this constructor.
};
Complex::Complex(int x, int y):x(x), y(y){
    cout << "constructor called" << endl;
}
Complex::Complex():Complex(0,0){
}
Complex::Complex(const Complex &c){
    cout << "copy constructor" << endl;
    x = c.x;
    y = c.y;
}
void printComplexReference(Complex & c){
    if(c.y > 0){
        cout << c.x << " + i" << c.y << endl;
    }else if(c.y < 0){
        cout << c.x << " - i" << -c.y << endl;
    }else{
        cout << c.x << endl;
    }
}
void printComplexValue(Complex c){
    if(c.y > 0){
        cout << c.x << " + i" << c.y << endl;
    }else if(c.y < 0){
        cout << c.x << " - i" << -c.y << endl;
    }else{
        cout << c.x << endl;
    }
}
int main(){
    Complex c(1,-1); // constructor called
    printComplexReference(c); // 1 - i1
    printComplexValue(c); // copy constructor /n 1 - i1
    Complex x = c; // copy constructor
}
