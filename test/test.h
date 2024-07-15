#pragma once
#include "failed.h"
#include "memory/memory.h"
#include "wasi/wasi.h"

namespace uwvm::test
{
    inline void test() noexcept
    {
        // test
        ::uwvm::test::test_memory();
        ::uwvm::test::test_wasi();
        if(::uwvm::test::failed) [[unlikely]] { ::fast_io::fast_terminate(); }
    }
}  // namespace uwvm::test
