// .h file
#include <iostream>
namespace H {
 
class F{
 public:
  void print();
};
 
}  // namespace H
namespace K {
 
class F{
 public:
  void print(){
    std::cout << "K-F" << std::endl;
  }
};
 
}  // namespace K
