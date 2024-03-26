#include <fast_io.h>
#include <fast_io_device.h>

#include <io_device.h>
#include "../clpara/impl.h"
#include <version.h>
#if !defined(__MSDOS__)
    #include "../path/install_path.h"
#endif

int main(int argc, char** argv)
{
#if !defined(__MSDOS__)
    ::uwvm::path::init_install_path(argc ? *argv : nullptr);
#endif

    auto& parse_res{::uwvm::parsing_result};
    int pr{::uwvm::parsing(argc, argv, parse_res, ::uwvm::hash_table)};

    if(pr != 0) { return 0; }

    ::fast_io::io::print(::uwvm::u8out,
                         u8"\033[0m"
#ifdef __MSDOS__
                         u8"\033[35m"
#else
                         u8"\033[95m"
#endif
                         u8"uwvm "
                         u8"\033[32m"
                         u8"V",
                         ::uwvm::uwvm_version,
                         u8"\033[0m\n");

    return 0;
}
