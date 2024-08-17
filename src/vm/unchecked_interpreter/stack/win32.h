#pragma once
#include <bit>
#include <fast_io.h>
#include <io_device.h>
#include <instrinsic.h>

namespace uwvm::vm::unchecked_interpreter::stack
{
    struct stack_t
    {
        inline static ::std::size_t default_stack_size{static_cast<::std::size_t>(1024)};

        ::uwvm::vm::unchecked_interpreter::stack_t* storage{};
        ::std::byte* memory_begin{};

        stack_t(stack_t const&) = delete;
        stack_t& operator= (stack_t const&) = delete;

        stack_t() noexcept { init(); };

        void init() noexcept
        {
            auto const stack_size{default_stack_size * sizeof(::uwvm::vm::unchecked_interpreter::stack_t)};
            ::std::size_t total_pages_bytes{stack_size + static_cast<::std::size_t>(2 * 4096)};

            memory_begin =
                reinterpret_cast<::std::byte*>(::fast_io::win32::VirtualAlloc(nullptr, total_pages_bytes, 0x00002000 /*MEM_RESERVE*/, 0x01 /*PAGE_NOACCESS*/));
            if(memory_begin == nullptr) [[unlikely]] { ::fast_io::fast_terminate(); }

            ::std::byte* stroage_begin{memory_begin + 4096};

            using stack_t_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
                [[__gnu__::__may_alias__]]
#endif
                = ::uwvm::vm::unchecked_interpreter::stack_t*;

            storage = reinterpret_cast<stack_t_may_alias_ptr>(stroage_begin);

            auto const vamemory2{
                reinterpret_cast<::std::byte*>(::fast_io::win32::VirtualAlloc(stroage_begin, stack_size, 0x00001000 /*MEM_COMMIT*/, 0x04 /*PAGE_READWRITE*/))};
            if(vamemory2 != stroage_begin) [[unlikely]] { ::fast_io::fast_terminate(); }
        }

        void clear() noexcept
        {
            if(memory_begin) [[likely]]
            {
                if(!::fast_io::win32::VirtualFree(memory_begin, 0, 0x00008000 /*MEM_RELEASE*/)) [[unlikely]] { ::fast_io::fast_terminate(); }
                storage = nullptr;
                memory_begin = nullptr;
            }
        }

        ~stack_t() { clear(); }
    };

}  // namespace uwvm::vm::unchecked_interpreter::stack

namespace fast_io::freestanding
{
    struct is_trivially_relocatable<::uwvm::vm::unchecked_interpreter::stack::stack_t>
    {
        inline static constexpr bool value = true;
    };

    struct is_zero_default_constructible<::uwvm::vm::unchecked_interpreter::stack::stack_t>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding
