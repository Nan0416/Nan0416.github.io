Created: 2020-01-03
Modified: 2020-01-03

* [Introduction](#intro)
* [References](#reference)
***
### <a id="intro">Introduction</a>

### <a id="examples">Examples</a>
1. Connect macro: ##, generates variable
```C
#define variable(n) var##n

int main(){
    int variable(1) = 10; // int var1 = 10;
    return 0;
}
```
Run preprocessor part to check what happened on the macro.
`gcc -E macro.c -o macro.i`

2. 

### <a id="reference">References</a>
1. <a href="https://guides.github.com/features/mastering-markdown/" target="_blank">Mastering Markdown</a>