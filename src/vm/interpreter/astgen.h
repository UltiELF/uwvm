#pragma once
#include <fast_io.h>
#include <fast_io_dsal/stack.h>
#include "ast.h"
#include "func/func.h"

#include "../../run/features.h"
#include "../../wasm/opcode.h"
#include "../../run/wasm_file.h"

namespace uwvm::vm::interpreter
{
    namespace details
    {
        struct d_flow_t
        {
            ::std::byte const* flow_p{};
            ::uwvm::vm::interpreter::operator_t* op{};
            ::uwvm::vm::interpreter::flow_control_t flow_e{};
        };

        inline ::fast_io::stack<d_flow_t> ga_flow{};
    }  // namespace details

    inline constexpr ::uwvm::vm::interpreter::ast generate_ast(::uwvm::wasm::func_body const& fb) noexcept
    {
        auto const& wasmmod{::uwvm::global_wasm_module};

        auto const local_count{fb.local_count};
        auto const& locals{fb.locals};

        auto curr{fb.begin};
        auto const end{fb.end};

        ::uwvm::vm::interpreter::ast temp{};

        for(; curr < end;)
        {
            ::uwvm::wasm::op_basic ob{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(ob), curr, sizeof(ob));

            ::uwvm::vm::interpreter::operator_t op{};

            op.code_begin = curr;

            switch(ob)
            {
                case ::uwvm::wasm::op_basic::block:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::block);

                    ++curr;

                    ::uwvm::wasm::value_type vt{};
                    ::fast_io::freestanding::my_memcpy(__builtin_addressof(vt), curr, sizeof(vt));

                    switch(vt)
                    {
                        case ::uwvm::wasm::value_type::resulttype: [[fallthrough]];
                        case ::uwvm::wasm::value_type::i32: [[fallthrough]];
                        case ::uwvm::wasm::value_type::i64: [[fallthrough]];
                        case ::uwvm::wasm::value_type::f32: [[fallthrough]];
                        case ::uwvm::wasm::value_type::f64: break;
                        case ::uwvm::wasm::value_type::v128:
                        {
                            if(!::uwvm::features::enable_fixed_width_simd) [[unlikely]]
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
                                        u8"Enter parameter --enable-fixed-width-simd to enable wasm fixed-width simd."
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                            break;
                        }
                        case ::uwvm::wasm::value_type::externref: [[fallthrough]];
                        case ::uwvm::wasm::value_type::funcref:
                        {
                            if(!::uwvm::features::enable_reference_types) [[unlikely]]
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
                                        u8"Enter parameter --enable-reference-types to enable wasm reference types."
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                            break;
                        }
                        default:
                            [[unlikely]]
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
                                        u8"Invalid Value Type: ",
                                        ::fast_io::mnp::hex0x<true>(static_cast<::std::uint_fast8_t>(vt)),
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    auto& op_ebr{temp.operators.emplace_back(op)};

                    details::ga_flow.push({op.code_begin, __builtin_addressof(op_ebr), ::uwvm::vm::interpreter::flow_control_t::block});

                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::loop:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::loop);

                    ++curr;

                    ::uwvm::wasm::value_type vt{};
                    ::fast_io::freestanding::my_memcpy(__builtin_addressof(vt), curr, sizeof(vt));

                    switch(vt)
                    {
                        case ::uwvm::wasm::value_type::resulttype: [[fallthrough]];
                        case ::uwvm::wasm::value_type::i32: [[fallthrough]];
                        case ::uwvm::wasm::value_type::i64: [[fallthrough]];
                        case ::uwvm::wasm::value_type::f32: [[fallthrough]];
                        case ::uwvm::wasm::value_type::f64: break;
                        case ::uwvm::wasm::value_type::v128:
                        {
                            if(!::uwvm::features::enable_fixed_width_simd) [[unlikely]]
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
                                        u8"Enter parameter --enable-fixed-width-simd to enable wasm fixed-width simd."
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                            break;
                        }
                        case ::uwvm::wasm::value_type::externref: [[fallthrough]];
                        case ::uwvm::wasm::value_type::funcref:
                        {
                            if(!::uwvm::features::enable_reference_types) [[unlikely]]
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
                                        u8"Enter parameter --enable-reference-types to enable wasm reference types."
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                            break;
                        }
                        default:
                            [[unlikely]]
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
                                        u8"Invalid Value Type: ",
                                        ::fast_io::mnp::hex0x<true>(static_cast<::std::uint_fast8_t>(vt)),
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }
                    auto& op_ebr{temp.operators.emplace_back(op)};
                    details::ga_flow.push({op.code_begin, __builtin_addressof(op_ebr), ::uwvm::vm::interpreter::flow_control_t::loop});

                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::if_:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::if_);

                    ++curr;

                    ::uwvm::wasm::value_type vt{};
                    ::fast_io::freestanding::my_memcpy(__builtin_addressof(vt), curr, sizeof(vt));

                    switch(vt)
                    {
                        case ::uwvm::wasm::value_type::resulttype: [[fallthrough]];
                        case ::uwvm::wasm::value_type::i32: [[fallthrough]];
                        case ::uwvm::wasm::value_type::i64: [[fallthrough]];
                        case ::uwvm::wasm::value_type::f32: [[fallthrough]];
                        case ::uwvm::wasm::value_type::f64: break;
                        case ::uwvm::wasm::value_type::v128:
                        {
                            if(!::uwvm::features::enable_fixed_width_simd) [[unlikely]]
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
                                        u8"Enter parameter --enable-fixed-width-simd to enable wasm fixed-width simd."
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                            break;
                        }
                        case ::uwvm::wasm::value_type::externref: [[fallthrough]];
                        case ::uwvm::wasm::value_type::funcref:
                        {
                            if(!::uwvm::features::enable_reference_types) [[unlikely]]
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
                                        u8"Enter parameter --enable-reference-types to enable wasm reference types."
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                            break;
                        }
                        default:
                            [[unlikely]]
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
                                        u8"Invalid Value Type: ",
                                        ::fast_io::mnp::hex0x<true>(static_cast<::std::uint_fast8_t>(vt)),
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    auto& op_ebr{temp.operators.emplace_back(op)};
                    details::ga_flow.push({op.code_begin, __builtin_addressof(op_ebr), ::uwvm::vm::interpreter::flow_control_t::if_});

                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::else_:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::non);

                    if(details::ga_flow.empty()) [[unlikely]]
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
                                u8"Control flow is empty."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    auto const& f{details::ga_flow.top_unchecked()};
                    if(f.flow_e != ::uwvm::vm::interpreter::flow_control_t::if_) [[unlikely]]
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
                                u8"No if corrseponding to else."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    auto& op_ebr{temp.operators.emplace_back(op)};
                    f.op->ext.branch = __builtin_addressof(op_ebr);
                    details::ga_flow.push({op.code_begin, __builtin_addressof(op_ebr), ::uwvm::vm::interpreter::flow_control_t::else_});

                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::end:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::end);
                    if(details::ga_flow.empty()) [[unlikely]]
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
                                u8"Control flow is empty."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    auto& op_ebr{temp.operators.emplace_back(op)};

                    auto f{details::ga_flow.pop_element_unchecked()};
                    switch(f.flow_e)
                    {
                        case ::uwvm::vm::interpreter::flow_control_t::block: [[fallthrough]];
                        case ::uwvm::vm::interpreter::flow_control_t::loop: [[fallthrough]];
                        case ::uwvm::vm::interpreter::flow_control_t::if_: break;
                        case ::uwvm::vm::interpreter::flow_control_t::else_: // pop twice
                        {
                            auto f{details::ga_flow.pop_element_unchecked()}; // if
                            f.op->ext.end = __builtin_addressof(op_ebr);
                            break;
                        }
                        default:
                            [[unlikely]]
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
                                u8"Error control flow type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                                break;
                            }
                    }

                    f.op->ext.end = __builtin_addressof(op_ebr);

                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::try_:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::catch_:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::delegate_:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::catch_all:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::unreachable:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::br:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::br_if:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::br_table:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::return_:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::call:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::call_indirect:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::drop:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::local_get:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::local_set:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::local_tee:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::global_get:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::global_set:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::table_get:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::table_set:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::throw_:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::rethrow:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::ref_null:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::ref_is_null:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::select:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_load:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_load:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_load:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_load8_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_load8_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_load16_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_load16_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load8_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load8_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load16_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load16_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load32_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load32_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_store:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_store:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_store:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_store:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_store8:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_store16:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_store8:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_store16:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_store32:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::memory_size:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::memory_grow:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::nop:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_const:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_const:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_const:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_const:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_eqz:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_eq:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_ne:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_lt_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_lt_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_gt_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_gt_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_le_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_le_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_ge_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_ge_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_eqz:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_eq:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_ne:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_lt_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_lt_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_gt_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_gt_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_le_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_le_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_ge_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_ge_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_eq:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_ne:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_lt:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_gt:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_le:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_ge:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_eq:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_ne:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_lt:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_gt:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_le:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_ge:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_clz:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_ctz:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_popcnt:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_add:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_sub:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_mul:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_div_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_div_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_rem_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_rem_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_and:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_or:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_xor:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_shl:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_shr_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_shr_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_rotl:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_rotr:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_clz:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_ctz:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_popcnt:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_add:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_sub:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_mul:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_div_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_div_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_rem_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_rem_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_and:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_or:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_xor:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_shl:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_shr_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_shr_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_rotl:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_rotr:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_abs:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_neg:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_ceil:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_floor:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_trunc:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_nearest:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_sqrt:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_add:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_sub:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_mul:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_div:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_min:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_max:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_copysign:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_abs:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_neg:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_ceil:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_floor:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_trunc:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_nearest:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_sqrt:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_add:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_sub:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_mul:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_div:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_min:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_max:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_copysign:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_wrap_i64:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_trunc_f32_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_trunc_f32_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_trunc_f64_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_trunc_f64_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_extend_i32_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_extend_i32_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_trunc_f32_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_trunc_f32_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_trunc_f64_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_trunc_f64_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_convert_i32_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_convert_i32_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_convert_i64_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_convert_i64_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_demote_f64:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_convert_i32_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_convert_i32_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_convert_i64_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_convert_i64_u:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_promote_f32:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_reinterpret_f32:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_reinterpret_f64:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_reinterpret_i32:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_reinterpret_i64:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_extend8_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_extend16_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_extend8_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_extend16_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_extend32_s:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::ref_func:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::simd_interleaved:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::simd:
                {
                    break;
                }
                case ::uwvm::wasm::op_basic::reference_types:  // bulk_memory
                {
                    break;
                }
                default:
                    [[unlikely]]
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
                                u8"Invalid table length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
            }
        }

        if(!details::ga_flow.empty()) [[unlikely]]
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
                                u8"Control flow isnt empty."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }

        return temp;
    }
}  // namespace uwvm::vm::interpreter
