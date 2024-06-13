#pragma once
#include <fast_io.h>
#include <fast_io_dsal/btree_map.h>

#include "../section/custom.h"
#include "manager.h"

namespace uwvm::wasm::custom
{
    inline bool import_custom_section_from_dll(::fast_io::native_dll_io_observer ndio) noexcept
    {
        auto csfunc{reinterpret_cast<::uwvm::wasm::custom::csfunc_may_alias_ptr>(::fast_io::dll_load_symbol(ndio, u8"CustomSection"))};
        if(csfunc)
        {
            auto csfunc_returnval{csfunc()};

            for(; csfunc_returnval; ++csfunc_returnval)
            {
                auto const& handlefunc{*csfunc_returnval};
                if(handlefunc.func && handlefunc.name_begin) [[likely]]
                {
                    ::fast_io::u8string_view u8sv{};
                    if(handlefunc.name_end)
                    {
                        u8sv = ::fast_io::u8string_view{handlefunc.name_begin, static_cast<::std::size_t>(handlefunc.name_end - handlefunc.name_begin)};
                    }
                    else { u8sv = ::fast_io::u8string_view{::fast_io::mnp::os_c_str(handlefunc.name_begin)}; }
                    ::uwvm::wasm::custom::custom_handle_funcs.insert({u8sv, handlefunc.func});
                }
                else { break; }
            }
            return true;
        }
        return false;
    }
}  // namespace uwvm::wasm::custom
