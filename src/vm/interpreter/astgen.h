#pragma once
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
        };

        inline thread_local ::fast_io::tlc::stack<d_flow_t, ::fast_io::tlc::vector<d_flow_t>> ga_flow{};
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
        ::std::size_t all_local_count{};
        for(auto const& i: fb.locals) { all_local_count += i.count; }
        temp.local_size = all_local_count;

        // ast
        temp.operators.reserve(static_cast<::std::size_t>(fb.end - fb.begin));

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

                    op.ext.end = details::ga_flow.get_container().index_unchecked(gfsz - index).op->ext.end;

                    temp.operators.emplace_back_unchecked(op);

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

                    op.ext.end = details::ga_flow.get_container().index_unchecked(gfsz - index).op->ext.end;

                    temp.operators.emplace_back_unchecked(op);

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

                    ::fast_io::vector<operator_t const*> vo{};

                    vo.reserve(table_size);

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

                        vo.push_back_unchecked(details::ga_flow.get_container().index_unchecked(gfsz - index).op->ext.end);

                        curr = reinterpret_cast<::std::byte const*>(next);
                    }

                    auto const& vec{::uwvm::vm::interpreter::stroage.ext.emplace_back(::std::move(vo))};

                    using operator_t_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
                        [[__gnu__::__may_alias__]]
#endif
                        = operator_t const*;

                    op.ext.branch = reinterpret_cast<operator_t_const_may_alias_ptr>(__builtin_addressof(vec));

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
                    op.ext.branch = reinterpret_cast<operator_t const*>(index);

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

                    op.ext.branch = reinterpret_cast<operator_t const*>(index);
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

                    op.ext.branch = reinterpret_cast<operator_t const*>(index);
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

                    op.ext.branch = reinterpret_cast<operator_t const*>(index);
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
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_load:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_load:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_load:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_load8_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_load8_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_load16_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_load16_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load8_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load8_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load16_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load16_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load32_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_load32_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_store:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_store:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_store:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_store:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_store8:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_store16:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_store8:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_store16:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_store32:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::memory_size:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::memory_grow:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::nop:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_const:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_const:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_const:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_const:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_eqz:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_eq:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_ne:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_lt_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_lt_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_gt_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_gt_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_le_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_le_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_ge_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_ge_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_eqz:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_eq:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_ne:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_lt_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_lt_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_gt_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_gt_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_le_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_le_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_ge_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_ge_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_eq:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_ne:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_lt:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_gt:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_le:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_ge:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_eq:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_ne:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_lt:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_gt:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_le:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_ge:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_clz:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_ctz:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_popcnt:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_add:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_sub:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_mul:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_div_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_div_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_rem_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_rem_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_and:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_or:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_xor:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_shl:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_shr_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_shr_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_rotl:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_rotr:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_clz:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_ctz:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_popcnt:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_add:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_sub:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_mul:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_div_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_div_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_rem_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_rem_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_and:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_or:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_xor:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_shl:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_shr_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_shr_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_rotl:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_rotr:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_abs:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_neg:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_ceil:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_floor:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_trunc:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_nearest:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_sqrt:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_add:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_sub:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_mul:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_div:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_min:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_max:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_copysign:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_abs:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_neg:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_ceil:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_floor:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_trunc:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_nearest:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_sqrt:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_add:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_sub:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_mul:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_div:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_min:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_max:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_copysign:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_wrap_i64:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_trunc_f32_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_trunc_f32_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_trunc_f64_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_trunc_f64_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_extend_i32_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_extend_i32_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_trunc_f32_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_trunc_f32_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_trunc_f64_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_trunc_f64_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_convert_i32_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_convert_i32_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_convert_i64_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_convert_i64_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_demote_f64:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_convert_i32_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_convert_i32_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_convert_i64_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_convert_i64_u:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_promote_f32:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_reinterpret_f32:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_reinterpret_f64:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_reinterpret_i32:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_reinterpret_i64:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_extend8_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_extend16_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_extend8_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_extend16_s:
                {
                    ::uwvm::unfinished();
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_extend32_s:
                {
                    ::uwvm::unfinished();
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
