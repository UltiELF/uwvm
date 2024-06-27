#pragma once
#include <cstdint>
#include <cstddef>
#include <fast_io_dsal/string_view.h>
#include "../run/wasm_file.h"

namespace uwvm::vm
{
    inline ::std::size_t start_func{static_cast<::std::size_t>(-1)};

    inline ::std::size_t get_start_func(::uwvm::wasm::wasm_module const& wasmmod) noexcept
    {
        if(wasmmod.startsec.sec_begin) { return wasmmod.startsec.index; }

        for(auto const f: wasmmod.exportsec.func_types)
        {
            if(::fast_io::u8string_view{f->name_begin, static_cast<::std::size_t>(f->name_end - f->name_begin)} == u8"_start") { return f->index; }
        }

        return static_cast<::std::size_t>(-1);
    }

}  // namespace uwvm::vm
