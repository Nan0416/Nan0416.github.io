# 1 "macro.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 363 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "macro.c" 2
# 1 "./test.h" 1
int my_sqrt(int i);
inline static int my_sqrt(int x){
    return x * x;
}
# 2 "macro.c" 2
int main(){
    int s10 = my_sqrt(10);
    return 0;
}
