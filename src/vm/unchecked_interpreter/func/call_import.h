#pragma once
#include "../import.h"
#include "../ast.h"
#include "../aststorge.h"

namespace uwvm::vm::unchecked_interpreter
{
    inline void call_import_func(::std::byte const* code, ::uwvm::vm::unchecked_interpreter::operator_t const* curr,  ::uwvm::vm::unchecked_interpreter::stack_t* local_begin, ::uwvm::vm::unchecked_interpreter::stack_t* stack_curr) noexcept
    {
        auto const imfun{::uwvm::global_wasm_module.importsec.func_types.index_unchecked(index)};

        auto const& func_type{*imfun->extern_type.function};
        auto const func_type_para_size{static_cast<::std::size_t>(func_type.parameter_end - func_type.parameter_begin)};
        auto const func_type_result_size{static_cast<::std::size_t>(func_type.result_end - func_type.result_begin)};

        if(sm.stack.size() < func_type_para_size) [[unlikely]]
        {
            ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(reinterpret_cast<::std::byte const*>(func_type.parameter_begin) - global_wasm_module.module_begin),
                                u8") "
                                u8"The data stack is empty."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::unchecked_interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        auto curr_st{sm.stack.get_container().cend() - 1};
        for(auto curr{func_type.parameter_end - 1}; curr != func_type.parameter_begin - 1; --curr)
        {
            auto const curr_vt{*curr};
            auto const curr_st_vt{curr_st->vt};
            if(curr_vt != curr_st_vt) [[unlikely]]
            {
                ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(reinterpret_cast<::std::byte const*>(func_type.parameter_begin) - global_wasm_module.module_begin),
                                u8") "
                                u8"Value type not match."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::uwvm::vm::unchecked_interpreter::int_bt();
                ::fast_io::fast_terminate();
            }
            --curr_st;
        }

        auto const func{::uwvm::vm::unchecked_interpreter::imports.index_unchecked(index)};

        func(sm.stack.get_container().cend() - func_type_para_size, sm);

        // Func handles the return value on its own

        // check return value
        if(sm.stack.size() < func_type_result_size) [[unlikely]]
        {
            ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(reinterpret_cast<::std::byte const*>(func_type.parameter_begin) - global_wasm_module.module_begin),
                                u8") "
                                u8"The data stack is empty."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::uwvm::vm::unchecked_interpreter::int_bt();
            ::fast_io::fast_terminate();
        }

        curr_st = sm.stack.get_container().cend() - 1;
        for(auto curr{func_type.result_end - 1}; curr != func_type.result_begin - 1; --curr)
        {
            auto const curr_vt{*curr};
            auto const curr_st_vt{curr_st->vt};
            if(curr_vt != curr_st_vt) [[unlikely]]
            {
                ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(reinterpret_cast<::std::byte const*>(func_type.parameter_begin) - global_wasm_module.module_begin),
                                u8") "
                                u8"Value type not match."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::uwvm::vm::unchecked_interpreter::int_bt();
                ::fast_io::fast_terminate();
            }
            --curr_st;
        }
    }
}  // namespace uwvm::vm::unchecked_interpreter
