#pragma once
#include "../import.h"

namespace uwvm::vm::interpreter
{
    inline void call_import_func(::std::size_t index, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const fun{::uwvm::vm::interpreter::imports.index_unchecked(index)};
        ::fast_io::fast_terminate(); // to  do
    }
}
