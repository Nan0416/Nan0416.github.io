#include <iostream>
#include "Header.h"
#include "Header2.h"
#include "Example2.cpp"
void Inline::showTime(){
    std::cout << "Jan 2, 2018 2:23AM" << std::endl;
}
void Inline2::show(){
    std::cout << "show" << std::endl;
}
void H::test(){
    std::cout << "test" << std::endl;
}
class S2{
public:
    void showS();
};
void S2::showS(){
    std::cout << "show S2" << std::endl;
}
int main()
{
    Inline a;
    a.showTime();
    a.showDate();
    Inline2 a2;
    a2.show();
    H h1;
    h1.test();
    std::cout << a2.getA() << std::endl;
    S2 s;
    s.showS();
    return 0;
}
