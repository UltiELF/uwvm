#pragma once
#include <fast_io.h>
#include <fast_io_dsal/vector.h>

namespace uwvm
{
#if(defined(_WIN32) || defined(__CYGWIN__)) ||                                                                                                                 \
    ((!defined(_WIN32) || defined(__WINE__)) && (defined(__CYGWIN__) || (!defined(__NEWLIB__) && !defined(__wasi__))))
    inline ::fast_io::vector<::fast_io::native_dll_file> plugins{};
#endif

    // plung import func map

}  // namespace uwvm
