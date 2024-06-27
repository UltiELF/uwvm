#pragma once
#ifdef UWVM_TIMER
    #include <fast_io_driver/timer.h>
#endif
#include "astgen.h"
#include "aststorge.h"
#include "../../run/wasm_file.h"
#include "../wasm.h"

#include "ast.h"
#include "aststorge.h"
#include "global.h"

#include "astgen.h"
#include "astrun.h"
#include "memory/memory.h"

namespace uwvm::vm::interpreter
{
    inline void interpret() noexcept
    {
#ifdef UWVM_TIMER
        ::fast_io::timer int_timer{u8"uwvm: [timer] interpret"};
#endif
        auto const& wasmmod{::uwvm::global_wasm_module};

        ::uwvm::vm::start_func = ::uwvm::vm::get_start_func(wasmmod);

        if(::uwvm::vm::start_func == static_cast<::std::size_t>(-1)) [[unlikely]]
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
                                u8"Cannot find function: start.\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }

        auto const import_function_count{wasmmod.importsec.func_types.size()};
        auto const local_func_count{wasmmod.functionsec.function_count};
        auto const func_count{import_function_count + local_func_count};

        auto const import_global_count{wasmmod.importsec.global_types.size()};
        auto const local_global_count{wasmmod.globalsec.global_count};
        auto const global_count{import_global_count + local_global_count};

        // init global
        ::uwvm::vm::interpreter::globals.init(global_count);

        auto curr_global{::uwvm::vm::interpreter::globals.globals};

        for(auto const i: wasmmod.importsec.global_types)
        {
            curr_global->gt = i->extern_type.global;
            ++curr_global;
        }

        for(auto const& i: wasmmod.globalsec.types)
        {
            curr_global->gt = i.type;

            switch(curr_global->gt.type)
            {
                case ::uwvm::wasm::value_type::externref: [[fallthrough]];
                case ::uwvm::wasm::value_type::funcref:
                {
                    curr_global->gt.type = ::uwvm::wasm::value_type::i32;
                    break;
                }
                default: break;
            }

            // init
            switch(i.initializer.type_opcode.op)
            {
                case ::uwvm::wasm::op_basic::global_get:
                {
                    curr_global->value = ::uwvm::vm::interpreter::globals.globals[i.initializer.ref].value;
                    break;
                }
                case ::uwvm::wasm::op_basic::i32_const:
                {
                    curr_global->value.i32 = i.initializer.i32;
                    break;
                }
                case ::uwvm::wasm::op_basic::i64_const:
                {
                    curr_global->value.i64 = i.initializer.i64;
                    break;
                }
                case ::uwvm::wasm::op_basic::f32_const:
                {
                    curr_global->value.f32 = i.initializer.f32;
                    break;
                }
                case ::uwvm::wasm::op_basic::f64_const:
                {
                    curr_global->value.f64 = i.initializer.f64;
                    break;
                }
                case ::uwvm::wasm::op_basic::ref_null:
                {
                    curr_global->value.i32 = static_cast<::uwvm::wasm::wasm_i32>(i.initializer.null_reftype);
                    break;
                }
                case ::uwvm::wasm::op_basic::ref_func:
                {
                    curr_global->value.i32 = static_cast<::uwvm::wasm::wasm_i32>(i.initializer.ref);
                    break;
                }
                case ::uwvm::wasm::op_basic::simd:
                {
                    curr_global->value.v128 = i.initializer.v128;
                    break;
                }
                default: ::fast_io::unreachable();
            }
            ++curr_global;
        }

        // init memory
        ::uwvm::vm::interpreter::memories.reserve(wasmmod.memorysec.memory_count);
        for(auto& i: wasmmod.memorysec.types)
        {
            ::uwvm::vm::interpreter::memories.emplace_back_unchecked(i);
        }

        // init ast
        ::uwvm::vm::interpreter::stroage.asts = ::fast_io::vector<::uwvm::vm::interpreter::ast>(local_func_count);

        if(::uwvm::vm::start_func >= import_function_count)
        {
            auto const index{::uwvm::vm::start_func - import_function_count};
            auto& start_func{::uwvm::vm::interpreter::stroage.asts.index_unchecked(index)};
            start_func = ::uwvm::vm::interpreter::generate_ast(wasmmod.functionsec.types.index_unchecked(index).func_type,
                                                               wasmmod.codesec.bodies.index_unchecked(index));
            ::uwvm::vm::interpreter::run_ast(start_func);
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
                                u8"Cannot start import function.\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
    }

}  // namespace uwvm::vm::interpreter
