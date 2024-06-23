#pragma once
#include <fast_io.h>
#include <io_device.h>
#include <back_trace.h>

#include "../ast.h"
#include "../../../run/wasm_file.h"

namespace uwvm::vm::interpreter::func
{
#if __has_cpp_attribute(__gnu__::__cold__)
    [[__gnu__::__cold__]]
#endif
    [[noreturn]] inline void
        unreachable(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
                                u8"Catch unreachable\n"
                                u8"\033[33m"
                                u8"[back trace] \n"
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                            );

        ::uwvm::backtrace();
        ::fast_io::fast_terminate();
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        if_(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
#if 0
        sm.flow.emplace(curr, ::uwvm::vm::interpreter::flow_control_t::if_);
#endif

        if(sm.stack.size() < sm.stack_top) [[unlikely]]
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
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        auto st{sm.stack.pop_element_unchecked()};
        if(st.i32) { ++sm.curr_op; }
        else
        {
            if(sm.curr_op->ext.branch) { sm.curr_op = sm.curr_op->ext.branch + 1; }
            else { sm.curr_op = sm.curr_op->ext.end + 1; /* next to end */ }
        }
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        br(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        sm.curr_op = sm.curr_op->ext.end + 1;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        br_if(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < sm.stack_top) [[unlikely]]
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
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        auto st{sm.stack.pop_element_unchecked()};
        if(st.i32) { sm.curr_op = sm.curr_op->ext.end + 1; }
        else { ++sm.curr_op; }
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        br_table(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < sm.stack_top) [[unlikely]]
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
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        auto st{sm.stack.pop_element_unchecked()};

        using vec_op_const_p_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::fast_io::vector<operator_t const*> const*;

        auto const& vec{*reinterpret_cast<vec_op_const_p_const_may_alias_ptr>(sm.curr_op->ext.branch)};
        if(vec.size() <= st.i32) [[unlikely]]
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
                                u8"br_table size <= index."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        sm.curr_op = vec.index_unchecked(st.i32)->ext.end + 1;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        return_(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        sm.curr_op = sm.end_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    extern void
        call(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept;
}  // namespace uwvm::vm::interpreter::func
