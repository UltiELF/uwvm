#pragma once
#pragma once
#include <cmath>
#include <fast_io.h>
#include <io_device.h>
#include <back_trace.h>
#include <unfinished.h>

#include "../ast.h"
#include "../../global.h"
#include "../../memory/memory.h"
#include "../../../run/wasm_file.h"

namespace uwvm::vm::interpreter::func
{
    namespace fcext
    {
#if __has_cpp_attribute(__gnu__::__hot__)
        [[__gnu__::__hot__]]
#endif
        inline void
            memory_copy(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
        {
            if(sm.stack.size() <  3u) [[unlikely]]
            {
                ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - global_wasm_module.module_begin),
                                u8") "
                                u8"The data stack is empty."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::uwvm::vm::interpreter::int_bt(sm);
                ::fast_io::fast_terminate();
            }

            auto const para_sz{sm.stack.pop_element_unchecked()};
            auto const para_src{sm.stack.pop_element_unchecked()};
            auto const para_dest{sm.stack.pop_element_unchecked()};

            if(para_sz.vt != ::uwvm::wasm::value_type::i32 || para_src.vt != ::uwvm::wasm::value_type::i32 || para_dest.vt != ::uwvm::wasm::value_type::i32)
                [[unlikely]]
            {
                ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - global_wasm_module.module_begin),
                                u8") "
                                u8"The data type is not i32."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::uwvm::vm::interpreter::int_bt(sm);
                ::fast_io::fast_terminate();
            }

            auto const& memory{::uwvm::vm::memories.front_unchecked()};
            auto const begin{memory.memory_begin};
            auto const len{memory.memory_length};

            if(static_cast<::std::size_t>(para_src.i32) + static_cast<::std::size_t>(para_sz.i32) > len ||
               static_cast<::std::size_t>(para_dest.i32) + static_cast<::std::size_t>(para_sz.i32) > len) [[unlikely]]
            {
                ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - global_wasm_module.module_begin),
                                u8") "
                                u8"Memory out of bounds."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::uwvm::vm::interpreter::int_bt(sm);
                ::fast_io::fast_terminate();
            }

            ::fast_io::freestanding::my_memcpy(begin + static_cast<::std::size_t>(para_dest.i32),
                                               begin + static_cast<::std::size_t>(para_src.i32),
                                               static_cast<::std::size_t>(para_sz.i32));

            ++sm.curr_op;
        }

#if __has_cpp_attribute(__gnu__::__hot__)
        [[__gnu__::__hot__]]
#endif
        inline void
            memory_fill(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
        {
            if(sm.stack.size() <  3) [[unlikely]]
            {
                ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - global_wasm_module.module_begin),
                                u8") "
                                u8"The data stack is empty."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::uwvm::vm::interpreter::int_bt(sm);
                ::fast_io::fast_terminate();
            }

            auto const para_sz{sm.stack.pop_element_unchecked()};
            auto const para_bytev{sm.stack.pop_element_unchecked()};
            auto const para_dest{sm.stack.pop_element_unchecked()};

            if(para_sz.vt != ::uwvm::wasm::value_type::i32 || para_bytev.vt != ::uwvm::wasm::value_type::i32 || para_dest.vt != ::uwvm::wasm::value_type::i32)
                [[unlikely]]
            {
                ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - global_wasm_module.module_begin),
                                u8") "
                                u8"The data type is not i32."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::uwvm::vm::interpreter::int_bt(sm);
                ::fast_io::fast_terminate();
            }

            auto const& memory{::uwvm::vm::memories.front_unchecked()};
            auto const begin{memory.memory_begin};
            auto const len{memory.memory_length};

            if(static_cast<::std::size_t>(para_dest.i32) + static_cast<::std::size_t>(para_sz.i32) > len) [[unlikely]]
            {
                ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - global_wasm_module.module_begin),
                                u8") "
                                u8"Memory out of bounds."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::uwvm::vm::interpreter::int_bt(sm);
                ::fast_io::fast_terminate();
            }

            ::fast_io::freestanding::my_memset(begin + static_cast<::std::size_t>(para_dest.i32),
                                               static_cast<int>(para_bytev.i32),
                                               static_cast<::std::size_t>(para_sz.i32));

            ++sm.curr_op;
        }
    }  // namespace fcext
}  // namespace uwvm::vm::interpreter::func
