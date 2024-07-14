#pragma once
#include <back_trace.h>
#include "ast.h"
#include "aststorge.h"

namespace uwvm::vm::interpreter
{
    extern thread_local ::uwvm::vm::interpreter::stack_machine uwvm_sm;

    inline void run_ast(ast const& a) noexcept
    {
        if(uwvm_sm.stack.empty()) [[unlikely]]
        {
            // Just checking the stack is enough
            uwvm_sm.init();
        }

        if(!a.operators.empty()) [[likely]]
        {
            // storage last op
            auto const last_begin_op{uwvm_sm.begin_op};
            auto const last_curr_op{uwvm_sm.curr_op};
            auto const last_end_op{uwvm_sm.end_op};

            // prepare
            auto const begin_op{::std::to_address(a.operators.begin())};
            auto const end_op{::std::to_address(a.operators.end())};
            uwvm_sm.begin_op = begin_op;
            uwvm_sm.end_op = end_op;

            // check stack
            auto const& func_type{*a.ft};
            auto const func_type_para_size{static_cast<::std::size_t>(func_type.parameter_end - func_type.parameter_begin)};
            auto const func_type_result_size{static_cast<::std::size_t>(func_type.result_end - func_type.result_begin)};

            if(uwvm_sm.stack.size() < func_type_para_size) [[unlikely]]
            {
                ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
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
                ::uwvm::backtrace();
                ::fast_io::fast_terminate();
            }

            auto curr_st{uwvm_sm.stack.get_container().cend() - 1};
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
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
                --curr_st;
            }

            // local
            auto& local_storage_c{uwvm_sm.local_storages.get_container()};
            auto local_curr{local_storage_c.imp.curr_ptr};
            auto local_end{local_storage_c.imp.end_ptr};
            auto local_storage_size{static_cast<::std::size_t>(local_end - local_curr)};

            if(local_storage_size < a.local_size) [[unlikely]]
            {
                local_storage_c.reserve(local_storage_c.size() * 2);
                local_curr = local_storage_c.imp.curr_ptr;
                local_end = local_storage_c.imp.end_ptr;
                local_storage_size = static_cast<::std::size_t>(local_end - local_curr);
            }

            auto const last_local_top{uwvm_sm.local_top};
            uwvm_sm.local_top = static_cast<::std::size_t>(local_curr - local_storage_c.imp.begin_ptr);
            local_storage_c.imp.curr_ptr += a.local_size;

            auto local_curr_temp{local_curr};
            auto const stack_cend{uwvm_sm.stack.get_container().cend()};
            for(auto i{stack_cend - func_type_para_size}; i != stack_cend; ++i)
            {
                *local_curr_temp = *i;
                ++local_curr_temp;
            }

            // set local value_type
            for(auto const& i: a.fb->locals)
            {
                auto const it{i.type};
                for(::std::size_t j{}; j < i.count; ++j)
                {
                    local_curr_temp->vt = it;

                    switch(it)
                    {
                        case ::uwvm::wasm::value_type::i32:
                        {
                            local_curr_temp->i32 = decltype(local_curr_temp->i32){};
                            break;
                        }
                        case ::uwvm::wasm::value_type::i64:
                        {
                            local_curr_temp->i64 = decltype(local_curr_temp->i64){};
                            break;
                        }
                        case ::uwvm::wasm::value_type::f32:
                        {
                            local_curr_temp->f32 = decltype(local_curr_temp->f32){};
                            break;
                        }
                        case ::uwvm::wasm::value_type::f64:
                        {
                            local_curr_temp->f64 = decltype(local_curr_temp->f64){};
                            break;
                        }
                        case ::uwvm::wasm::value_type::v128:
                        {
                            local_curr_temp->v128 = decltype(local_curr_temp->v128){};
                            break;
                        }
                        case ::uwvm::wasm::value_type::funcref: [[fallthrough]];
                        case ::uwvm::wasm::value_type::externref:
                        {
                            local_curr_temp->ref = decltype(local_curr_temp->ref){};
                            break;
                        }
                        default:
                        {
                            ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
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
                            ::uwvm::backtrace();
                            ::fast_io::fast_terminate();
                        }
                    }

                    ++local_curr_temp;
                }
            }

#if 0
            // set stack curr
            uwvm_sm.stack.get_container().imp.curr_ptr -= func_type_para_size;
#endif

            // run
            for(uwvm_sm.curr_op = begin_op; uwvm_sm.curr_op != end_op;)
            {
#if 0
                if(uwvm_sm.curr_op->code_begin - global_wasm_module.module_begin == 0x000af6)
                {
                    ::fast_io::io::perr(::uwvm::u8out, u8"enter\n");
                    __debugbreak();
                }
                ::fast_io::io::perrln(::uwvm::u8out,
                                      ::fast_io::mnp::hex0x(static_cast<::std::size_t>(uwvm_sm.curr_op->code_begin - global_wasm_module.module_begin)),
                                      u8": ",
                                      uwvm_sm.stack.size());
#endif

                if(uwvm_sm.curr_op->int_func) [[likely]] { uwvm_sm.curr_op->int_func(uwvm_sm.curr_op->code_begin, uwvm_sm); }
                else { ++uwvm_sm.curr_op; }
            }

            // reset local point
            local_storage_c.imp.curr_ptr = local_curr;
            uwvm_sm.local_top = last_local_top;

            // check stack
            if(uwvm_sm.stack.size() < func_type_result_size) [[unlikely]]
            {
                ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
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
                ::uwvm::backtrace();
                ::fast_io::fast_terminate();
            }

            curr_st = uwvm_sm.stack.get_container().cend() - 1;
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
                    ::uwvm::backtrace();
                    ::fast_io::fast_terminate();
                }
                --curr_st;
            }

            // reset op
            uwvm_sm.begin_op = last_begin_op;
            uwvm_sm.curr_op = last_curr_op;
            uwvm_sm.end_op = last_end_op;
        }
    }
}  // namespace uwvm::vm::interpreter
