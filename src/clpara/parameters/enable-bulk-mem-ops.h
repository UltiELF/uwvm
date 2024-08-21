#pragma once

#include <fast_io.h>
#include <fast_io_freestanding.h>

#include <io_device.h>
#include <cmdline/parameter.h>
#include "../../run/features.h"

namespace uwvm::parameter
{
    namespace details
    {
        inline constexpr ::fast_io::string_view enable_bulk_memory_operations_alias{"-Ebmops"};
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter enable_bulk_memory_operations{
        .name{::fast_io::string_view{"--enable-bulk-memory-operations"}},
        .describe{::fast_io::u8string_view{
            u8"Enable bulk memory operations for WebAssembly (https://github.com/WebAssembly/bulk-memory-operations/blob/master/proposals/bulk-memory-operations/Overview.md). Usage: [--enable-bulk-memory-operations|-Ebmops]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::enable_bulk_memory_operations_alias), 1}},
        .is_exist{__builtin_addressof(::uwvm::features::enable_bulk_memory_operations)},
    };
}  // namespace uwvm::parameter
