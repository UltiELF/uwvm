#include "mvp.h"
#include "../astgen.h"
#include "../astrun.h"
#include "../../table.h"

#include "call_import.h"

#if __has_cpp_attribute(__gnu__::__hot__)
[[__gnu__::__hot__]]
#endif
void ::uwvm::vm::unchecked_interpreter::func::call(::std::byte const* curr,
                                                   ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                                                   ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                                                   ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
{
    auto const& wasmmod{::uwvm::global_wasm_module};

    auto const import_function_count{wasmmod.importsec.func_types.size()};
    auto const all_func_index{reinterpret_cast<::std::size_t>(curr_opt->ext.branch)};

#if 0  // debug
    ::std::size_t break_point{107};
    if(all_func_index == break_point) { __debugbreak(); }
#endif

    if(all_func_index >= import_function_count)
    {
        auto const index{all_func_index - import_function_count};
        auto& ast_temp{::uwvm::vm::unchecked_interpreter::stroage.asts.index_unchecked(index)};
        if(ast_temp.operators.empty()) [[unlikely]]
        {
            ast_temp =
                ::uwvm::vm::unchecked_interpreter::generate_ast(wasmmod.functionsec.types.begin() + index, wasmmod.codesec.bodies.index_unchecked(index));
        }
        ::uwvm::vm::unchecked_interpreter::run_ast(ast_temp, stack_curr);
    }
    else { ::uwvm::vm::unchecked_interpreter::call_import_func(all_func_index, stack_curr); }
    auto next_op{curr_opt + 1};
    next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
}

#if __has_cpp_attribute(__gnu__::__hot__)
[[__gnu__::__hot__]]
#endif
void ::uwvm::vm::unchecked_interpreter::func::call_indirect(::std::byte const* curr,
                                                            ::uwvm::vm::unchecked_interpreter::operator_t const* curr_opt,
                                                            ::uwvm::vm::unchecked_interpreter::stack_t* local_begin,
                                                            ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
{
    auto const& wasmmod{::uwvm::global_wasm_module};

    auto const import_function_count{wasmmod.importsec.func_types.size()};
    auto const local_func_count{wasmmod.functionsec.function_count};
    auto const func_count{import_function_count + local_func_count};

    auto st{details::pop_get_val(stack_curr)};

    using function_type_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
        [[__gnu__::__may_alias__]]
#endif
        = ::uwvm::wasm::function_type const*;

    auto const type_p{reinterpret_cast<function_type_const_may_alias_ptr>(curr_opt->ext.branch)};

    auto& table{::uwvm::vm::table::table_enum.front_unchecked()};
    auto const st_sz{static_cast<::std::size_t>(st.i32)};
    if(st_sz >= table.size()) [[unlikely]]
    {
        ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid table index."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
        ::fast_io::fast_terminate();
    }
    auto const func_index{table.index_unchecked(st_sz)};

    if(func_index < import_function_count)
    {
        if(type_p != wasmmod.importsec.func_types.index_unchecked(func_index)->extern_type.function) [[unlikely]]
        {
            ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Unmatched func type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
        ::uwvm::vm::unchecked_interpreter::call_import_func(func_index, stack_curr);
    }
    else if(func_index < func_count)
    {
        auto const index{func_index - import_function_count};
        auto const local_func_type{wasmmod.functionsec.types.begin() + index};
        if(type_p != local_func_type->func_type) [[unlikely]]
        {
            ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Unmatched func type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }

        auto& ast_temp{::uwvm::vm::unchecked_interpreter::stroage.asts.index_unchecked(index)};
        if(ast_temp.operators.empty()) [[unlikely]]
        {
            ast_temp = ::uwvm::vm::unchecked_interpreter::generate_ast(local_func_type, wasmmod.codesec.bodies.index_unchecked(index));
        }
        ::uwvm::vm::unchecked_interpreter::run_ast(ast_temp, stack_curr);
    }
    else [[unlikely]]
    {
        ::fast_io::io::perr(::uwvm::u8err,
                                    u8"\033[0m"
#ifdef __MSDOS__
                                    u8"\033[37m"
#else
                                    u8"\033[97m"
#endif
                                    u8"uwvm: "
                                    u8"\033[31m"
                                    u8"[fatal] "
                                    u8"\033[0m"
#ifdef __MSDOS__
                                    u8"\033[37m"
#else
                                    u8"\033[97m"
#endif
                                    u8"(offset=",
                                    ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                    u8") "
                                    u8"Invalid func index."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
        ::fast_io::fast_terminate();
    }
    auto next_op{curr_opt + 1};
    next_op->int_func(next_op->code_begin, next_op, local_begin, stack_curr);
}
