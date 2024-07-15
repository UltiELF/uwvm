#pragma once
#include "memory/memory.h"
#include "wasi/wasi.h"

namespace uwvm::test
{
    inline void test() noexcept
    {
        // test
        ::uwvm::test::test_memory();
        ::uwvm::test::test_wasi();
    }
}  // namespace uwvm
