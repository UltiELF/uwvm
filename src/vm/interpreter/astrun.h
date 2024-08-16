#pragma once
#include <prefetch.h>
#include "ast.h"
#include "aststorge.h"

namespace uwvm::vm::interpreter
{
#if !(defined(__wasi__) && !defined(UWVM_ENABLE_WASI_THREADS))
    inline thread_local ::uwvm::vm::interpreter::stack_machine uwvm_sm{};
    inline thread_local ::fast_io::tlc::stack<::uwvm::wasm::local_function_type const*, ::fast_io::tlc::vector<::uwvm::wasm::local_function_type const*>>
        int_call_stack{};
#else
    inline ::uwvm::vm::interpreter::stack_machine uwvm_sm{};
    inline ::fast_io::tlc::stack<::uwvm::wasm::local_function_type const*, ::fast_io::tlc::vector<::uwvm::wasm::local_function_type const*>> int_call_stack{};

#endif

    inline void run_ast(ast const& a) noexcept
    {
        auto& uwvm_sm_r{uwvm_sm};
        auto& int_call_stack_r{int_call_stack};
        if(uwvm_sm_r.stack.empty()) [[unlikely]]
        {
            // Just checking the stack is enough
            uwvm_sm_r.init();
            int_call_stack_r.reserve(1024);
        }

        ::uwvm::prefetch(uwvm_sm_r.stack.get_container().cbegin());
        ::uwvm::prefetch(uwvm_sm_r.local_storages.get_container().cbegin());

        if(!a.operators.empty()) [[likely]]
        {
            int_call_stack_r.emplace(a.ft);

            // storage last op
            auto const last_begin_op{uwvm_sm_r.begin_op};
            auto const last_curr_op{uwvm_sm_r.curr_op};
            auto const last_end_op{uwvm_sm_r.end_op};

            // prepare
            auto const begin_op{::std::to_address(a.operators.begin())};
            auto const end_op{::std::to_address(a.operators.end())};
            uwvm_sm_r.begin_op = begin_op;
            uwvm_sm_r.end_op = end_op;

            // check stack
            auto const& local_func_type{*a.ft};
            auto const& func_type{*local_func_type.func_type};
            auto const func_type_para_size{static_cast<::std::size_t>(func_type.parameter_end - func_type.parameter_begin)};
            auto const func_type_result_size{static_cast<::std::size_t>(func_type.result_end - func_type.result_begin)};

            if(uwvm_sm_r.stack.size() < func_type_para_size) [[unlikely]]
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
                ::uwvm::vm::interpreter::int_bt();
                ::fast_io::fast_terminate();
            }

            auto curr_st{uwvm_sm_r.stack.get_container().cend() - 1};
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
                --curr_st;
            }

            // local
            auto& local_storage_c{uwvm_sm_r.local_storages.get_container()};
            auto const last_local_size{local_storage_c.size()};
            auto local_curr{local_storage_c.imp.curr_ptr};
            auto local_end{local_storage_c.imp.end_ptr};
            auto local_storage_size{static_cast<::std::size_t>(local_end - local_curr)};

            if(local_storage_size < a.local_size) [[unlikely]]
            {
                local_storage_c.reserve(::std::max(local_storage_c.size() * 2, local_storage_c.size() + a.local_size));
                local_curr = local_storage_c.imp.curr_ptr;
                local_end = local_storage_c.imp.end_ptr;
                local_storage_size = static_cast<::std::size_t>(local_end - local_curr);
            }

            auto const last_local_top{uwvm_sm_r.local_top};
            uwvm_sm_r.local_top = static_cast<::std::size_t>(local_curr - local_storage_c.imp.begin_ptr);
            local_storage_c.imp.curr_ptr += a.local_size;

            auto local_curr_temp{local_curr};
            auto const stack_cend{uwvm_sm_r.stack.get_container().cend()};
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
                            ::uwvm::vm::interpreter::int_bt();
                            ::fast_io::fast_terminate();
                        }
                    }

                    ++local_curr_temp;
                }
            }

            ::uwvm::prefetch(local_curr);

            // set stack curr (Clear the parameter stack after writing the parameter stack to local)
            uwvm_sm_r.stack.get_container().imp.curr_ptr -= func_type_para_size;

            // run
            for(uwvm_sm_r.curr_op = begin_op; uwvm_sm_r.curr_op != end_op;)
            {
#if 0  // debug
                if(uwvm_sm_r.curr_op->code_begin - global_wasm_module.module_begin == 0x004225)
                {
                    ::fast_io::io::perr(::uwvm::u8out, u8"enter\n");
                    __debugbreak();
                }
                ::fast_io::io::perrln(::uwvm::u8out,
                                      ::fast_io::mnp::hex0x(static_cast<::std::size_t>(uwvm_sm_r.curr_op->code_begin - global_wasm_module.module_begin)),
                                      u8": ",
                                      uwvm_sm_r.stack.size());
#endif

                if(uwvm_sm_r.curr_op->int_func) [[likely]]
                {
                    // run
                    uwvm_sm_r.curr_op->int_func(uwvm_sm_r.curr_op->code_begin, uwvm_sm_r);
                }
                else { ++uwvm_sm_r.curr_op; }
            }

            // reset local point
            local_storage_c.imp.curr_ptr = local_storage_c.imp.begin_ptr + last_local_size;
            uwvm_sm_r.local_top = last_local_top;

            // check stack
            if(uwvm_sm_r.stack.size() < func_type_result_size) [[unlikely]]
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
                ::uwvm::vm::interpreter::int_bt();
                ::fast_io::fast_terminate();
            }

            curr_st = uwvm_sm_r.stack.get_container().cend() - 1;
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
                    ::uwvm::vm::interpreter::int_bt();
                    ::fast_io::fast_terminate();
                }
                --curr_st;
            }

            // reset op
            uwvm_sm_r.begin_op = last_begin_op;
            uwvm_sm_r.curr_op = last_curr_op;
            uwvm_sm_r.end_op = last_end_op;

            int_call_stack_r.pop();
        }
    }
}  // namespace uwvm::vm::interpreter
