Created: 2020-01-03
Modified: 2020-01-30

2020-01-30 @ Ziquanjun, Yangcun. 出国之后第一次回家过年。不巧赶上2019-

* [Introduction](#intro)
* [Preprocessor Directives & Macro](#macro)
* [Inline function](#inline)
* [References](#reference)
***
### <a id="intro">I. Introduction</a>
C preprocessor provides a compile-time feature. With certain syntax, the preprocessor will modifiy the code before compiling. `gcc -E macro.c -o macro.i` run preprocessor part.

Features provided by preprocessor:
1. Macro.
2. File inclusion.
3. inline function.

#### Start a newline in C program.
C allows program to start a newline for better readability. And the C preprocessor will eliminate newline `\` before processing the program.

For example,
```C
int ma\
in(){
    int temp = rand\
        om();
    int number = 21\
012;
    char * msg = "qin\
    nan";}    
```

will become
```C
int main(){
    int temp = rand om(); // because the `om()` doesn't start at the beginning.
    int number = 21012;
    char * msg = "qin   nan";}
```

### <a id="macro">II. Preprocessor Directives & Macro</a>

Preprocessor provides `directive`s to control how the program should be preprocessed.
* #define: used to define macro. `#define macro_name [macro_body]`. Depend on the preprocessor's implementation, it may abort or warn for macro redefinition. Some preprocessor may allow redefine marco with the same body. `macro` can be used as a function-like precedure, constant, or flags for other directives e.g. `#ifdef`.

* predefined macro: A compiler can have a few predefined macro available to the program. For example, 
1. compiler version: __STDC_VERSION__
2. line number: __LINE__
...

* #ifdef, #ifndef, #else, #endif: flow control directives.
```C
#define ABC
#undef ABC // cancel the definition of a macro.
#ifdef ABC
#warning "Defined ABC"
#else
#warning "Undefine ABC" // right here.
#endif
```

* #if, #elif: #if and #elif must be followed with a integer, this integer can be a constant, from another macro, from `==` or `defined` operator.
```C
#define ABC 0
#if defined(BCD)
#warning BCD
#elif ABC == 0
#warning "ABC == 0" // right here.
#elif 10
#warning "10"
#endif
```

* #warning: give a compilation warning.
* #error: give a compilation error and terminate the compilation.
```C
#if __STDC_VERSION__ != 201112L // C11 code.
#error Not C11
#else
#warning C11
#endif
```

* #pragma: a directive used to control the compiler.

#### Macro
* A macro will be replaced at compiling time with the macro body. A macro can also has `parameter`.
```C
#define sqrt_add(x, y) ((x)*(x)+(y)*(y))
#define sqrt_add2(x, y) x*x+y*y
```
`sqrt_add2` can cause unexpected bug. For example, in the following cases:
```C
100/sqrt_add2(2,1+3);
// expected to be 100 / (4 + 16) = 5. However, it will be 
// 100/2*2+1+3*1+3, which evaluates to 100 + 1 + 3 + 3 = 107

int x = 10;
sqrt_add(x, 1); // its result is 10*11 +1*1 = 111, instead of 101.
```

Using Macro defined precedure avoid `real` function's overhead, e.g. setup stack and pop result. So if a function is short, consider macro. <span style="color:red">However, a `real` function can have its address, e.g. function pointer.</span>. And then used as a variable.

* Connect macro: ##, generates variable
```C
#define variable(n) var##n
int main(){
    int variable(1) = 10; // int var1 = 10;
    return 0;
}
```
Run preprocessor part to check what happened on the macro.


### <a id="inline">III. Inline/static/extern function</a>
* C++ inline function: treated as a macro-function by replacing the its name with body.
* C99 & C11 standard say: "Making a function an inline function suggests that calls to the function be as fast as possible. The extent to which such suggestions are effective is implementation-defined". So if a inline-function is treated as inline is determined by the compiler.
* Even though C standard defined as above, `C inline function is only a hint such that the compiler doesn't complain about doubly defined symbols`.

### functions
When invoking a function, the function can come from
1. the same file: either in the current .c file or using `#include` to include from .h file.
2. the linker: either from the current compilation unit or using `extern` to indicate from other libraries.

* When defining a regular function in .c file, the function is added to the symbol table, which is used by the linker to link function call. Even thought the function is not declared in a header `.h` file or use `extern` keyword, the compilation can still be successful. It just gives a warning `implicit declaration`.
```C
// file1.c
int cal(int x){
    return x + 1;
}
// file2.c
int main(){
    printf("%d\n", cal(10));
    return 0;
}
// gcc file1.c file2.c
```

* Defining a regular function in a header file is correct but not practical. Because multiple .c files will include this .h file, it means the function will be declared multiple times, which cause `duplicate symbol` linker error.
```C
//header.h
int cal(int x){
    return x + 1;
}
// file1.c
#include "header.h"
int main(){
    printf("%d\n", cal(10));
    return 0;
}
// file2.c
#include "header.h"
int cal_add_one(int x){
    return cal(x) + 1;
}
// gcc file1.c file2.c
```

* A static function can only be used within the .c file who has its definition, and will not be added to the symbol table, which means a static function can be defined in either .h or .c file. If a static function is defined in .h file, .c file can use the `#include` directive to include its definition as if the function is defined in the .c file.

* `static` with `extern`: A static and regular function cannot co-exist within the same source file, which cause `redefinition of ...` error. But a static and regular function can co-exist within the same `compilation unit`.
```C
static int cal(int x); // static is part of the function declaration.
int main(){
    int x = cal(10);
}
int cal(int x){
    return x;
}
```

* A inline function should be in the same .c file (either .c or include via .h). 
Since a inline-function may be used as either a macro or a regular function, we don't know if it will appear on the symbol table. So it's better to define a inline-function as `inline static ...` in a header file.

    * In case it's treated as a regular function, `static` prevent from adding duplication in symbol table.
    * In case it's treated as a macro, using header file to include it to every .c file.

In addition, you should add the function's declaration. For example,
```C
// [extern] inline int my_square(int x); add declaration. Why?
inline int my_square(int x){
    return x * x;
}
int main(){
    int s10 = my_square(10);
    return 0;
}
```
The above code will give compilation error said the `my_square` symbol is not found. 

### <a id="reference">References</a>
1. <a href="" target="_blank">C Primer Plus (6th edition). Chapter 16.</a>
2. <a href="https://stackoverflow.com/questions/16245521/c99-inline-function-in-c-file/16245669#16245669" target="_blank">C99 inline function in .c file</a>