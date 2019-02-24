// .cc file
#include "namespace.h"
#include <iostream>
using namespace H; 
 
void F::print(){
    std::cout << "H-F" << std::endl;
}
 
  // namespace H
 
int main(){
  F f;
  F e;
  f.print();
  e.print();
}
