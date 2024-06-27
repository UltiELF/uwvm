#include "mvp.h"
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
        auto& ast_temp{::uwvm::vm::interpreter::stroage.asts.index_unchecked(index)};
        if(ast_temp.operators.empty()) [[unlikely]]
        {
            ast_temp = ::uwvm::vm::interpreter::generate_ast(wasmmod.functionsec.types.index_unchecked(index).func_type,
                                                             wasmmod.codesec.bodies.index_unchecked(index));
        }
        ::uwvm::vm::interpreter::run_ast(ast_temp);
    }
    else
    {
        // to do
        ::uwvm::unfinished();
    }
    ++sm.curr_op;
}

#if __has_cpp_attribute(__gnu__::__hot__)
[[__gnu__::__hot__]]
#endif
void ::uwvm::vm::interpreter::func::call_indirect(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
{
    auto const& wasmmod{::uwvm::global_wasm_module};

    if(sm.stack.size() < sm.stack_top) [[unlikely]]
    {
        ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
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
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"The data type is not i32."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
        ::uwvm::backtrace();
        ::fast_io::fast_terminate();
    }

    using function_type_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
        [[__gnu__::__may_alias__]]
#endif
        = ::uwvm::wasm::function_type const*;

    auto const type_p{reinterpret_cast<function_type_const_may_alias_ptr>(sm.curr_op->ext.branch)};

    auto const import_function_count{wasmmod.importsec.func_types.size()};
    auto const local_func_count{wasmmod.functionsec.function_count};
    auto const func_count{import_function_count + local_func_count};

    if(auto const st_sz{static_cast<::std::size_t>(st.i32)}; st_sz < import_function_count)
    {
        // to do
        ::uwvm::unfinished();
    }
    else if(st_sz < func_count)
    {
        auto const index{st_sz - import_function_count};
        auto& ast_temp{::uwvm::vm::interpreter::stroage.asts.index_unchecked(index)};
        if(ast_temp.operators.empty()) [[unlikely]]
        {
            ast_temp = ::uwvm::vm::interpreter::generate_ast(wasmmod.functionsec.types.index_unchecked(index).func_type,
                                                             wasmmod.codesec.bodies.index_unchecked(index));
        }
        ::uwvm::vm::interpreter::run_ast(ast_temp);
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
        ::uwvm::backtrace();
        ::fast_io::fast_terminate();
    }
    ++sm.curr_op;
}
