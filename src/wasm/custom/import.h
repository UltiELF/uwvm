#pragma once
#include <fast_io.h>
#include <fast_io_dsal/btree_map.h>

#include "../section/custom.h"
#include "manager.h"

namespace uwvm::wasm::custom
{
    inline bool import_custom(::fast_io::native_dll_io_observer ndio) noexcept
    {
        /*
         *  extern "C"
         *  {
         *      csfunc_return_struct const* CustomSection()
         *  }
         */

        auto csfunc{reinterpret_cast<::uwvm::wasm::custom::csfunc_may_alias_ptr>(::fast_io::dll_load_symbol(ndio, u8"CustomSection"))};
        if(csfunc)
        {
            auto csfunc_returnval{csfunc()};

            for(; csfunc_returnval; ++csfunc_returnval)
            {
                auto const& handlefunc{*csfunc_returnval};
                if(handlefunc.name_begin && handlefunc.func) [[likely]]
                {
                    ::uwvm::wasm::custom::custom_handle_funcs.insert({
                        ::fast_io::u8string_view{handlefunc.name_begin, static_cast<::std::size_t>(handlefunc.name_end - handlefunc.name_begin)},
                        handlefunc.func
                    });
                }
            }
            return true;
        }
        return false;
    }
}  // namespace uwvm::wasm::custom
