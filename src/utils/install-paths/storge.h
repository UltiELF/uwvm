#pragma once
#include <fast_io.h>
#include <fast_io_device.h>
#include <fast_io_dsal/string.h>

namespace uwvm::path {
    inline char* argv0{};
    inline ::fast_io::dir_file module_install_path_df{};
    inline ::fast_io::u8string module_path{};
    inline ::fast_io::u8string module_install_path{};
}