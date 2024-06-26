#pragma once
#include <fast_io.h>
#include "ast.h"
#include "../../wasm/types/global.h"

namespace uwvm::vm::interpreter
{
    struct int_global_type
    {
        union value_t
        {
            ::uwvm::wasm::wasm_i32 i32;
            ::uwvm::wasm::wasm_i64 i64;
            ::uwvm::wasm::wasm_f32 f32;
            ::uwvm::wasm::wasm_f64 f64;
            ::uwvm::wasm::wasm_v128 v128;
        };

        value_t value{};
        ::uwvm::wasm::global_type gt{};
    };

    struct global_storage
    {
        using global_type = int_global_type;
        using Alloc = ::fast_io::native_typed_thread_local_allocator<global_type>;

        global_type* globals{};
        ::std::size_t global_size{};

        constexpr global_storage() noexcept = default;

        constexpr global_storage(::std::size_t sz) noexcept : global_size{sz} { globals = Alloc::allocate(sz); }

        constexpr void init(::std::size_t sz) noexcept
        {
            if(globals == nullptr) [[likely]]
            {
                global_size = sz;
                globals = Alloc::allocate(sz);
            }
        }

        constexpr global_storage(global_storage const& other) noexcept
        {
            global_size = other.global_size;
            globals = Alloc::allocate(global_size);
            ::fast_io::freestanding::non_overlapped_copy_n(other.globals, global_size, globals);
        }

        constexpr global_storage& operator= (global_storage const& other) noexcept
        {
            clean();
            global_size = other.global_size;
            globals = Alloc::allocate(global_size);
            ::fast_io::freestanding::non_overlapped_copy_n(other.globals, global_size, globals);
            return *this;
        }

        constexpr global_storage(global_storage&& other) noexcept
        {
            global_size = other.global_size;
            globals = other.globals;
            other.global_size = 0;
            other.globals = nullptr;
        }

        constexpr global_storage& operator= (global_storage&& other) noexcept
        {
            clean();
            global_size = other.global_size;
            globals = other.globals;
            other.global_size = 0;
            other.globals = nullptr;
            return *this;
        }

        constexpr void clean() noexcept
        {
            if(globals) [[likely]]
            {
                Alloc::deallocate_n(globals, global_size);
                global_size = 0;
                globals = nullptr;
            }
        }

        constexpr ~global_storage() { clean(); }
    };

    inline global_storage globals{};

}  // namespace uwvm::vm::interpreter

namespace fast_io::freestanding
{
    template <>
    struct is_trivially_relocatable<::uwvm::vm::interpreter::global_storage>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_zero_default_constructible<::uwvm::vm::interpreter::global_storage>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding
