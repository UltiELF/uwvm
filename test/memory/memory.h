#pragma once
#include "../../src/vm/interpreter/memory/memory.h"

namespace uwvm::test
{
    inline void test_memory() noexcept {
        ::uwvm::wasm::memory_type mt{
            .limits{.min{10}, .max{1000}}
        };
        ::uwvm::vm::interpreter::memory::memory_t m{};
        m.init_by_memory_type(mt);
        ::fast_io::io::print(::fast_io::obuffer_view{reinterpret_cast<char*>(m.memory_begin), reinterpret_cast<char*>(m.memory_begin) + m.memory_length},
                             "Hello World!");
        m.grow_by_memory_type(mt, 10);

        ::uwvm::vm::interpreter::memory::memory_t m2{m};  // copy
        ::uwvm::vm::interpreter::memory::memory_t m3{::std::move(m2)};  // move
    }
}
