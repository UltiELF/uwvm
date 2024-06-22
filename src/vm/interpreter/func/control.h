#pragma once
#if defined(_MSC_VER) || defined(__MSDOS__)
    #include <stacktrace>
#else
    #include <libunwind.h>
#endif

#include <fast_io.h>
#include <io_device.h>

#include "../ast.h"
#include "../../../run/wasm_file.h"

namespace uwvm::vm::interpreter::func
{
#if __has_cpp_attribute(__gnu__::__cold__)
    [[__gnu__::__cold__]]
#endif
    inline void
        backtrace() noexcept
    {
#if defined(_MSC_VER) || defined(__MSDOS__)
        auto const bt{::std::stacktrace::current()};
        ::std::size_t counter{};
        for(auto const& i: bt) { ::fast_io::io::perr(::uwvm::u8err, u8"[", counter++, u8"] (", ::fast_io::mnp::code_cvt(i.description()), u8")\n"); }
        ::fast_io::io::perrln(::uwvm::u8err);
#else
        ::unw_cursor_t cursor{};
        ::unw_context_t context{};

        ::unw_getcontext(__builtin_addressof(context));
        ::unw_init_local(__builtin_addressof(cursor), __builtin_addressof(context));

        ::std::size_t counter{};
        while(::unw_step(__builtin_addressof(cursor)) > 0)
        {
            ::unw_word_t offset{};
            ::unw_word_t pc{};

            char8_t fname[1024];
            fname[0] = u8'\0';

            ::unw_get_reg(__builtin_addressof(cursor), UNW_REG_IP, __builtin_addressof(pc));

            ::unw_get_proc_name(__builtin_addressof(cursor), reinterpret_cast<char*>(fname), sizeof(fname), __builtin_addressof(offset));

            ::fast_io::io::perr(::uwvm::u8err,
                                u8"[",
                                counter++,
                                u8"] (",
                                ::fast_io::mnp::os_c_str(fname),
                                u8"+",
                                ::fast_io::mnp::addrvw(offset),
                                u8") [",
                                ::fast_io::mnp::addrvw(pc),
                                u8"]\n");
        }
        ::fast_io::io::perrln(::uwvm::u8err);
#endif
    }

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

        backtrace();
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

        if(sm.stack.empty()) [[unlikely]]
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
            backtrace();
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
        if(sm.stack.empty()) [[unlikely]]
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
            backtrace();
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
        ::uwvm::unfinished();
    }
}  // namespace uwvm::vm::interpreter::func
