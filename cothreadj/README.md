# setjmp / longjmp implementation ![C library](https://github.com/cedric84/libcothread2/actions/workflows/build.yml/badge.svg?branch=ci)

This implementation aims to be as simpler as possible,
uses the ISO C [setjmp & longjmp](https://www.man7.org/linux/man-pages/man3/setjmp.3.html) functions
to save & restore the CPU registers and some assembly code to initialize the new execution context.
Although it works fine for C programs, it reveals some troubles for C++ ones when targeting the MS-Windows
operating system (see the [compatibility matrix](#compatibility-matrix) below.)

## How to use the library
The [cothreadj.h](lib/include/cothread/cothreadj.h) main header defines the `cothreadj_t` structure
which contains all the data needed to switch from an execution context to another one:
1. To initialize such a structure, the user shall first configure some `cothreadj_attr_t` attributes
with the `cothreadj_attr_init` function, and use these attributes in a call to the `cothreadj_init` one ;
2. Once the `cothreadj_t` is initialized, the user may call the `cothreadj_set_user_data` function to store
any data he/she wants, and the `cothreadj_get_user_data` one to retrieve it
(the library never modifies this user data) ;
3. From this point, the current execution context may be paused and the created one resumed using
the `cothreadj_yield` function (note that this function may be called many times to get back
to the previous execution context.)

For debugging purposes, the user may name the execution contexts using
the `cothreadj_attr_set_dbg_caller_name` and the `cothreadj_attr_set_dbg_callee_name` functions
and instruct the library to log some informations to an output stream using
the `cothreadj_attr_set_dbg_strm` one.

See this [example](examples/cothreadj_example0.c) for more details.

## Compatibility matrix
### Supported
![cothreadj-cxx-x86-gnu_linux](https://github.com/cedric84/libcothread2/actions/workflows/cothreadj-cxx-x86-gnu_linux.yml/badge.svg?branch=ci)
![cothreadj-cxx-x86_64-gnu_linux](https://github.com/cedric84/libcothread2/actions/workflows/cothreadj-cxx-x86_64-gnu_linux.yml/badge.svg?branch=ci)
![cothreadj-cxx-x86_64-macos](https://github.com/cedric84/libcothread2/actions/workflows/cothreadj-cxx-x86_64-macos.yml/badge.svg?branch=ci)
![cothreadj-cxx-x86-mingw](https://github.com/cedric84/libcothread2/actions/workflows/cothreadj-cxx-x86-mingw.yml/badge.svg?branch=ci)

### Unsupported
![cothreadj-cxx-x86_64-mingw](https://github.com/cedric84/libcothread2/actions/workflows/cothreadj-cxx-x86_64-mingw.yml/badge.svg?branch=ci)
![cothreadj-cxx-x86_64-windows](https://github.com/cedric84/libcothread2/actions/workflows/cothreadj-cxx-x86_64-windows.yml/badge.svg?branch=ci)
![cothreadj-cxx-x86-windows](https://github.com/cedric84/libcothread2/actions/workflows/cothreadj-cxx-x86-windows.yml/badge.svg?branch=ci)
