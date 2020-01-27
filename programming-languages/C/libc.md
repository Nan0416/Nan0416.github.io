Created: 2020-01-22
Modified: 2020-01-22

* [Introduction](#intro)
* [Code](#code)
* [References](#reference)
***
### <a id="intro">I. Introduction</a>
libc is a C language standard provided library. It's not related with operating system. 
* ISO C11 is a libc standard.
* POSIX C is a superset of libc standard, which also defines API, services and others for operating system.
* GUN C Library (glibc) is an implementation and available in Linux.


#### GUN C Library
* [Reference](https://www.gnu.org/software/libc/)
* [github mirror](https://github.com/bminor/glibc) 
* [mirror in china](https://gitee.com/Aloxaf/glibc.git)

### <a id="code">Code</a>
1. stddef.h
```C
// linux gun x86 64.
#define __SIZE_TYPE__ long unsigned int
typedef __SIZE_TYPE__ size_t;
```
* sizeof is an operator instead of a function, it returns the input variable's size in `size_t`.
* order of `unsigned`, `int`, `long` doesn't matter.
* `unsigned long int` 64 bits.

size_t in different machine has a different definition, but in Linux, it's guaranteed to be a 64 bit unsigned integer.

2. stdlib.h
malloc

3. string.h
```C
extern int strcmp (const char *__s1, const char *__s2) __THROW __attribute_pure__ __nonnull ((1, 2));

```
### <a id="reference">References</a>
1. <a href="https://guides.github.com/features/mastering-markdown/" target="_blank">Mastering Markdown</a>