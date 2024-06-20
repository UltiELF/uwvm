#pragma once
#include <fast_io.h>
#include <fast_io_dsal/deque.h>
#include <fast_io_dsal/stack.h>
#include "../../wasm/module.h"

namespace uwvm::vm::interpreter
{
    union stack_t
    {
        ::uwvm::wasm::wasm_i32 i32;
        ::uwvm::wasm::wasm_i64 i64;
        ::uwvm::wasm::wasm_f32 f32;
        ::uwvm::wasm::wasm_f64 f64;
        ::uwvm::wasm::wasm_v128 v128;
    };

    enum class flow_control_t : ::std::uint_fast8_t
    {
        block,
        loop,
        if_,
        else_,
        end,
        try_,
        catch_,
        delegate_,
        catch_all,
    };

    struct flow_t
    {
        ::std::byte const* flow_p{};
        flow_control_t flow_e{};
    };

    struct operator_t;

    struct stack_machine
    {
        ::fast_io::tlc::stack<stack_t, ::fast_io::tlc::vector<stack_t>> stack{};
        ::fast_io::tlc::stack<flow_t, ::fast_io::tlc::vector<flow_t>> flow{};
        operator_t const* curr_op{};

        constexpr stack_machine() noexcept 
        {
            stack.reserve(::uwvm::default_int_stack_size);
            flow.reserve(static_cast<::std::size_t>(2) * 1024);
        }
    };

    using int_func_t = void (*)(::std::byte const*, stack_machine&);

    struct ext_t
    {
        operator_t const* end{};
        operator_t const* branch{};
    };

    struct operator_t
    {
        ::std::byte const* code_begin{};
        int_func_t int_func{};
        ext_t ext{};  // extra info
    };

    struct ast
    {
        ::fast_io::deque<operator_t> operators{};
    };

}  // namespace uwvm::vm::interpreter

namespace fast_io::freestanding
{
    template <>
    struct is_trivially_relocatable<::uwvm::vm::interpreter::stack_machine>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_zero_default_constructible<::uwvm::vm::interpreter::stack_machine>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_trivially_relocatable<::uwvm::vm::interpreter::ast>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_zero_default_constructible<::uwvm::vm::interpreter::ast>
    {
        inline static constexpr bool value = true;
    };
}  // namespace fast_io::freestanding
