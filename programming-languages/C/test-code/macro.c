//int my_sqrt(int);
#include <math.h>
#include <stdio.h>
inline static double my_sqrt(double x);
double my_sqrt(double x){
    return x * x * x;
}
int main(){
    double s10 = my_sqrt(10);
    printf("%f\n", s10);
    return 0;
}