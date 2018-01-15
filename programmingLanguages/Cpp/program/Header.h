#ifndef H
#define H
#include <iostream>
class Inline{
public:
    void showTime();
    void showDate();
};

class Inline2{
private:
    int a;
public:
    Inline2();
    void show();
    int getA(){
        // actually an inline function
        return a;
    }
};
#endif
