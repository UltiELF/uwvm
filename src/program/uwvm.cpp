﻿#include "version_check.h"

#include <fast_io.h>
#include <fast_io_device.h>

#include <io_device.h>
#include "../clpara/impl.h"
#include <version.h>
#include <install-paths/storge_argv0.h>
#include "../run/run.h"

int main(int argc, char** argv)
{
    ::uwvm::path::argv0 = argc ? *argv : nullptr;

    auto& parse_res{::uwvm::parsing_result};
    switch(int const pr{::uwvm::parsing(argc, argv, parse_res, ::uwvm::hash_table)}; pr)
    {
        case -1: return -1;
        case 0: break;
        case 1: return 0;
        default: ::std::unreachable();
    }

    ::uwvm::run();

    return 0;
}
