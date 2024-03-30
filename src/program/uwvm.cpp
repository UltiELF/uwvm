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

    // to do

    return 0;
}
