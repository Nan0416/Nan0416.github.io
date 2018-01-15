#include "Header.h"
#include <iostream>
void Inline::showDate(){
    std::cout << "No data" << std::endl;
}
Inline2::Inline2(){
    a = 11;
}
class S{
public:
    void showS();
};
void S::showS(){
    std::cout << "show S" << std::endl;
}
