Created: 2020-01-03
Modified: 2020-01-03

* [Compilation](#compilation)
* [pkg-config](#pkg-config)
* [References](#reference)
***
### <a id="compilation">Compilation</a>
1. Compiling (.c -> .o) a source file only requires the included header files (API declaration) but not the library files (implementation).
2. Linking compiled code requires all compiled code source (e.g. .o .a and .so). 
3. Running the exeuctable file requires the shared libraries at runtime.

```shell
# compilation time
gcc -S code.c
as code.s -o code.o # object files
gcc -shared -o libcode lcode.so

# gcc -c code.c to generate the .o file

# linking time
gcc -o exeutable main.o -lcode -L .

# runtime
./executable # must also has the lcode installed in the runtime system.
```

#### dynamic/shared library
windows .dll,
linux .so
macos .so or .dylib

#### executable
1. startup code: different operating systems have different startup code. For example, the address of the first instruction (e.g. _startup label.) The startup code typically reset registers and invoke `main`.
2. a symbol table has main function and other dependent functions from either the same source code or libraries.


### <a id="pkg-config">pkg-config</a>
pkg-config is a command line tool for finding metainformation about installed libraries.

Here, metainformation includes
* header file location
* library file location
* linked libraries name.

When compiling a file, gcc can accept `pkg-config` to get the neccessary dependency information.

For example,
```Bash
gcc `pkg-config --cflags --libs glib-2.0` test.c
# same as
gcc

-I/usr/local/Cellar/glib/2.62.4/include/glib-2.0 # -I header file location.
-I/usr/local/Cellar/glib/2.62.4/lib/glib-2.0/include 
-I/usr/local/opt/gettext/include 
-I/usr/local/Cellar/pcre/8.43/include 
-L/usr/local/Cellar/glib/2.62.4/lib # -L library file location
-L/usr/local/opt/gettext/lib 
-lglib-2.0 # -l library name.
-lintl

test.c
```

### <a id="reference">References</a>
