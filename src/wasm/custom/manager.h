#pragma once
#include <fast_io.h>
#include <fast_io_dsal/string_view.h>
//#include <fast_io_dsal/btree_map.h>
#include <map>

#include "builtin.h"

namespace uwvm::wasm::custom
{
    using handlefunc_may_alias_ptr = bool (*)(::uwvm::wasm::section::custom_section);

    struct csfunc_return_struct
    {
        handlefunc_may_alias_ptr func{};
        char8_t const* name_begin{};
    };

    using csfunc_may_alias_ptr = csfunc_return_struct const* (*)();

    // storage
    // use ::fast_io::btree_map instead
    inline ::std::map<::fast_io::u8string_view, handlefunc_may_alias_ptr> custom_handle_funcs{
        {u8"name", __builtin_addressof(::uwvm::wasm::custom::scan_name_custom_section)}
    };

}
