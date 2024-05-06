#pragma once
#include <cstddef>
#include <limits>
#include <fast_io.h>
#ifdef UWVM_TIMER
    #include <fast_io_driver/timer.h>
#endif
#include <io_device.h>

#include "types.h"
#include "section.h"
#include "module.h"

namespace uwvm::wasm
{
    template <::std::integral char_type, typename s>
    inline constexpr void print_wasm_module(s&& stm, ::uwvm::wasm::wasm_module const& wasmmod) noexcept
    {
        // wasm file
        auto const wasm_file_begin{wasmmod.module_begin};
        auto const wasm_file_end{wasmmod.module_end};

        if constexpr(::std::same_as<char_type, char>) {}
        else if constexpr(::std::same_as<char_type, wchar_t>) {}
        else if constexpr(::std::same_as<char_type, char8_t>)
        {
            // objdump
            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                             // L0
                                                             u8"\n",
                                                             // L1
                                                             wasmmod.module_name,
                                                             u8":\n"
                                                             // L2
                                                             u8"file format wasm ",
                                                             ::fast_io::mnp::hex0x(wasmmod.wasm_version),
                                                             u8"\n"
                                                             // L3
                                                             u8"Section Details:"
                                                             // L4 - L5
                                                             u8"\n\n");

            if(wasmmod.typesec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u8"Type[",
                                                                 wasmmod.typesec.type_count,
                                                                 u8"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.typesec.sec_begin - wasm_file_begin),
                                                                 u8" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.typesec.sec_end - wasm_file_begin),
                                                                 u8" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.typesec.sec_end - wasmmod.typesec.sec_begin),
                                                                 u8"):\n");

                // Type
                for(::std::size_t count{}; auto const& t: wasmmod.typesec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8" - type[", count++, u8"] (");
                    if(t.parameter_begin != t.parameter_end)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), *t.parameter_begin);
                        for(auto curr_para{t.parameter_begin + 1}; curr_para < t.parameter_end; ++curr_para)
                        {
                            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8", ", *curr_para);
                        }
                    }

                    if(t.result_begin == t.result_end) { ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u8") -> nil"); }
                    else if(t.result_end - t.result_begin == 1)
                    {
                        ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u8") -> ", *t.result_begin);
                    }
                    else
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8") -> (", *t.result_begin);
                        for(auto curr_res{t.result_begin + 1}; curr_res < t.result_end; ++curr_res)
                        {
                            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8", ", *curr_res);
                        }
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8")\n");
                    }
                }
            }

            auto const func_type_table_base{wasmmod.typesec.types.cbegin()};

            if(wasmmod.importsec.sec_begin) [[likely]]
            {
                // Import
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u8"\n" u8"Import[",
                                                                 wasmmod.importsec.import_count,
                                                                 u8"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.importsec.sec_begin - wasm_file_begin),
                                                                 u8" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.importsec.sec_end - wasm_file_begin),
                                                                 u8" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.importsec.sec_end - wasmmod.importsec.sec_begin),
                                                                 u8"):\n");

                // func
                for(::std::size_t count{}; auto const t: wasmmod.importsec.func_types)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                u8" - "
                                                                u8"func"
                                                                u8"[",
                                                                count++,
                                                                u8"] sig=",
                                                                t->extern_type.function - func_type_table_base,
                                                                u8" <__imported_",
                                                                ::fast_io::mnp::strvw(t->module_begin, t->module_end),
                                                                u8"_",
                                                                ::fast_io::mnp::strvw(t->name_begin, t->name_end),
                                                                u8"> <- ",
                                                                ::fast_io::mnp::strvw(t->module_begin, t->module_end),
                                                                u8".",
                                                                ::fast_io::mnp::strvw(t->name_begin, t->name_end));
                }

                // table
                for(::std::size_t count{}; auto const t: wasmmod.importsec.table_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                u8" - "
                                                                u8"table"
                                                                u8"[",
                                                                count++,
                                                                u8"] type=",
                                                                t->extern_type.table.elem_type,
                                                                u8" initial=",
                                                                t->extern_type.table.limits.min);
                    if(t->extern_type.table.limits.present_max)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8" max=", t->extern_type.table.limits.max);
                    }

                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    u8" <- ",
                                                                    ::fast_io::mnp::strvw(t->module_begin, t->module_end),
                                                                    u8".",
                                                                    ::fast_io::mnp::strvw(t->name_begin, t->name_end));
                }

                // memory
                for(::std::size_t count{}; auto const t: wasmmod.importsec.memory_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                u8" - "
                                                                u8"memory"
                                                                u8"[",
                                                                count++,
                                                                u8"] initial=",
                                                                t->extern_type.memory.limits.min);
                    if(t->extern_type.memory.limits.present_max)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8" max=", t->extern_type.memory.limits.max);
                    }

                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    u8" <- ",
                                                                    ::fast_io::mnp::strvw(t->module_begin, t->module_end),
                                                                    u8".",
                                                                    ::fast_io::mnp::strvw(t->name_begin, t->name_end));
                }

                // global
                for(::std::size_t count{}; auto const t: wasmmod.importsec.global_types)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                u8" - "
                                                                u8"global"
                                                                u8"[",
                                                                count++,
                                                                u8"] ",
                                                                t->extern_type.global.type,
                                                                u8" mutable=",
                                                                t->extern_type.global.is_mutable,
                                                                u8" <- ",
                                                                ::fast_io::mnp::strvw(t->module_begin, t->module_end),
                                                                u8".",
                                                                ::fast_io::mnp::strvw(t->name_begin, t->name_end));
                }

                // tag
                for(::std::size_t count{}; auto const t: wasmmod.importsec.tag_types)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                u8" - "
                                                                u8"tag"
                                                                u8"[",
                                                                count++,
                                                                u8"] attribute=",
                                                                ::fast_io::mnp::hex0x<true>(t->extern_type.exception.attribute),
                                                                u8" type=",
                                                                t->extern_type.exception.type - func_type_table_base,
                                                                u8" <- ",
                                                                ::fast_io::mnp::strvw(t->module_begin, t->module_end),
                                                                u8".",
                                                                ::fast_io::mnp::strvw(t->name_begin, t->name_end));
                }
            }

            if(wasmmod.functionsec.sec_begin) [[likely]]
            {
                // Function
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u8"\n" u8"Function[",
                                                                 wasmmod.functionsec.function_count,
                                                                 u8"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.functionsec.sec_begin - wasm_file_begin),
                                                                 u8" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.functionsec.sec_end - wasm_file_begin),
                                                                 u8" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.functionsec.sec_end - wasmmod.functionsec.sec_begin),
                                                                 u8"):\n");

                for(::std::size_t count{wasmmod.importsec.func_types.size()}; auto const& t: wasmmod.functionsec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                u8" - "
                                                                u8"func"
                                                                u8"[",
                                                                count++,
                                                                u8"] sig=",
                                                                t.func_type - func_type_table_base);
                    if((t.name_end - t.name_begin) != 0)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                         u8" <",
                                                                         ::fast_io::mnp::strvw(t.name_begin, t.name_end),
                                                                         u8">");
                    }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
                }
            }

            if(wasmmod.tablesec.sec_begin) [[likely]]
            {
                // Table
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u8"\n" u8"table[",
                                                                 wasmmod.tablesec.table_count,
                                                                 u8"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.tablesec.sec_begin - wasm_file_begin),
                                                                 u8" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.tablesec.sec_end - wasm_file_begin),
                                                                 u8" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.tablesec.sec_end - wasmmod.tablesec.sec_begin),
                                                                 u8"):\n");

                for(::std::size_t count{wasmmod.importsec.table_types.size()}; auto const& t: wasmmod.tablesec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                u8" - "
                                                                u8"table"
                                                                u8"[",
                                                                count++,
                                                                u8"] type=",
                                                                t.elem_type,
                                                                u8" initial=",
                                                                t.limits.min);
                    if(t.limits.present_max) { ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8" max=", t.limits.max); }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
                }
            }

            if(wasmmod.memorysec.sec_begin) [[likely]]
            {
                // Table
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u8"\n" u8"memory[",
                                                                 wasmmod.memorysec.memory_count,
                                                                 u8"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.memorysec.sec_begin - wasm_file_begin),
                                                                 u8" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.memorysec.sec_end - wasm_file_begin),
                                                                 u8" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.memorysec.sec_end - wasmmod.memorysec.sec_begin),
                                                                 u8"):\n");

                for(::std::size_t count{wasmmod.importsec.memory_types.size()}; auto const& t: wasmmod.memorysec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8" - " u8"memory" u8"[", count++, u8"] initial=", t.limits.min);
                    if(t.limits.present_max) { ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8" max=", t.limits.max); }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
                }
            }

            if(wasmmod.globalsec.sec_begin) [[likely]]
            {
                // Table
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u8"\n" u8"global[",
                                                                 wasmmod.globalsec.global_count,
                                                                 u8"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.globalsec.sec_begin - wasm_file_begin),
                                                                 u8" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.globalsec.sec_end - wasm_file_begin),
                                                                 u8" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.globalsec.sec_end - wasmmod.globalsec.sec_begin),
                                                                 u8"):\n");

                for(::std::size_t count{wasmmod.importsec.global_types.size()}; auto const& t: wasmmod.globalsec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                u8" - "
                                                                u8"global"
                                                                u8"[",
                                                                count++,
                                                                u8"] ",
                                                                t.type.type,
                                                                u8" mutable=",
                                                                t.type.is_mutable);
                    if((t.name_end - t.name_begin) != 0)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                         u8" <",
                                                                         ::fast_io::mnp::strvw(t.name_begin, t.name_end),
                                                                         u8">");
                    }
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8" - init ");

                    switch(t.initializer.type_opcode.op)
                    {
                        case ::uwvm::wasm::op_basic::global_get:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u8"global_get=global[", t.initializer.ref, u8"]");
                            break;
                        }
                        case ::uwvm::wasm::op_basic::i32_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u8"i32=", t.initializer.i32);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::i64_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u8"i64=", t.initializer.i64);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::f32_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u8"f32=", t.initializer.f32);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::f64_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u8"f64=", t.initializer.f64);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::ref_null:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u8"ref_null=", t.initializer.null_reftype);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::ref_func:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u8"ref_func=function[", t.initializer.ref, u8"]");
                            break;
                        }
                        case ::uwvm::wasm::op_basic::simd_prefix:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u8"v128=i8x16: ", i8x16(t.initializer.v128));
                            break;
                        }
                        default: ::fast_io::unreachable();
                    }
                }
            }

            if(wasmmod.exportsec.sec_begin) [[likely]]
            {
                // Export
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u8"\n" u8"Export[",
                                                                 wasmmod.exportsec.export_count,
                                                                 u8"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.exportsec.sec_begin - wasm_file_begin),
                                                                 u8" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.exportsec.sec_end - wasm_file_begin),
                                                                 u8" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.exportsec.sec_end - wasmmod.exportsec.sec_begin),
                                                                 u8"):\n");

                // func

                for(auto const t: wasmmod.exportsec.func_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     u8" - "
                                                                     u8"func" 
                                                                     u8"[",
                                                                     t->index, 
                                                                     u8"] -> \"",
                                                                     ::fast_io::mnp::strvw(t->name_begin, t->name_end),
                                                                     u8"\"\n");
                }

                // table
                for(auto const t: wasmmod.exportsec.table_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     u8" - "
                                                                     u8"table" 
                                                                     u8"[",
                                                                     t->index, 
                                                                     u8"] -> \"",
                                                                     ::fast_io::mnp::strvw(t->name_begin, t->name_end),
                                                                     u8"\"\n");
                }

                // memory
                for(auto const t: wasmmod.exportsec.memory_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     u8" - "
                                                                     u8"memroy" 
                                                                     u8"[",
                                                                     t->index, 
                                                                     u8"] -> \"",
                                                                     ::fast_io::mnp::strvw(t->name_begin, t->name_end),
                                                                     u8"\"\n");
                }

                // global
                for(auto const t: wasmmod.exportsec.global_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     u8" - "
                                                                     u8"global" 
                                                                     u8"[",
                                                                     t->index, 
                                                                     u8"] -> \"",
                                                                     ::fast_io::mnp::strvw(t->name_begin, t->name_end),
                                                                     u8"\"\n");
                }

                // tag
                for(auto const t: wasmmod.exportsec.tag_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     u8" - "
                                                                     u8"tag" 
                                                                     u8"[",
                                                                     t->index, 
                                                                     u8"] -> \"",
                                                                     ::fast_io::mnp::strvw(t->name_begin, t->name_end),
                                                                     u8"\"\n");
                }
            }

            // start
            if(wasmmod.startsec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u8"\n" u8"start",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.startsec.sec_begin - wasm_file_begin),
                                                                 u8" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.startsec.sec_end - wasm_file_begin),
                                                                 u8" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.startsec.sec_end - wasmmod.startsec.sec_begin),
                                                                 u8"):\n" 
                                                                 u8" - "
                                                                 u8"func[",
                                                                 wasmmod.startsec.index,
                                                                 u8"]\n");
            }
        }
        else if constexpr(::std::same_as<char_type, char16_t>) {}
        else if constexpr(::std::same_as<char_type, char32_t>) {}

        // ln
        ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
    }

    namespace details
    {
        struct objdump_printer_lv
        {
            ::uwvm::wasm::wasm_module const& wasmmod_cref;
        };

        template <::std::integral char_type, typename s>
        inline constexpr void print_define(::fast_io::io_reserve_type_t<char_type, objdump_printer_lv>, s&& stm, objdump_printer_lv op) noexcept
        {
            print_wasm_module<char_type>(::std::forward<s>(stm), op.wasmmod_cref);
        }

        struct objdump_printer_rv
        {
            ::uwvm::wasm::wasm_module wasmmod_cref;
        };

        template <::std::integral char_type, typename s>
        inline constexpr void print_define(::fast_io::io_reserve_type_t<char_type, objdump_printer_rv>, s&& stm, objdump_printer_rv const& op) noexcept
        {
            print_wasm_module<char_type>(::std::forward<s>(stm), op.wasmmod_cref);
        }
    }  // namespace details

    inline constexpr details::objdump_printer_lv objdump(::uwvm::wasm::wasm_module const& wasmmod) noexcept { return details::objdump_printer_lv{wasmmod}; }

    inline constexpr details::objdump_printer_rv objdump(::uwvm::wasm::wasm_module&& wasmmod) noexcept
    {
        return details::objdump_printer_rv{::std::move(wasmmod)};
    }

}  // namespace uwvm::wasm
