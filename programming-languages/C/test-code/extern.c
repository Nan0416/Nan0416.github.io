#include <stdio.h>

int cal(int x){
    return x + 1;
}
extern int cal2(int);
int main(){
    int x = cal2(10);
    printf("%d\n",x);
    return 0;
}