Created: 2020-01-03
Modified: 2020-01-03

* [Introduction](#intro)
* [Symbol table](#symbol)
* [Keyword](#keyword)
* [References](#reference)
***
### <a id="intro">Introduction</a>
1. No Object, No Inheritance. No Interface. No template. in the language itself. Only struct and procedure (function). 
<span style="color:red">But object and inheritance's style can be written in code. (e.g. GObject)</span>

2. Separate .h file for API reference and compilation.
3. Memory manipulation and pointer.
4. C code is compiled into machine code, which requires minimal runtime support. 
5. Marco (kind of customized syntax)
6. No namespace.

History. (wiki)
> C was originally developed at Bell Labs by Dennis Ritchie between 1972 and 1973 to make utilities running on Unix. 
> Later, it was applied to re-implementing the kernel of the Unix operating system
> C has been standardized by the ANSI since 1989 (ANSI C) and by the ISO.

#### Standard
* C89 from ANSI
* C90 same language standard as C89 from ISO.
* C95: support multi-byte char, wchar.
* C99
* C11
* C18

#### Compiler
gcc, clang.

#### C standard library and POSIX C library
* POSIX:  Portable Operating System Interface, IEEE standard for maintaining compatibility between operating systems. It specifies Operating systems provided API (e.g. closeand command lines (such as ls, cd).
* POSIX is also a superset of C library.
* C standard library (ISO C library) (libc): defines API for math, io, memory management and serveral os services.
* C standard library doesn't define complex data structure, such as hashmap, set, and list.

C standard library is usually distributed with the operating system since POSIX is a superset of C standar library.
* GUN C library (glibc): from GNU/Linux
* BSD libc: from BSD, MacOS.
e.g. malloc, stdio.h, stdlib.h

#### Data structure for C
C data structures, such as hash table, list, are provided by third parties and there is no such standard.

* Glib (GNOME libraries)

#### System API
Operating systems (e.g. Linux) provides API for programs to access system services. For example,
Linux's `unistd.h` provides `fork` function to allow program to create new processes.

### <a id="symbol>Symbol Table</a>
A global, non static function or variable will be place into the symbol table. Symbol table tells the linker what functions/variables are avaliable from the compiled file. 

### <a id="keyword">Keywords</a>
1. extern
Declare a function or a variable is defined somewhere else and the code can use it right here.
```C
extern int cal(int);
int main(){
    int x = cal(10);
    printf("%d\n",x);
    return 0;
}
```
The code can be compiled with -c, but cannot generate executable (must supply the cal's implementation to linker.)

2. static 
A function defined in .c file, by default, will be exported and visible to other functions. Using `static` keywords makes the function or variable only be usable within the current file.

### <a id="reference">References</a>
1. <a id="https://stackoverflow.com/questions/6118539/why-are-there-no-hashtables-in-the-c-standard-library" target="_blank">Why are there no hashtables in the c standard library?</a>