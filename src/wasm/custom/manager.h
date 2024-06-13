#pragma once
#include <fast_io.h>
#include <fast_io_dsal/string_view.h>
#include <fast_io_dsal/btree_map.h>

#include "builtin.h"

namespace uwvm::wasm::custom
{
    using handlefunc_may_alias_ptr =
#if __has_cpp_attribute(__gnu__::__may_alias__)
        [[__gnu__::__may_alias__]]
#endif
        bool (*)(::uwvm::wasm::section::custom_section);

    struct csfunc_return_struct
    {
        handlefunc_may_alias_ptr func{};
        char8_t const* name_begin{};
        char8_t const* name_end{};
    };

    using csfunc_may_alias_ptr =
#if __has_cpp_attribute(__gnu__::__may_alias__)
        [[__gnu__::__may_alias__]]
#endif
        csfunc_return_struct const* (*)();

    // storage
    inline ::fast_io::btree_map<::fast_io::u8string_view, handlefunc_may_alias_ptr> custom_handle_funcs{};

    inline void init_builtin_custom_section() noexcept 
    { 
        custom_handle_funcs.insert({u8"name", __builtin_addressof(::uwvm::wasm::custom::scan_name_custom_section)});
    }
}