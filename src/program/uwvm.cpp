#include "version_check.h"

#include <fast_io.h>
#include <fast_io_device.h>

#include <io_device.h>
#include <version.h>
#include <install-paths/storge_argv0.h>
#include "../clpara/impl.h"
#include "../run/run.h"

int main(int argc, char** argv)
{
    ::uwvm::path::argv0 = argc ? *argv : nullptr;

    switch(::uwvm::parsing(argc, argv, ::uwvm::parsing_result, ::uwvm::hash_table))
    {
        case ::uwvm::parsing_return_val::def: break;
        case ::uwvm::parsing_return_val::return0: return 0;
        case ::uwvm::parsing_return_val::returnm1: return -1;
        default: ::fast_io::unreachable();
    }

    ::uwvm::run();

    return 0;
}
