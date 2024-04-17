#include <fast_io.h>
#include "../../src/wasmo/bjdump.h"
int main()
{
    ::uwvm::wasm::wasm_module m{};
    ::fast_io::io::perr(objdump(m)); // lv
    
    ::fast_io::io::perr(objdump(::uwvm::wasm::wasm_module{})); // rv

    auto o1{objdump(m)};
    auto o2{objdump(::uwvm::wasm::wasm_module{})};
    ::fast_io::io::perr(o1);
    ::fast_io::io::perr(o2);
}

/*
 * g++ -o xxx xxx.cc -std=c++23 -Ofast -s -flto -march=native -fno-exceptions -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-ident -mprefetchi -I../../third-parties/fast_io/include -I../../src/utils
 * */
