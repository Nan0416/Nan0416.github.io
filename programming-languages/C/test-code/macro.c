#define variable(n) var##n
#define stringify(s) #s

int main(){
    int variable(1) = 10;
    int number = 20;
    char * name = stringify(number);
    char * name2 = stringify(variable(100));
    return 0;
}