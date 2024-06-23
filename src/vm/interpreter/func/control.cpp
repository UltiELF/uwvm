#include "control.h"
#include "../astgen.h"
#include "../astrun.h"

#if __has_cpp_attribute(__gnu__::__hot__)
[[__gnu__::__hot__]]
#endif
void ::uwvm::vm::interpreter::func::call(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
{

    auto const& wasmmod{::uwvm::global_wasm_module};

    auto const import_function_count{wasmmod.importsec.func_types.size()};
    auto const all_func_index{reinterpret_cast<::std::size_t>(sm.curr_op->ext.branch)};
    if(all_func_index >= import_function_count)
    {
        auto const index{all_func_index - import_function_count};
        auto& ast{::uwvm::vm::interpreter::stroage.asts.index_unchecked(index)};
        if(ast.operators.empty()) [[unlikely]]
        {
            ast = ::uwvm::vm::interpreter::generate_ast(wasmmod.functionsec.types.index_unchecked(index).func_type,
                                                        wasmmod.codesec.bodies.index_unchecked(index));
        }
        ::uwvm::vm::interpreter::run_ast(ast);
    }
    else
    {
        // to do
        ::uwvm::unfinished();
    }
}
