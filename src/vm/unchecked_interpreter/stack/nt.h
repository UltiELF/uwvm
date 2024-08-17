#pragma once
#include <bit>
#include <fast_io.h>
#include <io_device.h>
#include <instrinsic.h>
#include "../stack_t.h"

namespace uwvm::vm::unchecked_interpreter::stack
{
    template <::fast_io::nt_family family>
    struct nt_family_stack_t
    {
        inline static constexpr bool zw{family == ::fast_io::nt_family::zw};

        inline static ::std::size_t default_stack_size{static_cast<::std::size_t>(1024)};

        ::uwvm::vm::unchecked_interpreter::stack_t* storage{};
        ::std::byte* memory_begin{};

        nt_family_stack_t(nt_family_stack_t const&) = delete;
        nt_family_stack_t& operator= (nt_family_stack_t const&) = delete;

        nt_family_stack_t() noexcept { init(); };

        void init() noexcept
        {
            auto stack_size{default_stack_size * sizeof(::uwvm::vm::unchecked_interpreter::stack_t)};
            ::std::size_t total_pages_bytes{stack_size + static_cast<::std::size_t>(2 * 4096)};

            auto status{::fast_io::win32::nt::nt_allocate_virtual_memory<zw>(reinterpret_cast<void*>(-1),
                                                                             reinterpret_cast<void**>(__builtin_addressof(memory_begin)),
                                                                             0,
                                                                             __builtin_addressof(total_pages_bytes),
                                                                             0x00002000 /*MEM_RESERVE*/,
                                                                             0x01 /*PAGE_NOACCESS*/)};

            if(status) [[unlikely]] { ::fast_io::fast_terminate(); }

            ::std::byte* stroage_begin{memory_begin + 4096};

            using stack_t_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
                [[__gnu__::__may_alias__]]
#endif
                = ::uwvm::vm::unchecked_interpreter::stack_t*;

            storage = reinterpret_cast<stack_t_may_alias_ptr>(stroage_begin);

            status = ::fast_io::win32::nt::nt_allocate_virtual_memory<zw>(reinterpret_cast<void*>(-1),
                                                                          reinterpret_cast<void**>(__builtin_addressof(stroage_begin)),
                                                                          0,
                                                                          __builtin_addressof(stack_size),
                                                                          0x00001000 /*MEM_COMMIT*/,
                                                                          0x04 /*PAGE_READWRITE*/);

            if(status || stroage_begin != reinterpret_cast<::std::byte*>(storage)) [[unlikely]] { ::fast_io::fast_terminate(); }
        }

        void clear() noexcept
        {
            if(memory_begin) [[likely]]
            {
                ::std::byte* vfmemory{memory_begin};
                ::std::size_t free_type{0};
                if(::fast_io::win32::nt::nt_free_virtual_memory<zw>(reinterpret_cast<void*>(-1),
                                                                    reinterpret_cast<void**>(__builtin_addressof(vfmemory)),
                                                                    __builtin_addressof(free_type),
                                                                    0x00008000 /*MEM_RELEASE*/)) [[unlikely]]
                {
                    ::fast_io::fast_terminate();
                }
                storage = nullptr;
                memory_begin = nullptr;
            }
        }

        ~nt_family_stack_t() { clear(); }
    };

    using stack_t = nt_family_stack_t<::fast_io::nt_family::nt>;
}  // namespace uwvm::vm::unchecked_interpreter::stack

namespace fast_io::freestanding
{
    template <::fast_io::nt_family family>
    struct is_trivially_relocatable<::uwvm::vm::unchecked_interpreter::stack::nt_family_stack_t<family>>
    {
        inline static constexpr bool value = true;
    };

    template <::fast_io::nt_family family>
    struct is_zero_default_constructible<::uwvm::vm::unchecked_interpreter::stack::nt_family_stack_t<family>>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding
