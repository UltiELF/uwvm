#pragma once
#pragma once
#include <cmath>
#include <fast_io.h>
#include <io_device.h>
#include <back_trace.h>
#include <unfinished.h>

#include "base.h"
#include "../ast.h"
#include "../../global.h"
#include "../../memory/memory.h"
#include "../../../run/wasm_file.h"

namespace uwvm::vm::unchecked_interpreter::func
{
    namespace fcext
    {
#if __has_cpp_attribute(__gnu__::__hot__)
        [[__gnu__::__hot__]]
#endif
        inline void
            memory_copy(::std::byte const* curr, ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
        {

            auto const para_sz{details::pop_get_val(stack_curr)};
            auto const para_src{details::pop_get_val(stack_curr)};
            auto const para_dest{details::pop_get_val(stack_curr)};


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
                ::fast_io::fast_terminate();
            }

            ::fast_io::freestanding::my_memcpy(begin + static_cast<::std::size_t>(para_dest.i32),
                                               begin + static_cast<::std::size_t>(para_src.i32),
                                               static_cast<::std::size_t>(para_sz.i32));

            auto next_op{curr_opt + 1};
            next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
        }

#if __has_cpp_attribute(__gnu__::__hot__)
        [[__gnu__::__hot__]]
#endif
        inline void
            memory_fill(::std::byte const* curr, ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
        {
            auto const para_sz{details::pop_get_val(stack_curr)};
            auto const para_bytev{details::pop_get_val(stack_curr)};
            auto const para_dest{details::pop_get_val(stack_curr)};


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
                ::fast_io::fast_terminate();
            }

            ::fast_io::freestanding::my_memset(begin + static_cast<::std::size_t>(para_dest.i32),
                                               static_cast<int>(para_bytev.i32),
                                               static_cast<::std::size_t>(para_sz.i32));

            auto next_op{curr_opt + 1};
            next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
        }
    }  // namespace fcext
}  // namespace uwvm::vm::unchecked_interpreter::func
