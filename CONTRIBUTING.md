# Contributing to ulti-wvm

## Coding style

- Source file postfix: .cpp, not .cc
- Indentation: 4 spaces
- Try to minimize the use of object-oriented programming. (The compiler front-end does not do any processing, and excessive use of oo will only garbage the IR.)
- Try to minimize the use of C++ host features and replace them with system APIs and freestanding features.
- Prohibit the use of C++ exceptions, rtti, stream (like iostream, etc.), format (like format string of stdio, fmt lib, c++ 20 print, c++ 23 print, etc.)
- Do not use until the module proposal is complete
- Name of functions, variables, classes(structures), etc. : `aaa_bbb_ccc`
- Avoid using smart pointers unless you have to.
- Write code according to [Portable-Cpp-Guideline](https://github.com/cppfastio/Portable-Cpp-Guideline)
- Encoding: utf-8 or utf-8 with BOM
