#pragma once
#include "../import.h"
#include "../ast.h"
#include "../aststorge.h"

namespace uwvm::vm::unchecked_interpreter
{
    inline void call_import_func(::std::size_t index, ::uwvm::vm::unchecked_interpreter::stack_t*& stack_curr) noexcept
    {
        auto const imfun{::uwvm::global_wasm_module.importsec.func_types.index_unchecked(index)};

        auto const& func_type{*imfun->extern_type.function};
        auto const func_type_para_size{static_cast<::std::size_t>(func_type.parameter_end - func_type.parameter_begin)};
        auto const func_type_result_size{static_cast<::std::size_t>(func_type.result_end - func_type.result_begin)};


        auto const func{::uwvm::vm::unchecked_interpreter::imports.index_unchecked(index)};

        func(stack_curr - func_type_para_size, stack_curr);
    }
}  // namespace uwvm::vm::unchecked_interpreter
