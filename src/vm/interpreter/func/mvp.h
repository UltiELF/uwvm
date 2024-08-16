#pragma once
#include <cmath>
#include <fast_io.h>
#include <io_device.h>
#include <unfinished.h>

#include "../int_bt.h"
#include "../ast.h"
#include "../global.h"
#include "../memory/memory.h"
#include "../../../run/wasm_file.h"

namespace uwvm::vm::interpreter::func
{
#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        nop(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ++sm.curr_op ;
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
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");

        ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto st{sm.stack.pop_element_unchecked()};

        using vec_op_const_p_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::fast_io::vector<operator_t> const*;

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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const jump_index{static_cast<::std::size_t>(st.i32)};

        if(vec.size() <= jump_index) [[unlikely]]
        {
            auto const& op{vec.back()};
            sm.curr_op = op.ext.end + 1;
            return;
        }
        else
        {
            auto const& op{vec.index_unchecked(jump_index)};
            sm.curr_op = op.ext.end + 1;
        }
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
            ::uwvm::vm::interpreter::int_bt();
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
        auto const index{sm.curr_op->ext.sz2};
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
        auto const index{sm.curr_op->ext.sz2};
        auto& local{sm.local_storages.get_container().index_unchecked(sm.local_top + index)};
        auto const local_type{local.vt};

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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
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
        auto const index{sm.curr_op->ext.sz2};
        auto& local{sm.local_storages.get_container().index_unchecked(sm.local_top + index)};
        auto const local_type{local.vt};

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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
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
        if(sm.stack.size() < 3) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const cond{sm.stack.pop_element_unchecked()};
        auto const i2{sm.stack.pop_element_unchecked()};

        switch(i2.vt)
        {
            case ::uwvm::wasm::value_type::i32: [[fallthrough]];
            case ::uwvm::wasm::value_type::i64: [[fallthrough]];
            case ::uwvm::wasm::value_type::f32: [[fallthrough]];
            case ::uwvm::wasm::value_type::f64: [[fallthrough]];
            case ::uwvm::wasm::value_type::v128: break;

            case ::uwvm::wasm::value_type::externref: [[fallthrough]];
            case ::uwvm::wasm::value_type::funcref:
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
                                u8"The type is not number type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::uwvm::vm::interpreter::int_bt();
                ::fast_io::fast_terminate();
            }
            default: ::fast_io::unreachable();
        }

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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        if(!cond.i32) { sm.stack.get_container().back_unchecked() = i2; }

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        select_t(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 3) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

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
            ::uwvm::vm::interpreter::int_bt();
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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least32_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        ::std::uint_least32_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;
        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least64_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        ::std::uint_least64_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;
        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least32_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        ::std::uint_least32_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least64_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        ::std::uint_least64_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least8_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        ::std::uint_least8_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;
        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least8_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        ::std::uint_least8_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;
        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least16_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        ::std::uint_least16_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;
        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least16_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        ::std::uint_least16_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;
        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least8_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        ::std::uint_least8_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;
        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least8_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        ::std::uint_least8_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;
        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least16_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        ::std::uint_least16_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;
        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least16_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        ::std::uint_least16_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;
        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least32_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        ::std::uint_least32_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;
        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least32_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        ::std::uint_least32_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;
        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least32_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least64_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least32_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;

        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least64_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;
        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least8_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;
        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least16_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;
        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least8_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;
        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least16_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

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
        auto const aligment{sm.curr_op->ext.sz1};
        auto const offset{sm.curr_op->ext.sz2};

        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const st{sm.stack.pop_element_unchecked()};

        auto& global_memory{::uwvm::vm::interpreter::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

        switch(st.vt)
        {
            case ::uwvm::wasm::value_type::i32:
                [[likely]]
                {
                    mem += static_cast<::std::size_t>(st.i32);
                    break;
                }
            case ::uwvm::wasm::value_type::i64:
            {
                mem += static_cast<::std::size_t>(st.i64);
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
        }

        mem += offset;
        mem = ((mem + (static_cast<::std::size_t>(1) << aligment) - 1) >> aligment) << aligment;
        if(static_cast<::std::size_t>(mem_end - mem) < sizeof(::std::uint_least32_t) || mem > mem_end) [[unlikely]]
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
                                u8"Cross border access."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const ml{::fast_io::little_endian(static_cast<::std::uint_least32_t>(static_cast<::std::uint_least64_t>(num.i64)))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        memory_size(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        using memory_t_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::uwvm::vm::interpreter::memory::memory_t const*;
        auto const mem{reinterpret_cast<memory_t_const_may_alias_ptr>(sm.curr_op->ext.branch)};

        sm.stack.push(stack_t{.i32{static_cast<::std::int_least32_t>(mem->get_page_size())}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        memory_grow(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        using memory_t_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::uwvm::vm::interpreter::memory::memory_t*;
        auto mem{reinterpret_cast<memory_t_may_alias_ptr>(const_cast<::uwvm::vm::interpreter::operator_t*>(sm.curr_op->ext.branch))};

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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        // Due to the overcommit mechanism, failure situations are not considered

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(mem->get_page_size())}, .vt{::uwvm::wasm::value_type::i32}});

        using memory_type_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::uwvm::wasm::memory_type const*;

        mem->grow_by_memory_type(*reinterpret_cast<memory_type_const_may_alias_ptr>(sm.curr_op->ext.end), static_cast<::std::size_t>(num.i32));

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_const(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        sm.stack.push(stack_t{.i32{sm.curr_op->ext.i32}, .vt{::uwvm::wasm::value_type::i32}});
        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_const(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        sm.stack.push(stack_t{.i64{sm.curr_op->ext.i64}, .vt{::uwvm::wasm::value_type::i64}});
        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_const(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        sm.stack.push(stack_t{.f32{sm.curr_op->ext.f32}, .vt{::uwvm::wasm::value_type::f32}});
        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_const(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        sm.stack.push(stack_t{.f64{sm.curr_op->ext.f64}, .vt{::uwvm::wasm::value_type::f64}});
        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_eqz(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num.i32 == 0)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_eq(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 == num2.i32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_ne(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 != num2.i32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_lt_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 < num2.i32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_lt_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num1.i32) < static_cast<::std::uint_least32_t>(num2.i32))},
                    .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_gt_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 > num2.i32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_gt_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num1.i32) > static_cast<::std::uint_least32_t>(num2.i32))},
                    .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_le_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 <= num2.i32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_le_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num1.i32) <= static_cast<::std::uint_least32_t>(num2.i32))},
                    .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_ge_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 >= num2.i32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_ge_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num1.i32) >= static_cast<::std::uint_least32_t>(num2.i32))},
                    .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_eqz(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num.i64 == 0)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_eq(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i64 == num2.i64)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_ne(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i64 != num2.i64)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_lt_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i64 < num2.i64)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_lt_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least64_t>(num1.i64) < static_cast<::std::uint_least64_t>(num2.i64))},
                    .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_gt_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i64 > num2.i64)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_gt_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least64_t>(num1.i64) > static_cast<::std::uint_least64_t>(num2.i64))},
                    .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_le_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i64 <= num2.i64)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_le_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least64_t>(num1.i64) <= static_cast<::std::uint_least64_t>(num2.i64))},
                    .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_ge_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i64 >= num2.i64)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_ge_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least64_t>(num1.i64) >= static_cast<::std::uint_least64_t>(num2.i64))},
                    .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_eq(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f32 || num2.vt != ::uwvm::wasm::value_type::f32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.f32 == num2.f32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_ne(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f32 || num2.vt != ::uwvm::wasm::value_type::f32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.f32 != num2.f32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_lt(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f32 || num2.vt != ::uwvm::wasm::value_type::f32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.f32 < num2.f32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_gt(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f32 || num2.vt != ::uwvm::wasm::value_type::f32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.f32 > num2.f32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_le(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f32 || num2.vt != ::uwvm::wasm::value_type::f32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.f32 <= num2.f32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_ge(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f32 || num2.vt != ::uwvm::wasm::value_type::f32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.f32 >= num2.f32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_eq(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f64 || num2.vt != ::uwvm::wasm::value_type::f64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.f64 == num2.f64)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_ne(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f64 || num2.vt != ::uwvm::wasm::value_type::f64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.f64 != num2.f64)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_lt(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f64 || num2.vt != ::uwvm::wasm::value_type::f64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.f64 < num2.f64)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_gt(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f64 || num2.vt != ::uwvm::wasm::value_type::f64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.f64 > num2.f64)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_le(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f64 || num2.vt != ::uwvm::wasm::value_type::f64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.f64 <= num2.f64)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_ge(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f64 || num2.vt != ::uwvm::wasm::value_type::f64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.f64 >= num2.f64)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_clz(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(::std::countl_zero(static_cast<::std::uint_least32_t>(num.i32)))},
                                        .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_ctz(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(::std::countr_zero(static_cast<::std::uint_least32_t>(num.i32)))},
                                        .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_popcnt(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(::std::popcount(static_cast<::std::uint_least32_t>(num.i32)))}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_add(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 + num2.i32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_sub(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 - num2.i32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_mul(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 * num2.i32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_div_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 / num2.i32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_div_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num1.i32) / static_cast<::std::uint_least32_t>(num2.i32))},
                    .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_rem_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 % num2.i32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_rem_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num1.i32) % static_cast<::std::uint_least32_t>(num2.i32))},
                    .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_and(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 & num2.i32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_or(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 | num2.i32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_xor(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 ^ num2.i32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_shl(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 << num2.i32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_shr_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 >> num2.i32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_shr_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num1.i32) >> static_cast<::std::uint_least32_t>(num2.i32))},
                    .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_rotl(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(::std::rotl(static_cast<::std::uint_least32_t>(num1.i32), static_cast<int>(num2.i32)))},
                    .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_rotr(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i32 || num2.vt != ::uwvm::wasm::value_type::i32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(::std::rotr(static_cast<::std::uint_least32_t>(num1.i32), static_cast<int>(num2.i32)))},
                    .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_clz(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(::std::countl_zero(static_cast<::std::uint_least64_t>(num.i64)))},
                                        .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_ctz(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(::std::countr_zero(static_cast<::std::uint_least64_t>(num.i64)))},
                                        .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_popcnt(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i64{static_cast<::std::int_least64_t>(::std::popcount(static_cast<::std::uint_least64_t>(num.i64)))}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_add(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 + num2.i64)}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_sub(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 - num2.i64)}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_mul(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 * num2.i64)}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_div_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 / num2.i64)}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_div_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i64{static_cast<::std::int_least64_t>(static_cast<::std::uint_least64_t>(num1.i64) / static_cast<::std::uint_least64_t>(num2.i64))},
                    .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_rem_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 % num2.i64)}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_rem_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i64{static_cast<::std::int_least64_t>(static_cast<::std::uint_least64_t>(num1.i64) % static_cast<::std::uint_least64_t>(num2.i64))},
                    .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_and(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 & num2.i64)}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_or(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 | num2.i64)}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_xor(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 ^ num2.i64)}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_shl(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 << num2.i64)}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_shr_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 >> num2.i64)}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_shr_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i64{static_cast<::std::int_least64_t>(static_cast<::std::uint_least64_t>(num1.i64) >> static_cast<::std::uint_least64_t>(num2.i64))},
                    .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_rotl(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i64{static_cast<::std::int_least64_t>(::std::rotl(static_cast<::std::uint_least64_t>(num1.i64), static_cast<int>(num2.i64)))},
                    .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_rotr(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::i64 || num2.vt != ::uwvm::wasm::value_type::i64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i64{static_cast<::std::int_least64_t>(::std::rotr(static_cast<::std::uint_least64_t>(num1.i64), static_cast<int>(num2.i64)))},
                    .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_abs(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{::std::abs(num.f32)}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_neg(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{-num.f32}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_ceil(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{::std::ceil(num.f32)}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_floor(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{::std::floor(num.f32)}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_trunc(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{::truncf(num.f32)}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_nearest(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{::roundf(num.f32)}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_sqrt(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{::std::sqrt(num.f32)}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_add(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f32 || num2.vt != ::uwvm::wasm::value_type::f32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{num1.f32 + num2.f32}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_sub(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f32 || num2.vt != ::uwvm::wasm::value_type::f32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{num1.f32 - num2.f32}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_mul(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f32 || num2.vt != ::uwvm::wasm::value_type::f32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{num1.f32 * num2.f32}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_div(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f32 || num2.vt != ::uwvm::wasm::value_type::f32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{num1.f32 / num2.f32}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_min(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f32 || num2.vt != ::uwvm::wasm::value_type::f32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{::std::min(num1.f32, num2.f32)}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_max(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f32 || num2.vt != ::uwvm::wasm::value_type::f32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{::std::max(num1.f32, num2.f32)}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_copysign(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f32 || num2.vt != ::uwvm::wasm::value_type::f32) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{::copysignf(num1.f32, num2.f32)}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_abs(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{::std::abs(num.f64)}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_neg(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{-num.f64}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_ceil(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{::std::ceil(num.f64)}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_floor(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{::std::floor(num.f64)}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_trunc(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{::trunc(num.f64)}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_nearest(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{::round(num.f64)}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_sqrt(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{::std::sqrt(num.f64)}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_add(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f64 || num2.vt != ::uwvm::wasm::value_type::f64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{num1.f64 + num2.f64}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_sub(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f64 || num2.vt != ::uwvm::wasm::value_type::f64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{num1.f64 - num2.f64}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_mul(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f64 || num2.vt != ::uwvm::wasm::value_type::f64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{num1.f64 * num2.f64}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_div(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f64 || num2.vt != ::uwvm::wasm::value_type::f64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{num1.f64 / num2.f64}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_min(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f64 || num2.vt != ::uwvm::wasm::value_type::f64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{::std::min(num1.f64, num2.f64)}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_max(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f64 || num2.vt != ::uwvm::wasm::value_type::f64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{::std::max(num1.f64, num2.f64)}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_copysign(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        if(sm.stack.size() < 2) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto const num2{sm.stack.pop_element_unchecked()};
        auto const num1{sm.stack.pop_element_unchecked()};

        if(num1.vt != ::uwvm::wasm::value_type::f64 || num2.vt != ::uwvm::wasm::value_type::f64) [[unlikely]]
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{::copysign(num1.f64, num2.f64)}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_wrap_i64(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num.i64)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_trunc_f32_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num.f32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_trunc_f32_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num.f32))}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_trunc_f64_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{static_cast<::std::int_least32_t>(num.f64)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_trunc_f64_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num.f64))}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_extend_i32_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(num.i32)}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_extend_i32_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i64{static_cast<::std::int_least64_t>(static_cast<::std::uint_least32_t>(num.i32))}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_trunc_f32_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(num.f32)}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_trunc_f32_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i64{static_cast<::std::int_least64_t>(static_cast<::std::uint_least64_t>(num.f32))}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_trunc_f64_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i64{static_cast<::std::int_least64_t>(num.f64)}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_trunc_f64_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i64{static_cast<::std::int_least64_t>(static_cast<::std::uint_least64_t>(num.f64))}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_convert_i32_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{static_cast<::uwvm::wasm::wasm_f32>(num.i32)}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_convert_i32_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.f32{static_cast<::uwvm::wasm::wasm_f32>(static_cast<::std::uint_least32_t>(num.i32))}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_convert_i64_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{static_cast<::uwvm::wasm::wasm_f32>(num.i64)}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_convert_i64_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.f32{static_cast<::uwvm::wasm::wasm_f32>(static_cast<::std::uint_least64_t>(num.i64))}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_demote_f64(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{static_cast<::uwvm::wasm::wasm_f32>(num.f64)}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_convert_i32_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{static_cast<::uwvm::wasm::wasm_f64>(num.i32)}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_convert_i32_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.f64{static_cast<::uwvm::wasm::wasm_f64>(static_cast<::std::uint_least32_t>(num.i32))}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_convert_i64_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{static_cast<::uwvm::wasm::wasm_f64>(num.i64)}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_convert_i64_u(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.f64{static_cast<::uwvm::wasm::wasm_f64>(static_cast<::std::uint_least64_t>(num.i64))}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_promote_f32(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{static_cast<::uwvm::wasm::wasm_f64>(num.f32)}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_reinterpret_f32(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i32{::std::bit_cast<::std::int_least32_t>(num.f32)}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_reinterpret_f64(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.i64{::std::bit_cast<::std::int_least64_t>(num.f64)}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_reinterpret_i32(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f32{::std::bit_cast<::uwvm::wasm::wasm_f32>(num.i32)}, .vt{::uwvm::wasm::value_type::f32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_reinterpret_i64(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(stack_t{.f64{::std::bit_cast<::uwvm::wasm::wasm_f64>(num.i64)}, .vt{::uwvm::wasm::value_type::f64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_extend8_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::int_least8_t>(num.i32))}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_extend16_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::int_least16_t>(num.i32))}, .vt{::uwvm::wasm::value_type::i32}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_extend8_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i64{static_cast<::std::int_least64_t>(static_cast<::std::int_least8_t>(num.i64))}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_extend16_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i64{static_cast<::std::int_least64_t>(static_cast<::std::int_least16_t>(num.i64))}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_extend32_s(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
            ::uwvm::vm::interpreter::int_bt();
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
            ::uwvm::vm::interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        sm.stack.push_unchecked(
            stack_t{.i64{static_cast<::std::int_least64_t>(static_cast<::std::int_least32_t>(num.i64))}, .vt{::uwvm::wasm::value_type::i64}});

        ++sm.curr_op;
    }

}  // namespace uwvm::vm::interpreter::func
