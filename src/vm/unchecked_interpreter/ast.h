#pragma once
#include <fast_io.h>
#include <fast_io_dsal/stack.h>
#include "../../wasm/module.h"
#include "../../run/wasm_file.h"
#include "stack_t.h"

namespace uwvm::vm::unchecked_interpreter
{
    enum class flow_control_t : ::std::uint_fast8_t
    {
        func,
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

    using int_func_t = void (*)(::std::byte const* curr,
                                operator_t const* curr_opt,
                                ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                                ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept;

    struct ext_t
    {
        union
        {
            // pointers
            struct
            {
                operator_t const* end;
                operator_t const* branch;
            };

            // size_t
            struct
            {
                ::std::size_t sz1;  // end
                ::std::size_t sz2;  // branch
            };

            // value
            ::uwvm::wasm::wasm_i32 i32;
            ::uwvm::wasm::wasm_i64 i64;
            ::uwvm::wasm::wasm_f32 f32;
            ::uwvm::wasm::wasm_f64 f64;
            ::uwvm::wasm::wasm_v128 v128;
        };
    };

    struct operator_t
    {
        ::std::byte const* code_begin{};
        int_func_t int_func{};
        ext_t ext{};  // extra info
    };

    struct ast
    {
        ::fast_io::vector<operator_t> operators{};
        ::uwvm::wasm::local_function_type const* ft{};
        ::uwvm::wasm::func_body const* fb{};
        ::std::size_t local_size{};
    };

}  // namespace uwvm::vm::unchecked_interpreter

namespace fast_io::freestanding
{
    template <>
    struct is_trivially_relocatable<::uwvm::vm::unchecked_interpreter::ast>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_zero_default_constructible<::uwvm::vm::unchecked_interpreter::ast>
    {
        inline static constexpr bool value = true;
    };
}  // namespace fast_io::freestanding
