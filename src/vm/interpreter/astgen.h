﻿#pragma once
#include <fast_io.h>
#include <fast_io_dsal/stack.h>
#include <unfinished.h>
#include "ast.h"
#include "aststorge.h"
#include "global.h"
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
            ::fast_io::vector<::uwvm::vm::interpreter::operator_t*> brs{};
        };
    }  // namespace details
}  // namespace uwvm::vm::interpreter

namespace fast_io::freestanding
{
    template <>
    struct is_trivially_relocatable<::uwvm::vm::interpreter::details::d_flow_t>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_zero_default_constructible<::uwvm::vm::interpreter::details::d_flow_t>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding

namespace uwvm::vm::interpreter
{
    namespace details
    {
        extern thread_local ::fast_io::tlc::stack<d_flow_t, ::fast_io::tlc::vector<d_flow_t>> ga_flow;
    }  // namespace details

    // https://pengowray.github.io/wasm-ops/

    inline ::uwvm::vm::interpreter::ast generate_ast(::uwvm::wasm::function_type const* lft, ::uwvm::wasm::func_body const& fb) noexcept
    {
        // alias def
        using char8_t_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = char8_t*;
        using char8_t_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = char8_t const*;
        details::ga_flow.reserve(static_cast<::std::size_t>(2) * 1024);

        auto const& wasmmod{::uwvm::global_wasm_module};

        auto const local_count{fb.local_count};
        auto const& locals{fb.locals};

        auto const import_function_count{wasmmod.importsec.func_types.size()};
        auto const local_func_count{wasmmod.functionsec.function_count};
        auto const func_count{import_function_count + local_func_count};

        auto const import_global_count{wasmmod.importsec.global_types.size()};
        auto const local_global_count{wasmmod.globalsec.global_count};
        auto const global_count{import_global_count + local_global_count};

        auto curr{fb.begin};
        auto const end{fb.end};

        ::uwvm::vm::interpreter::ast temp{};

        // func type
        temp.ft = lft;

        // func body
        temp.fb = __builtin_addressof(fb);

        // locals
        ::std::size_t all_local_count{static_cast<::std::size_t>(lft->parameter_end - lft->parameter_begin)};
        for(auto const& i: fb.locals) { all_local_count += i.count; }
        temp.local_size = all_local_count;

        // ast
        temp.operators.reserve(static_cast<::std::size_t>(fb.end - fb.begin));

        // func flow
        details::ga_flow.push({curr, nullptr, ::uwvm::vm::interpreter::flow_control_t::func});

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

                    auto& op_ebr{temp.operators.emplace_back_unchecked(op)};

                    details::ga_flow.push({op.code_begin, __builtin_addressof(op_ebr), ::uwvm::vm::interpreter::flow_control_t::block});

                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::loop:
                {
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
                    auto& op_ebr{temp.operators.emplace_back_unchecked(op)};
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

                    auto& op_ebr{temp.operators.emplace_back_unchecked(op)};
                    details::ga_flow.push({op.code_begin, __builtin_addressof(op_ebr), ::uwvm::vm::interpreter::flow_control_t::if_});

                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::else_:
                {
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

                    auto& f{details::ga_flow.top_unchecked()};
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
                                u8"No if corresponding to else."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    f.flow_e = ::uwvm::vm::interpreter::flow_control_t::else_;

                    auto& op_ebr{temp.operators.emplace_back_unchecked(op)};

                    // set "if" branch
                    f.op->ext.branch = __builtin_addressof(op_ebr);
                    op_ebr.ext.branch = f.op;

                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::end:
                {
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

                    auto& op_ebr{temp.operators.emplace_back_unchecked(op)};

                    auto f{details::ga_flow.pop_element_unchecked()};
                    switch(f.flow_e)
                    {
                        case ::uwvm::vm::interpreter::flow_control_t::func:
                        {
                            ++curr;
                            if(curr != end) [[unlikely]]
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
                                    u8"The function body ends but current is not equal to end."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                            continue;
                        }
                        case ::uwvm::vm::interpreter::flow_control_t::block: [[fallthrough]];
                        case ::uwvm::vm::interpreter::flow_control_t::loop: [[fallthrough]];
                        case ::uwvm::vm::interpreter::flow_control_t::if_: break;
                        case ::uwvm::vm::interpreter::flow_control_t::else_:
                        {
                            // set "if" and "else" end_pointer to this op
                            const_cast<::uwvm::vm::interpreter::operator_t*>(f.op->ext.branch)->ext.end = __builtin_addressof(op_ebr);
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
                            }
                    }

                    for(auto const i: f.brs) { i->ext.end = __builtin_addressof(op_ebr); }

                    f.op->ext.end = __builtin_addressof(op_ebr);
                    op_ebr.ext.branch = f.op;

                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::try_:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::catch_:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::delegate_:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::catch_all:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::unreachable:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::unreachable);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::br:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::br);

                    ++curr;

                    ::std::size_t index{};
                    auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                    reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                    ::fast_io::mnp::leb128_get(index))};
                    switch(err)
                    {
                        case ::fast_io::parse_code::ok: break;
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

                    curr = reinterpret_cast<::std::byte const*>(next);

                    auto const gfsz{details::ga_flow.size()};

                    ++index;

                    if(index > gfsz) [[unlikely]]
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
                                u8"Invalid br index."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    auto& brop{temp.operators.emplace_back_unchecked(op)};

                    details::ga_flow.get_container().index_unchecked(gfsz - index).brs.push_back(__builtin_addressof(brop));

                    break;
                }
                case ::uwvm::wasm::op_basic::br_if:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::br_if);

                    ++curr;

                    ::std::size_t index{};
                    auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                    reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                    ::fast_io::mnp::leb128_get(index))};
                    switch(err)
                    {
                        case ::fast_io::parse_code::ok: break;
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

                    curr = reinterpret_cast<::std::byte const*>(next);

                    auto const gfsz{details::ga_flow.size()};

                    ++index;

                    if(index > gfsz) [[unlikely]]
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
                                u8"Invalid br index."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    auto& brop{temp.operators.emplace_back_unchecked(op)};

                    details::ga_flow.get_container().index_unchecked(gfsz - index).brs.push_back(__builtin_addressof(brop));

                    break;
                }
                case ::uwvm::wasm::op_basic::br_table:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::br_table);

                    ++curr;

                    ::std::size_t table_size{};
                    auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                    reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                    ::fast_io::mnp::leb128_get(table_size))};
                    switch(err)
                    {
                        case ::fast_io::parse_code::ok: break;
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

                    ++table_size;

                    curr = reinterpret_cast<::std::byte const*>(next);

                    auto const gfsz{details::ga_flow.size()};

                    auto& vec{::uwvm::vm::interpreter::stroage.ext.emplace_back(table_size)};

                    using operator_t_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
                        [[__gnu__::__may_alias__]]
#endif
                        = operator_t const*;

                    op.ext.branch =
                        reinterpret_cast<operator_t_const_may_alias_ptr>(const_cast<::fast_io::vector<operator_t*> const*>(__builtin_addressof(vec)));

                    for(::std::size_t i{}; i < table_size; ++i)
                    {
                        ::std::size_t index{};
                        auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(index))};
                        switch(err)
                        {
                            case ::fast_io::parse_code::ok: break;
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

                        ++index;

                        if(index > gfsz) [[unlikely]]
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
                                u8"Invalid br index."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                            ::fast_io::fast_terminate();
                        }

                        details::ga_flow.get_container().index_unchecked(gfsz - index).brs.push_back(vec.index_unchecked(i));

                        curr = reinterpret_cast<::std::byte const*>(next);
                    }

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::return_:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::return_);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::call:
                {
                    ++curr;
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::call);

                    ::std::size_t index{};
                    auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                    reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                    ::fast_io::mnp::leb128_get(index))};
                    switch(err)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                    op.ext.sz2 = index;

                    temp.operators.emplace_back_unchecked(op);
                    curr = reinterpret_cast<::std::byte const*>(next);

                    break;
                }
                case ::uwvm::wasm::op_basic::call_indirect:
                {
                    ++curr;
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::call_indirect);

                    ::std::size_t index{};
                    auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                    reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                    ::fast_io::mnp::leb128_get(index))};
                    switch(err)
                    {
                        case ::fast_io::parse_code::ok: break;
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

                    if(index >= wasmmod.typesec.type_count) [[unlikely]]
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
                                    u8"Invalid type index."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    using operator_t_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
                        [[__gnu__::__may_alias__]]
#endif
                        = operator_t const*;

                    op.ext.branch = reinterpret_cast<operator_t_const_may_alias_ptr>(wasmmod.typesec.types.cbegin() + index);

                    curr = reinterpret_cast<::std::byte const*>(next);

                    ::std::uint_fast8_t reserved{};
                    ::fast_io::freestanding::my_memcpy(__builtin_addressof(reserved), curr, sizeof(::std::uint_fast8_t));

                    if(reserved == 0)
                    {
                        // do nothing
                    }
#if 0  // feature 🦄
                    else if(reserved == 1)
                    {
                        // feature
                        ::uwvm::unfinished();
                    }
#endif
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
                                    u8"Invalid type index."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    temp.operators.emplace_back_unchecked(op);
                    ++curr;

                    break;
                }
                case ::uwvm::wasm::op_basic::drop:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::drop);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::local_get:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::local_get);

                    ++curr;

                    ::std::size_t index{};
                    auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                    reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                    ::fast_io::mnp::leb128_get(index))};
                    switch(err)
                    {
                        case ::fast_io::parse_code::ok: break;
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

                    if(index >= all_local_count) [[unlikely]]
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
                                    u8"Invalid local index."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.ext.sz2 = index;
                    temp.operators.emplace_back_unchecked(op);

                    curr = reinterpret_cast<::std::byte const*>(next);

                    break;
                }
                case ::uwvm::wasm::op_basic::local_set:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::local_set);

                    ++curr;

                    ::std::size_t index{};
                    auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                    reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                    ::fast_io::mnp::leb128_get(index))};
                    switch(err)
                    {
                        case ::fast_io::parse_code::ok: break;
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

                    if(index >= all_local_count) [[unlikely]]
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
                                    u8"Invalid local index."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.ext.sz2 = index;
                    temp.operators.emplace_back_unchecked(op);

                    curr = reinterpret_cast<::std::byte const*>(next);

                    break;
                }
                case ::uwvm::wasm::op_basic::local_tee:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::local_tee);

                    ++curr;

                    ::std::size_t index{};
                    auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                    reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                    ::fast_io::mnp::leb128_get(index))};
                    switch(err)
                    {
                        case ::fast_io::parse_code::ok: break;
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

                    if(index >= all_local_count) [[unlikely]]
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
                                    u8"Invalid local index."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.ext.sz2 = index;
                    temp.operators.emplace_back_unchecked(op);

                    curr = reinterpret_cast<::std::byte const*>(next);

                    break;
                }
                case ::uwvm::wasm::op_basic::global_get:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::global_get);

                    ++curr;

                    ::std::size_t index{};
                    auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                    reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                    ::fast_io::mnp::leb128_get(index))};
                    switch(err)
                    {
                        case ::fast_io::parse_code::ok: break;
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

                    if(index >= global_count) [[unlikely]]
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
                                    u8"Invalid global index."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    using operator_t_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
                        [[__gnu__::__may_alias__]]
#endif
                        = operator_t const*;

                    op.ext.branch = reinterpret_cast<operator_t_const_may_alias_ptr>(::uwvm::vm::interpreter::globals.globals + index);
                    temp.operators.emplace_back_unchecked(op);

                    curr = reinterpret_cast<::std::byte const*>(next);

                    break;
                }
                case ::uwvm::wasm::op_basic::global_set:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::global_set);

                    ++curr;

                    ::std::size_t index{};
                    auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                    reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                    ::fast_io::mnp::leb128_get(index))};
                    switch(err)
                    {
                        case ::fast_io::parse_code::ok: break;
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

                    if(index >= global_count) [[unlikely]]
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
                                    u8"Invalid global index."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    using operator_t_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
                        [[__gnu__::__may_alias__]]
#endif
                        = operator_t const*;

                    op.ext.branch = reinterpret_cast<operator_t_const_may_alias_ptr>(::uwvm::vm::interpreter::globals.globals + index);
                    temp.operators.emplace_back_unchecked(op);

                    curr = reinterpret_cast<::std::byte const*>(next);

                    break;
                }
                case ::uwvm::wasm::op_basic::table_get:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::table_set:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::throw_:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::rethrow:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::ref_null:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::ref_is_null:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::select:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::select);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::select_t:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_load:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_load);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_load);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::f32_load:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_load);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::f64_load:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_load);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i32_load8_s:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_load8_s);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i32_load8_u:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_load8_u);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i32_load16_s:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_load16_s);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i32_load16_u:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_load16_u);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load8_s:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_load8_s);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load8_u:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_load8_u);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load16_s:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_load16_s);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load16_u:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_load16_u);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load32_s:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_load32_s);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load32_u:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_load32_u);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i32_store:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_store);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i64_store:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_store);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::f32_store:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_store);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::f64_store:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_store);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i32_store8:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_store8);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i32_store16:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_store16);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i64_store8:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_store8);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i64_store16:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_store16);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::i64_store32:
                {
                    if(wasmmod.memorysec.memory_count == 0) [[unlikely]]
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
                                    u8"No wasm memory found."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_store32);

                    ++curr;

                    ::std::size_t alignment{};
                    auto const [next_a, err_a]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(alignment))};
                    switch(err_a)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_a);

                    ::std::size_t offset{};
                    auto const [next_o, err_o]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(offset))};
                    switch(err_o)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid size."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    curr = reinterpret_cast<::std::byte const*>(next_o);

                    op.ext.sz1 = alignment;
                    op.ext.sz2 = offset;

                    temp.operators.emplace_back_unchecked(op);

                    break;
                }
                case ::uwvm::wasm::op_basic::memory_size:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::memory_size);

                    ++curr;

                    ::std::size_t index{};
                    auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                    reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                    ::fast_io::mnp::leb128_get(index))};
                    switch(err)
                    {
                        case ::fast_io::parse_code::ok: break;
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

                    if(index >= ::uwvm::vm::interpreter::memories.size()) [[unlikely]]
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
                                    u8"Invalid memory index."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    using operator_t_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
                        [[__gnu__::__may_alias__]]
#endif
                        = operator_t const*;

                    op.ext.branch = reinterpret_cast<operator_t_const_may_alias_ptr>(::uwvm::vm::interpreter::memories.cbegin() + index);
                    temp.operators.emplace_back_unchecked(op);

                    curr = reinterpret_cast<::std::byte const*>(next);
                    break;
                }
                case ::uwvm::wasm::op_basic::memory_grow:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::memory_grow);

                    ++curr;

                    ::std::size_t index{};
                    auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                    reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                    ::fast_io::mnp::leb128_get(index))};
                    switch(err)
                    {
                        case ::fast_io::parse_code::ok: break;
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

                    if(index >= ::uwvm::vm::interpreter::memories.size()) [[unlikely]]
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
                                    u8"Invalid memory index."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    using operator_t_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
                        [[__gnu__::__may_alias__]]
#endif
                        = operator_t const*;

                    op.ext.branch = reinterpret_cast<operator_t_const_may_alias_ptr>(::uwvm::vm::interpreter::memories.cbegin() + index);

                    if(auto const imtsize{wasmmod.importsec.memory_types.size()}; index < imtsize)
                    {
                        op.ext.end = reinterpret_cast<operator_t_const_may_alias_ptr>(
                            __builtin_addressof(wasmmod.importsec.memory_types.index_unchecked(index)->extern_type.memory));
                    }
                    else { op.ext.end = reinterpret_cast<operator_t_const_may_alias_ptr>(wasmmod.memorysec.types.cbegin() + (index - imtsize)); }

                    temp.operators.emplace_back_unchecked(op);

                    curr = reinterpret_cast<::std::byte const*>(next);
                    break;
                }
                case ::uwvm::wasm::op_basic::nop:
                {
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_const:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_const);

                    ++curr;

                    ::std::int_least32_t ci32{};
                    auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                    reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                    ::fast_io::mnp::leb128_get(ci32))};
                    switch(err)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid i32."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    op.ext.i32 = ci32;

                    temp.operators.emplace_back_unchecked(op);

                    curr = reinterpret_cast<::std::byte const*>(next);
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_const:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_const);

                    ++curr;

                    ::std::int_least64_t ci64{};
                    auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                    reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                    ::fast_io::mnp::leb128_get(ci64))};
                    switch(err)
                    {
                        case ::fast_io::parse_code::ok: break;
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
                                    u8"Invalid i64."
                                    u8"\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    op.ext.i64 = ci64;

                    temp.operators.emplace_back_unchecked(op);

                    curr = reinterpret_cast<::std::byte const*>(next);
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_const:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_const);

                    ++curr;

                    ::std::uint_least32_t b4{};
                    ::fast_io::freestanding::my_memcpy(__builtin_addressof(b4), curr, sizeof(b4));
                    b4 = ::fast_io::little_endian(b4);

                    op.ext.f32 = ::std::bit_cast<::uwvm::wasm::wasm_f32>(b4);

                    temp.operators.emplace_back_unchecked(op);

                    curr += sizeof(b4);

                    break;
                }
                case ::uwvm::wasm::op_basic::f64_const:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_const);

                    ++curr;

                    ::std::uint_least64_t b8{};
                    ::fast_io::freestanding::my_memcpy(__builtin_addressof(b8), curr, sizeof(b8));
                    b8 = ::fast_io::little_endian(b8);

                    op.ext.f64 = ::std::bit_cast<::uwvm::wasm::wasm_f64>(b8);

                    temp.operators.emplace_back_unchecked(op);

                    curr += sizeof(b8);

                    break;
                }
                case ::uwvm::wasm::op_basic::i32_eqz:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_eqz);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_eq:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_eq);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_ne:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_ne);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_lt_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_lt_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_lt_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_lt_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_gt_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_gt_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_gt_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_gt_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_le_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_le_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_le_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_le_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_ge_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_ge_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_ge_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_ge_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_eqz:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_eqz);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_eq:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_eq);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_ne:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_ne);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_lt_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_lt_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_lt_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_lt_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_gt_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_gt_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_gt_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_gt_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_le_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_le_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_le_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_le_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_ge_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_ge_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_ge_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_ge_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_eq:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_eq);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_ne:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_ne);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_lt:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_lt);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_gt:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_gt);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_le:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_le);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_ge:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_ge);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_eq:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_eq);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_ne:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_ne);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_lt:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_lt);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_gt:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_gt);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_le:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_le);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_ge:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_ge);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_clz:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_clz);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_ctz:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_ctz);  // todo
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_popcnt:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_popcnt);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_add:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_add);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_sub:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_sub);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_mul:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_mul);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_div_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_div_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_div_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_div_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_rem_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_rem_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_rem_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_rem_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_and:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_and);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_or:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_or);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_xor:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_xor);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_shl:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_shl);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_shr_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_shr_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_shr_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_shr_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_rotl:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_rotl);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_rotr:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_rotr);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_clz:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_clz);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_ctz:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_ctz);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_popcnt:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_popcnt);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_add:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_add);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_sub:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_sub);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_mul:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_mul);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_div_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_div_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_div_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_div_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_rem_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_rem_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_rem_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_rem_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_and:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_and);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_or:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_or);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_xor:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_xor);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_shl:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_shl);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_shr_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_shr_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_shr_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_shr_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_rotl:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_rotl);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_rotr:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_rotr);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_abs:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_abs);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_neg:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_neg);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_ceil:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_ceil);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_floor:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_floor);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_trunc:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_trunc);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_nearest:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_nearest);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_sqrt:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_sqrt);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_add:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_add);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_sub:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_sub);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_mul:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_mul);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_div:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_div);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_min:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_min);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_max:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_max);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_copysign:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_copysign);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_abs:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_abs);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_neg:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_neg);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_ceil:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_ceil);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_floor:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_floor);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_trunc:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_trunc);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_nearest:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_nearest);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_sqrt:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_sqrt);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_add:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_add);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_sub:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_sub);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_mul:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_mul);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_div:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_div);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_min:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_min);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_max:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_max);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_copysign:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_copysign);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_wrap_i64:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_wrap_i64);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_trunc_f32_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_trunc_f32_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_trunc_f32_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_trunc_f32_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_trunc_f64_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_trunc_f64_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_trunc_f64_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_trunc_f64_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_extend_i32_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_extend_i32_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_extend_i32_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_extend_i32_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_trunc_f32_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_trunc_f32_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_trunc_f32_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_trunc_f32_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_trunc_f64_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_trunc_f64_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_trunc_f64_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_trunc_f64_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_convert_i32_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_convert_i32_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_convert_i32_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_convert_i32_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_convert_i64_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_convert_i64_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_convert_i64_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_convert_i64_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_demote_f64:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_demote_f64);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_convert_i32_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_convert_i32_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_convert_i32_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_convert_i32_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_convert_i64_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_convert_i64_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_convert_i64_u:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_convert_i64_u);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_promote_f32:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_promote_f32);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_reinterpret_f32:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_reinterpret_f32);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_reinterpret_f64:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_reinterpret_f64);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_reinterpret_i32:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f32_reinterpret_i32);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_reinterpret_i64:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::f64_reinterpret_i64);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_extend8_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_extend8_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_extend16_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i32_extend16_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_extend8_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_extend8_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_extend16_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_extend16_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_extend32_s:
                {
                    op.int_func = __builtin_addressof(::uwvm::vm::interpreter::func::i64_extend32_s);
                    temp.operators.emplace_back_unchecked(op);
                    ++curr;
                    break;
                }
                case ::uwvm::wasm::op_basic::ref_func:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::simd_interleaved:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::simd:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::reference_types:  // bulk_memory
                {
                    ::uwvm::unfinished();
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
