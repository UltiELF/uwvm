#pragma once
#include <fast_io.h>
#include <io_device.h>
#include "../../src/vm/interpreter/memory/memory.h"

namespace uwvm::test
{
    inline void test_memory() noexcept {
        ::fast_io::perr(::uwvm::u8out, u8"Test: Memory\n");

        ::uwvm::wasm::memory_type mt{
            .limits{.min{10}, .max{1000}, .present_max{true}}
        };
        ::uwvm::vm::interpreter::memory::memory_t m{};

        ::fast_io::perr(::uwvm::u8out, u8"init\n");
        m.init_by_memory_type(mt);

        ::fast_io::perr(::uwvm::u8out, u8"print\n");
        ::fast_io::io::print(::fast_io::obuffer_view{reinterpret_cast<char*>(m.memory_begin), reinterpret_cast<char*>(m.memory_begin) + m.memory_length},
                             "Hello World!");

        ::fast_io::perr(::uwvm::u8out, u8"grow\n");
        m.grow_by_memory_type(mt, 10);

        ::fast_io::perr(::uwvm::u8out, u8"copy\n");
        ::uwvm::vm::interpreter::memory::memory_t m2{m};  // copy

        ::fast_io::perr(::uwvm::u8out, u8"move\n");
        ::uwvm::vm::interpreter::memory::memory_t m3{::std::move(m2)};  // move    

        ::fast_io::perrln(::uwvm::u8out, u8"\033[32mSuccessfully\033[0m\n");

    }
}
