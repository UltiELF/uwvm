#pragma once
#include <fast_io.h>
#include "../section/custom.h"
#include "manager.h"

namespace uvwm::wasm::custom
{
    inline bool import_custom(::fast_io::native_dll_io_observer ndio) noexcept
    {
        /*
         *  extern "C"
         *  {
         *      csfunc_return_struct const* CustomSection()
         *  }
         */

        auto csfunc{reinterpret_cast<::uvwm::wasm::custom::csfunc_may_alias_ptr>(::fast_io::dll_load_symbol(ndio, u8"CustomSection"))};
        auto csfunc_returnval{csfunc()};

        for(; csfunc_returnval; ++csfunc_returnval) 
        {
            auto const& handlefunc{*csfunc_returnval};
            // to do
            /*
            * map add ...
            */
        }
    }
}  // namespace uvwm::wasm::custom
