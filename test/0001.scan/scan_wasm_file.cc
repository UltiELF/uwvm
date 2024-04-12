#include <fast_io.h>
#include <fast_io_driver/timer.h>
#include "../../src/run/detect.h"

int main(int argc, char** argv)
{
    if(argc != 2) [[unlikely]]
    {
        if(argc == 0) [[unlikely]] { return 1; }
        ::fast_io::io::perr("Usage: ", fast_io::mnp::os_c_str(*argv), " <file>\n");
        return 1;
    }
    ::fast_io::native_file_loader loader(::fast_io::mnp::os_c_str(argv[1]));
    ::fast_io::timer timer{u8"scan"};
    ::uwvm::scan_wasm_file<true>(reinterpret_cast<::std::byte const*>(loader.cbegin()), reinterpret_cast<::std::byte const*>(loader.cend()));
}

/*
* g++ -o scan_wasm_file scan_wasm_file.cc -std=c++23 -Ofast -s -flto -march=native -fno-exceptions -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables -fno-ident -mprefetchi -I../../third-parties/fast_io/include -I../../src/utils
*/
