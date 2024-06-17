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

    enum class flow_control_t : ::std::size_t
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

    struct stack_machine
    {
        ::fast_io::stack<stack_t> stack{};
        ::fast_io::stack<flow_t> flow{};
    };

    using int_func_t = void (*)(::std::byte const*, stack_machine&);

    struct operator_t
    {
        ::std::byte const* code_begin{};
        int_func_t int_func{};
    };

    struct ast
    {
        ::fast_io::deque<operator_t> operators{};
    };

}  // namespace uwvm::vm::interpreter
