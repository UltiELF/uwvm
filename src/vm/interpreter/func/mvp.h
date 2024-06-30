#pragma once
#include <fast_io.h>
#include <io_device.h>
#include <back_trace.h>

#include "../ast.h"
#include "../global.h"
#include "../memory/memory.h"
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

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        if(st.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

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
        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        if(st.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        if(st.i32) { sm.curr_op = sm.curr_op->ext.end + 1; }
        else { ++sm.curr_op; }
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        br_table(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        if(st.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

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

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    extern void
        call_indirect(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept;

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        drop(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        sm.stack.pop_element_unchecked();

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        local_get(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const index{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};
        auto const& local{sm.local_storages.get_container().index_unchecked(sm.local_top + index)};

        sm.stack.push(local);

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        local_set(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const index{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};
        auto& local{sm.local_storages.get_container().index_unchecked(sm.local_top + index)};
        auto const local_type{local.vt};

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        if(st.vt != local_type) [[unlikely]]
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        local = st;
        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        local_tee(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const index{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};
        auto& local{sm.local_storages.get_container().index_unchecked(sm.local_top + index)};
        auto const local_type{local.vt};

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        auto const& st{sm.stack.top_unchecked()};

        if(st.vt != local_type) [[unlikely]]
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        local = st;
        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        global_get(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const& wasmmod{::uwvm::global_wasm_module};

        using int_global_type_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::uwvm::vm::interpreter::int_global_type const*;

        auto const& igt{*reinterpret_cast<int_global_type_const_may_alias_ptr>(sm.curr_op->ext.branch)};

        struct temp_t
        {
            ::uwvm::vm::interpreter::int_global_type::value_t value{};
            ::uwvm::wasm::value_type vt{};
        };

        sm.stack.push(::std::bit_cast<::uwvm::vm::interpreter::stack_t>(temp_t{.value{igt.value}, .vt{igt.gt.type}}));

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        global_set(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const& wasmmod{::uwvm::global_wasm_module};

        using int_global_type_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::uwvm::vm::interpreter::int_global_type const*;

        auto& igt{*const_cast<::uwvm::vm::interpreter::int_global_type*>(reinterpret_cast<int_global_type_const_may_alias_ptr>(sm.curr_op->ext.branch))};

        if(!igt.gt.is_mutable) [[unlikely]]
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
                                u8"Global is not mutable."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        auto const st{sm.stack.pop_element_unchecked()};

        if(st.vt != igt.gt.type) [[unlikely]]
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        struct temp_t
        {
            ::uwvm::vm::interpreter::int_global_type::value_t value{};
            ::uwvm::wasm::value_type vt{};
        };

        auto const local_gt{::std::bit_cast<temp_t>(st)};
        igt.value = local_gt.value;

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        select(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < sm.stack_top + 3) [[unlikely]]
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
                                u8"The number of data in the data stack is less than 3."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        auto const cond{sm.stack.pop_element_unchecked()};
        auto const i2{sm.stack.pop_element_unchecked()};

        if(cond.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        if(!cond.i32) { sm.stack.get_container().back_unchecked() = i2; }

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_load(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        ::std::uint_least32_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::int_least32_t>(::fast_io::little_endian(ml))};

        sm.stack.push_unchecked(stack_t{.i32{pl}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_load(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        ::std::uint_least64_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::int_least64_t>(::fast_io::little_endian(ml))};

        sm.stack.push_unchecked(stack_t{.i64{pl}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_load(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        ::std::uint_least32_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void*>(mem), sizeof(ml));
        auto const pl{::std::bit_cast<::uwvm::wasm::wasm_f32>(::fast_io::little_endian(ml))};

        sm.stack.push_unchecked(stack_t{.f32{pl}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_load(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        ::std::uint_least64_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void*>(mem), sizeof(ml));
        auto const pl{::std::bit_cast<::uwvm::wasm::wasm_f64>(::fast_io::little_endian(ml))};

        sm.stack.push_unchecked(stack_t{.f64{pl}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_load8_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        ::std::uint_least8_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::int_least8_t>(::fast_io::little_endian(ml))};

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(pl)}, .vt{::uwvm::wasm::value_type::i32}});
        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_load8_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        ::std::uint_least8_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::uint_least8_t>(::fast_io::little_endian(ml))};

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(pl)}, .vt{::uwvm::wasm::value_type::i32}});
        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_load16_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        ::std::uint_least16_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::int_least16_t>(::fast_io::little_endian(ml))};

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(pl)}, .vt{::uwvm::wasm::value_type::i32}});
        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_load16_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        ::std::uint_least16_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::uint_least16_t>(::fast_io::little_endian(ml))};

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(pl)}, .vt{::uwvm::wasm::value_type::i32}});
        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_load8_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        ::std::uint_least8_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::int_least8_t>(::fast_io::little_endian(ml))};

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(pl)}, .vt{::uwvm::wasm::value_type::i64}});
        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_load8_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        ::std::uint_least8_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::uint_least8_t>(::fast_io::little_endian(ml))};

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(pl)}, .vt{::uwvm::wasm::value_type::i64}});
        ++sm.curr_op;
    }
#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_load16_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        ::std::uint_least16_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::int_least16_t>(::fast_io::little_endian(ml))};

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(pl)}, .vt{::uwvm::wasm::value_type::i64}});
        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_load16_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        ::std::uint_least16_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::uint_least16_t>(::fast_io::little_endian(ml))};

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(pl)}, .vt{::uwvm::wasm::value_type::i64}});
        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_load32_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        ::std::uint_least32_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::int_least32_t>(::fast_io::little_endian(ml))};

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(pl)}, .vt{::uwvm::wasm::value_type::i64}});
        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_load32_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() <= sm.stack_top) [[unlikely]]
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

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        ::std::uint_least32_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::uint_least32_t>(::fast_io::little_endian(ml))};

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(pl)}, .vt{::uwvm::wasm::value_type::i64}});
        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_store(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() < sm.stack_top + 2) [[unlikely]]
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

        auto const num{sm.stack.pop_element_unchecked()};

        if(num.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        auto const ml{::fast_io::little_endian(static_cast<::std::uint_least32_t>(num.i32))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_store(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() < sm.stack_top + 2) [[unlikely]]
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

        auto const num{sm.stack.pop_element_unchecked()};

        if(num.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
                                u8"The data type is not i64."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        auto const ml{::fast_io::little_endian(static_cast<::std::uint_least64_t>(num.i64))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_store(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() < sm.stack_top + 2) [[unlikely]]
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

        auto const num{sm.stack.pop_element_unchecked()};

        if(num.vt != ::uwvm::wasm::value_type::f32) [[unlikely]]
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
                                u8"The data type is not f32."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        auto const ml{::fast_io::little_endian(::std::bit_cast<::std::uint_least32_t>(num.f32))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_store(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() < sm.stack_top + 2) [[unlikely]]
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

        auto const num{sm.stack.pop_element_unchecked()};

        if(num.vt != ::uwvm::wasm::value_type::f64) [[unlikely]]
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
                                u8"The data type is not f64."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        auto const ml{::fast_io::little_endian(::std::bit_cast<::std::uint_least64_t>(num.f64))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_store8(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() < sm.stack_top + 2) [[unlikely]]
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

        auto const num{sm.stack.pop_element_unchecked()};

        if(num.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        auto const ml{::fast_io::little_endian(static_cast<::std::uint_least8_t>(static_cast<::std::uint_least32_t>(num.i32)))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_store16(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() < sm.stack_top + 2) [[unlikely]]
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

        auto const num{sm.stack.pop_element_unchecked()};

        if(num.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        auto const ml{::fast_io::little_endian(static_cast<::std::uint_least16_t>(static_cast<::std::uint_least32_t>(num.i32)))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_store8(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() < sm.stack_top + 2) [[unlikely]]
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

        auto const num{sm.stack.pop_element_unchecked()};

        if(num.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
                                u8"The data type is not i64."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        auto const ml{::fast_io::little_endian(static_cast<::std::uint_least8_t>(static_cast<::std::uint_least64_t>(num.i64)))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_store16(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() < sm.stack_top + 2) [[unlikely]]
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

        auto const num{sm.stack.pop_element_unchecked()};

        if(num.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
                                u8"The data type is not i64."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        auto const ml{::fast_io::little_endian(static_cast<::std::uint_least16_t>(static_cast<::std::uint_least64_t>(num.i64)))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_store32(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        auto const aligment{reinterpret_cast<::std::size_t>(sm.curr_op->ext.end)};
        auto const offset{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};

        if(sm.stack.size() < sm.stack_top + 2) [[unlikely]]
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

        auto const num{sm.stack.pop_element_unchecked()};

        if(num.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
                                u8"The data type is not i64."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::backtrace();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto mem{reinterpret_cast<::std::size_t>(::uwvm::vm::interpreter::memories.front_unchecked().memory_begin)};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
            {
                mem = static_cast<::std::size_t>(st.i32);
                break;
            }
            case ::uwvm::wasm::value_type::i64:
            {
                mem = static_cast<::std::size_t>(st.i64);
                break;
            }
            default:
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
                                u8"The data type not match local type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        auto const ml{::fast_io::little_endian(static_cast<::std::uint_least32_t>(static_cast<::std::uint_least64_t>(num.i64)))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        ++sm.curr_op;
    }

}  // namespace uwvm::vm::interpreter::func
