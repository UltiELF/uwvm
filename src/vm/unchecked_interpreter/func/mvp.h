#pragma once
#include <cmath>
#include <fast_io.h>
#include <io_device.h>
#include <unfinished.h>

#include "../ast.h"
#include "../global.h"
#include "../stack_t.h"
#include "../../memory/memory.h"
#include "../../../run/wasm_file.h"

namespace uwvm::vm::unchecked_interpreter::func
{
    namespace details
    {
#if __has_cpp_attribute(__gnu__::__always_inline__)
        [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
        [[msvc::forceinline]]
#endif
        inline void
            pop(::uwvm::vm::unchecked_interpreter::stack_t*& stack_curr) noexcept
        {
            --stack_curr;
        }

#if __has_cpp_attribute(__gnu__::__always_inline__)
        [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
        [[msvc::forceinline]]
#endif
        inline ::uwvm::vm::unchecked_interpreter::stack_t
            pop_get_val(::uwvm::vm::unchecked_interpreter::stack_t*& stack_curr) noexcept
        {
            --stack_curr;
            return *stack_curr;
        }

#if __has_cpp_attribute(__gnu__::__always_inline__)
        [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
        [[msvc::forceinline]]
#endif
        inline void
            push(::uwvm::vm::unchecked_interpreter::stack_t*& stack_curr, ::uwvm::vm::unchecked_interpreter::stack_t val) noexcept
        {
            *stack_curr = val;
            ++stack_curr;
        }

#if __has_cpp_attribute(__gnu__::__always_inline__)
        [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
        [[msvc::forceinline]]
#endif
        inline ::uwvm::vm::unchecked_interpreter::stack_t&
            push_get_valref(::uwvm::vm::unchecked_interpreter::stack_t*& stack_curr, ::uwvm::vm::unchecked_interpreter::stack_t val) noexcept
        {
            *stack_curr = val;
            auto const ret{stack_curr};
            ++stack_curr;
            return *ret;
        }

#if __has_cpp_attribute(__gnu__::__always_inline__)
        [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
        [[msvc::forceinline]]
#endif
        inline ::uwvm::vm::unchecked_interpreter::stack_t&
            top(::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
        {
            return *(stack_curr - 1);
        }

    }  // namespace details

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        nop(::std::byte const* curr,
            ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
            ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
            ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__cold__)
    [[__gnu__::__cold__]]
#endif
    [[noreturn]] inline void
        unreachable(::std::byte const* curr,
                    ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                    ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                    ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
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

        ::fast_io::fast_terminate();
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        if_(::std::byte const* curr,
            ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
            ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
            ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const st_i32{details::pop_get_val(stack_curr).i32};

        if(st_i32)
        {
            auto next_op{curr_opt + 1};
            next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
        }
        else
        {
            if(curr_opt->ext.branch)
            {
                auto next_op{curr_opt->ext.branch + 1};
                next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
            }
            else
            {
                auto next_op{curr_opt->ext.end + 1};
                next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
            }
        }
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        br(::std::byte const* curr,
           ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
           ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
           ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto next_op{curr_opt->ext.end + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        br_if(::std::byte const* curr,
              ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
              ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
              ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const st_i32{details::pop_get_val(stack_curr).i32};

        if(st_i32)
        {
            auto next_op{curr_opt->ext.end + 1};
            next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
        }
        else
        {
            auto next_op{curr_opt + 1};
            next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
        }
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        br_table(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const st_i32{details::pop_get_val(stack_curr).i32};

        using vec_op_const_p_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::fast_io::vector<::uwvm::vm::unchecked_interpreter::operator_t> const*;

        auto const& vec{*reinterpret_cast<vec_op_const_p_const_may_alias_ptr>(curr_opt->ext.branch)};

        auto const jump_index{static_cast<::std::size_t>(st_i32)};

        if(vec.size() <= jump_index) [[unlikely]]
        {
            auto const& op{vec.back()};
            auto next_op{op.ext.end + 1};
            next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
        }
        else
        {
            auto const& op{vec.index_unchecked(jump_index)};
            auto next_op{op.ext.end + 1};
            next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
        }
    }

    // to do
#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        return_(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        return;
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    extern void
        call(::std::byte const* curr,
             ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
             ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
             ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept;

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    extern void
        call_indirect(::std::byte const* curr,
                      ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                      ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                      ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept;

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        drop(::std::byte const* curr,
             ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
             ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
             ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        details::pop(stack_curr);

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        local_get(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const index{curr_opt->ext.sz2};
        auto const local{local_begin + index};
        details::push(stack_curr, *local);

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        local_set(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const index{curr_opt->ext.sz2};
        auto local{local_begin + index};

        auto const st{details::pop_get_val(stack_curr)};

        *local = st;
        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

    // to do
#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        local_tee(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const index{curr_opt->ext.sz2};
        auto local{local_begin + index};

        auto const& st{details::top(stack_curr)};

        *local = st;
        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        global_get(::std::byte const* curr,
                   ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                   ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                   ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const& wasmmod{::uwvm::global_wasm_module};

        using int_global_type_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::uwvm::vm::unchecked_interpreter::int_global_type const*;

        auto const& igt{*reinterpret_cast<int_global_type_const_may_alias_ptr>(curr_opt->ext.branch)};

        details::push(stack_curr, ::std::bit_cast<::uwvm::vm::unchecked_interpreter::stack_t>(igt.value));

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        global_set(::std::byte const* curr,
                   ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                   ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                   ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const& wasmmod{::uwvm::global_wasm_module};

        using int_global_type_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::uwvm::vm::unchecked_interpreter::int_global_type const*;

        auto& igt{
            *const_cast<::uwvm::vm::unchecked_interpreter::int_global_type*>(reinterpret_cast<int_global_type_const_may_alias_ptr>(curr_opt->ext.branch))};

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

            ::fast_io::fast_terminate();
        }

        auto const st{details::pop_get_val(stack_curr)};

        auto const local_gt{::std::bit_cast<::uwvm::vm::unchecked_interpreter::int_global_type::value_t>(st)};
        igt.value = local_gt;

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        select(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const cond{details::pop_get_val(stack_curr)};
        auto const i2{details::pop_get_val(stack_curr)};

        if(!cond.i32) { details::top(stack_curr) = i2; }

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        select_t(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const cond{details::pop_get_val(stack_curr)};
        auto const i2{details::pop_get_val(stack_curr)};

        if(!cond.i32) { details::top(stack_curr) = i2; }

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_load(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        ::std::uint_least32_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::int_least32_t>(::fast_io::little_endian(ml))};

        details::push(stack_curr, stack_t{.i32{pl}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_load(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        ::std::uint_least64_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::int_least64_t>(::fast_io::little_endian(ml))};

        details::push(stack_curr, stack_t{.i64{pl}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_load(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        ::std::uint_least32_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
        auto const pl{::std::bit_cast<::uwvm::wasm::wasm_f32>(::fast_io::little_endian(ml))};

        details::push(stack_curr, stack_t{.f32{pl}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_load(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        ::std::uint_least64_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
        auto const pl{::std::bit_cast<::uwvm::wasm::wasm_f64>(::fast_io::little_endian(ml))};

        details::push(stack_curr, stack_t{.f64{pl}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_load8_s(::std::byte const* curr,
                    ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                    ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                    ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        ::std::uint_least8_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::int_least8_t>(::fast_io::little_endian(ml))};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(pl)}});
        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_load8_u(::std::byte const* curr,
                    ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                    ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                    ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        ::std::uint_least8_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::uint_least8_t>(::fast_io::little_endian(ml))};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(pl)}});
        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_load16_s(::std::byte const* curr,
                     ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                     ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                     ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        ::std::uint_least16_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::int_least16_t>(::fast_io::little_endian(ml))};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(pl)}});
        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_load16_u(::std::byte const* curr,
                     ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                     ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                     ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        ::std::uint_least16_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::uint_least16_t>(::fast_io::little_endian(ml))};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(pl)}});
        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_load8_s(::std::byte const* curr,
                    ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                    ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                    ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        ::std::uint_least8_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::int_least8_t>(::fast_io::little_endian(ml))};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(pl)}});
        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_load8_u(::std::byte const* curr,
                    ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                    ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                    ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        ::std::uint_least8_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::uint_least8_t>(::fast_io::little_endian(ml))};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(pl)}});
        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }
#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_load16_s(::std::byte const* curr,
                     ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                     ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                     ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        ::std::uint_least16_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::int_least16_t>(::fast_io::little_endian(ml))};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(pl)}});
        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_load16_u(::std::byte const* curr,
                     ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                     ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                     ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        ::std::uint_least16_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::uint_least16_t>(::fast_io::little_endian(ml))};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(pl)}});
        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_load32_s(::std::byte const* curr,
                     ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                     ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                     ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        ::std::uint_least32_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::int_least32_t>(::fast_io::little_endian(ml))};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(pl)}});
        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_load32_u(::std::byte const* curr,
                     ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                     ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                     ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        ::std::uint_least32_t ml{};

        ::fast_io::freestanding::my_memcpy(__builtin_addressof(ml), reinterpret_cast<void const*>(mem), sizeof(ml));
        auto const pl{static_cast<::std::uint_least32_t>(::fast_io::little_endian(ml))};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(pl)}});
        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_store(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const num{details::pop_get_val(stack_curr)};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        auto const ml{::fast_io::little_endian(static_cast<::std::uint_least32_t>(num.i32))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_store(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const num{details::pop_get_val(stack_curr)};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        auto const ml{::fast_io::little_endian(static_cast<::std::uint_least64_t>(num.i64))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_store(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const num{details::pop_get_val(stack_curr)};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        auto const ml{::fast_io::little_endian(::std::bit_cast<::std::uint_least32_t>(num.f32))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_store(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const num{details::pop_get_val(stack_curr)};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        auto const ml{::fast_io::little_endian(::std::bit_cast<::std::uint_least64_t>(num.f64))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_store8(::std::byte const* curr,
                   ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                   ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                   ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const num{details::pop_get_val(stack_curr)};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        auto const ml{::fast_io::little_endian(static_cast<::std::uint_least8_t>(static_cast<::std::uint_least32_t>(num.i32)))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_store16(::std::byte const* curr,
                    ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                    ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                    ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const num{details::pop_get_val(stack_curr)};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        auto const ml{::fast_io::little_endian(static_cast<::std::uint_least16_t>(static_cast<::std::uint_least32_t>(num.i32)))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_store8(::std::byte const* curr,
                   ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                   ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                   ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const num{details::pop_get_val(stack_curr)};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        auto const ml{::fast_io::little_endian(static_cast<::std::uint_least8_t>(static_cast<::std::uint_least64_t>(num.i64)))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_store16(::std::byte const* curr,
                    ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                    ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                    ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const num{details::pop_get_val(stack_curr)};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        auto const ml{::fast_io::little_endian(static_cast<::std::uint_least16_t>(static_cast<::std::uint_least64_t>(num.i64)))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_store32(::std::byte const* curr,
                    ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                    ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                    ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const aligment{curr_opt->ext.sz1};
        auto const offset{curr_opt->ext.sz2};

        auto const num{details::pop_get_val(stack_curr)};

        auto const st{details::pop_get_val(stack_curr)};

        auto& global_memory{::uwvm::vm::memories.front_unchecked()};
        auto mem{reinterpret_cast<::std::size_t>(global_memory.memory_begin)};
        auto const mem_end{mem + global_memory.memory_length};

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

            ::fast_io::fast_terminate();
        }

        auto const ml{::fast_io::little_endian(static_cast<::std::uint_least32_t>(static_cast<::std::uint_least64_t>(num.i64)))};

        ::fast_io::freestanding::my_memcpy(reinterpret_cast<void*>(mem), __builtin_addressof(ml), sizeof(ml));

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        memory_size(::std::byte const* curr,
                    ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                    ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                    ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        using memory_t_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::uwvm::vm::memory::memory_t const*;
        auto const mem{reinterpret_cast<memory_t_const_may_alias_ptr>(curr_opt->ext.branch)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(mem->get_page_size())}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        memory_grow(::std::byte const* curr,
                    ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                    ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                    ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        using memory_t_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::uwvm::vm::memory::memory_t*;
        auto mem{reinterpret_cast<memory_t_may_alias_ptr>(const_cast<::uwvm::vm::unchecked_interpreter::operator_t*>(curr_opt->ext.branch))};

        auto const num{details::pop_get_val(stack_curr)};

        // Due to the overcommit mechanism, failure situations are not considered

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(mem->get_page_size())}});

        using memory_type_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::uwvm::wasm::memory_type const*;

        mem->grow_by_memory_type(*reinterpret_cast<memory_type_const_may_alias_ptr>(curr_opt->ext.end), static_cast<::std::size_t>(num.i32));

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_const(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        details::push(stack_curr, stack_t{.i32{curr_opt->ext.i32}});
        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_const(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        details::push(stack_curr, stack_t{.i64{curr_opt->ext.i64}});
        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_const(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        details::push(stack_curr, stack_t{.f32{curr_opt->ext.f32}});
        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_const(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        details::push(stack_curr, stack_t{.f64{curr_opt->ext.f64}});
        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_eqz(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num.i32 == 0)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_eq(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 == num2.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_ne(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 != num2.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_lt_s(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 < num2.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_lt_u(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(
            stack_curr,
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num1.i32) < static_cast<::std::uint_least32_t>(num2.i32))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_gt_s(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 > num2.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_gt_u(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(
            stack_curr,
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num1.i32) > static_cast<::std::uint_least32_t>(num2.i32))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_le_s(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 <= num2.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_le_u(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(
            stack_curr,
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num1.i32) <= static_cast<::std::uint_least32_t>(num2.i32))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_ge_s(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 >= num2.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_ge_u(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(
            stack_curr,
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num1.i32) >= static_cast<::std::uint_least32_t>(num2.i32))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_eqz(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};
        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num.i64 == 0)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_eq(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i64 == num2.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_ne(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i64 != num2.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_lt_s(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i64 < num2.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_lt_u(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(
            stack_curr,
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least64_t>(num1.i64) < static_cast<::std::uint_least64_t>(num2.i64))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_gt_s(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i64 > num2.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_gt_u(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(
            stack_curr,
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least64_t>(num1.i64) > static_cast<::std::uint_least64_t>(num2.i64))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_le_s(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i64 <= num2.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_le_u(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(
            stack_curr,
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least64_t>(num1.i64) <= static_cast<::std::uint_least64_t>(num2.i64))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_ge_s(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i64 >= num2.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_ge_u(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(
            stack_curr,
            stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least64_t>(num1.i64) >= static_cast<::std::uint_least64_t>(num2.i64))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_eq(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.f32 == num2.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_ne(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.f32 != num2.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_lt(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.f32 < num2.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_gt(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.f32 > num2.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_le(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.f32 <= num2.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_ge(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.f32 >= num2.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_eq(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.f64 == num2.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_ne(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.f64 != num2.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_lt(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.f64 < num2.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_gt(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.f64 > num2.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_le(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.f64 <= num2.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_ge(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.f64 >= num2.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_clz(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(::std::countl_zero(static_cast<::std::uint_least32_t>(num.i32)))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_ctz(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr,
                      stack_t{
                          .i32{static_cast<::std::int_least32_t>(::std::countr_zero(static_cast<::std::uint_least32_t>(num.i32)))},
                      });

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_popcnt(::std::byte const* curr,
                   ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                   ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                   ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(::std::popcount(static_cast<::std::uint_least32_t>(num.i32)))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_add(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 + num2.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_sub(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 - num2.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_mul(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 * num2.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_div_s(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 / num2.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_div_u(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr,
                      stack_t{
                          .i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num1.i32) / static_cast<::std::uint_least32_t>(num2.i32))},
                      });

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_rem_s(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 % num2.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_rem_u(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr,
                      stack_t{
                          .i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num1.i32) % static_cast<::std::uint_least32_t>(num2.i32))},
                      });

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_and(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 & num2.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_or(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 | num2.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_xor(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 ^ num2.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_shl(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 << num2.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_shr_s(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num1.i32 >> num2.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_shr_u(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr,
                      stack_t{
                          .i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num1.i32) >> static_cast<::std::uint_least32_t>(num2.i32))},
                      });

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_rotl(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr,
                      stack_t{
                          .i32{static_cast<::std::int_least32_t>(::std::rotl(static_cast<::std::uint_least32_t>(num1.i32), static_cast<int>(num2.i32)))},
                      });

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_rotr(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr,
                      stack_t{
                          .i32{static_cast<::std::int_least32_t>(::std::rotr(static_cast<::std::uint_least32_t>(num1.i32), static_cast<int>(num2.i32)))},
                      });

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_clz(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr,
                      stack_t{
                          .i64{static_cast<::std::int_least64_t>(::std::countl_zero(static_cast<::std::uint_least64_t>(num.i64)))},
                      });

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_ctz(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr,
                      stack_t{
                          .i64{static_cast<::std::int_least64_t>(::std::countr_zero(static_cast<::std::uint_least64_t>(num.i64)))},
                      });

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_popcnt(::std::byte const* curr,
                   ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                   ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                   ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(::std::popcount(static_cast<::std::uint_least64_t>(num.i64)))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_add(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 + num2.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_sub(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 - num2.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_mul(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 * num2.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_div_s(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 / num2.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_div_u(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr,
                      stack_t{
                          .i64{static_cast<::std::int_least64_t>(static_cast<::std::uint_least64_t>(num1.i64) / static_cast<::std::uint_least64_t>(num2.i64))},
                      });

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_rem_s(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 % num2.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_rem_u(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr,
                      stack_t{
                          .i64{static_cast<::std::int_least64_t>(static_cast<::std::uint_least64_t>(num1.i64) % static_cast<::std::uint_least64_t>(num2.i64))},
                      });

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_and(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 & num2.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_or(::std::byte const* curr,
               ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
               ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
               ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 | num2.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_xor(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 ^ num2.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_shl(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 << num2.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_shr_s(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(num1.i64 >> num2.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_shr_u(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr,
                      stack_t{
                          .i64{static_cast<::std::int_least64_t>(static_cast<::std::uint_least64_t>(num1.i64) >> static_cast<::std::uint_least64_t>(num2.i64))},
                      });

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_rotl(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr,
                      stack_t{
                          .i64{static_cast<::std::int_least64_t>(::std::rotl(static_cast<::std::uint_least64_t>(num1.i64), static_cast<int>(num2.i64)))},
                      });

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_rotr(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr,
                      stack_t{
                          .i64{static_cast<::std::int_least64_t>(::std::rotr(static_cast<::std::uint_least64_t>(num1.i64), static_cast<int>(num2.i64)))},
                      });

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_abs(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{::std::abs(num.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_neg(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{-num.f32}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_ceil(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{::std::ceil(num.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_floor(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{::std::floor(num.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_trunc(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{::truncf(num.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_nearest(::std::byte const* curr,
                    ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                    ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                    ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{::roundf(num.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_sqrt(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{::std::sqrt(num.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_add(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{num1.f32 + num2.f32}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_sub(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{num1.f32 - num2.f32}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_mul(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{num1.f32 * num2.f32}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_div(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{num1.f32 / num2.f32}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_min(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{::std::min(num1.f32, num2.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_max(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{::std::max(num1.f32, num2.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_copysign(::std::byte const* curr,
                     ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                     ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                     ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{::copysignf(num1.f32, num2.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_abs(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{::std::abs(num.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_neg(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{-num.f64}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_ceil(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{::std::ceil(num.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_floor(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{::std::floor(num.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_trunc(::std::byte const* curr,
                  ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                  ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{::trunc(num.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_nearest(::std::byte const* curr,
                    ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                    ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                    ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{::round(num.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_sqrt(::std::byte const* curr,
                 ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                 ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                 ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{::std::sqrt(num.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_add(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{num1.f64 + num2.f64}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_sub(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{num1.f64 - num2.f64}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_mul(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{num1.f64 * num2.f64}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_div(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{num1.f64 / num2.f64}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_min(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{::std::min(num1.f64, num2.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_max(::std::byte const* curr,
                ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{::std::max(num1.f64, num2.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_copysign(::std::byte const* curr,
                     ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                     ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                     ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num2{details::pop_get_val(stack_curr)};
        auto const num1{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{::copysign(num1.f64, num2.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_wrap_i64(::std::byte const* curr,
                     ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                     ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                     ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_trunc_f32_s(::std::byte const* curr,
                        ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                        ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                        ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_trunc_f32_u(::std::byte const* curr,
                        ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                        ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                        ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num.f32))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_trunc_f64_s(::std::byte const* curr,
                        ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                        ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                        ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(num.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_trunc_f64_u(::std::byte const* curr,
                        ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                        ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                        ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::uint_least32_t>(num.f64))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_extend_i32_s(::std::byte const* curr,
                         ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                         ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                         ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(num.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_extend_i32_u(::std::byte const* curr,
                         ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                         ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                         ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(static_cast<::std::uint_least32_t>(num.i32))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_trunc_f32_s(::std::byte const* curr,
                        ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                        ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                        ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(num.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_trunc_f32_u(::std::byte const* curr,
                        ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                        ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                        ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(static_cast<::std::uint_least64_t>(num.f32))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_trunc_f64_s(::std::byte const* curr,
                        ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                        ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                        ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(num.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_trunc_f64_u(::std::byte const* curr,
                        ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                        ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                        ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(static_cast<::std::uint_least64_t>(num.f64))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_convert_i32_s(::std::byte const* curr,
                          ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                          ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                          ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{static_cast<::uwvm::wasm::wasm_f32>(num.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_convert_i32_u(::std::byte const* curr,
                          ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                          ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                          ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{static_cast<::uwvm::wasm::wasm_f32>(static_cast<::std::uint_least32_t>(num.i32))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_convert_i64_s(::std::byte const* curr,
                          ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                          ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                          ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{static_cast<::uwvm::wasm::wasm_f32>(num.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_convert_i64_u(::std::byte const* curr,
                          ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                          ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                          ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{static_cast<::uwvm::wasm::wasm_f32>(static_cast<::std::uint_least64_t>(num.i64))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_demote_f64(::std::byte const* curr,
                       ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                       ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                       ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{static_cast<::uwvm::wasm::wasm_f32>(num.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_convert_i32_s(::std::byte const* curr,
                          ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                          ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                          ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{static_cast<::uwvm::wasm::wasm_f64>(num.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_convert_i32_u(::std::byte const* curr,
                          ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                          ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                          ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{static_cast<::uwvm::wasm::wasm_f64>(static_cast<::std::uint_least32_t>(num.i32))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_convert_i64_s(::std::byte const* curr,
                          ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                          ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                          ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{static_cast<::uwvm::wasm::wasm_f64>(num.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_convert_i64_u(::std::byte const* curr,
                          ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                          ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                          ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{static_cast<::uwvm::wasm::wasm_f64>(static_cast<::std::uint_least64_t>(num.i64))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_promote_f32(::std::byte const* curr,
                        ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                        ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                        ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{static_cast<::uwvm::wasm::wasm_f64>(num.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_reinterpret_f32(::std::byte const* curr,
                            ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                            ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                            ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{::std::bit_cast<::std::int_least32_t>(num.f32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_reinterpret_f64(::std::byte const* curr,
                            ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                            ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                            ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{::std::bit_cast<::std::int_least64_t>(num.f64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f32_reinterpret_i32(::std::byte const* curr,
                            ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                            ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                            ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f32{::std::bit_cast<::uwvm::wasm::wasm_f32>(num.i32)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        f64_reinterpret_i64(::std::byte const* curr,
                            ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                            ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                            ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.f64{::std::bit_cast<::uwvm::wasm::wasm_f64>(num.i64)}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_extend8_s(::std::byte const* curr,
                      ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                      ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                      ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::int_least8_t>(num.i32))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i32_extend16_s(::std::byte const* curr,
                       ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                       ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                       ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i32{static_cast<::std::int_least32_t>(static_cast<::std::int_least16_t>(num.i32))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_extend8_s(::std::byte const* curr,
                      ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                      ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                      ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(static_cast<::std::int_least8_t>(num.i64))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_extend16_s(::std::byte const* curr,
                       ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                       ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                       ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(static_cast<::std::int_least16_t>(num.i64))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void
        i64_extend32_s(::std::byte const* curr,
                       ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                       ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                       ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {

        auto const num{details::pop_get_val(stack_curr)};

        details::push(stack_curr, stack_t{.i64{static_cast<::std::int_least64_t>(static_cast<::std::int_least32_t>(num.i64))}});

        auto next_op{curr_opt + 1};
        next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
    }

}  // namespace uwvm::vm::unchecked_interpreter::func
