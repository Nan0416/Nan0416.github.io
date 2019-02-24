#include <iostream>
using namespace std;
class Complex{
public:
    int x;
    int y;
    Complex();
    Complex(int x, int y);
    Complex operator = (Complex value);
    Complex(const Complex & value);
};
Complex::Complex():Complex(0,0){
    
}
Complex::Complex(int x, int y):x(x), y(y){
    cout << "constructor" << endl;
}
Complex::Complex(const Complex & value){
    x = value.x;
    y = value.y;
    cout << "copy constructor" << endl;
}
Complex Complex::operator = (Complex value){
    cout << "assigment constructor" << endl;
    this->x =  value.x;
    this->y =  value.y;
    return *this;
}

int main(){
    Complex c(1,1);
    Complex c2;
    Complex x = (c2 = c);
}
