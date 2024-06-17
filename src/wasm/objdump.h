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

        if constexpr(::std::same_as<char_type, char>)
        {
            // objdump
            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                             // L0
                                                             "\n",
                                                             // L1
                                                             ::fast_io::mnp::code_cvt(wasmmod.module_name),
                                                             ":\n"
                                                             // L2
                                                             "file format wasm ",
                                                             ::fast_io::mnp::hex0x(wasmmod.wasm_version),
                                                             "\n"
                                                             // L3
                                                             "Section Details:"
                                                             // L4 - L5
                                                             "\n\n");

            if(wasmmod.typesec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 "Type[",
                                                                 wasmmod.typesec.type_count,
                                                                 "] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.typesec.sec_begin - wasm_file_begin),
                                                                 " end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.typesec.sec_end - wasm_file_begin),
                                                                 " size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.typesec.sec_end - wasmmod.typesec.sec_begin),
                                                                 "):\n");

                // Type
                for(::std::size_t count{}; auto const& t: wasmmod.typesec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), " - type[", count++, "] (");
                    if(t.parameter_begin != t.parameter_end)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), *t.parameter_begin);
                        for(auto curr_para{t.parameter_begin + 1}; curr_para < t.parameter_end; ++curr_para)
                        {
                            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), ", ", *curr_para);
                        }
                    }

                    if(t.result_begin == t.result_end) { ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), ") -> nil"); }
                    else if(t.result_end - t.result_begin == 1)
                    {
                        ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), ") -> ", *t.result_begin);
                    }
                    else
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), ") -> (", *t.result_begin);
                        for(auto curr_res{t.result_begin + 1}; curr_res < t.result_end; ++curr_res)
                        {
                            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), ", ", *curr_res);
                        }
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), ")\n");
                    }
                }
            }

            auto const func_type_table_base{wasmmod.typesec.types.cbegin()};

            if(wasmmod.importsec.sec_begin) [[likely]]
            {
                // Import
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 "\n" "Import[",
                                                                 wasmmod.importsec.import_count,
                                                                 "] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.importsec.sec_begin - wasm_file_begin),
                                                                 " end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.importsec.sec_end - wasm_file_begin),
                                                                 " size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.importsec.sec_end - wasmmod.importsec.sec_begin),
                                                                 "):\n");

                // func
                for(::std::size_t count{}; auto const t: wasmmod.importsec.func_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                " - "
                                                                "func"
                                                                "[",
                                                                count++,
                                                                "] sig=",
                                                                t->extern_type.function - func_type_table_base);
                    if((t->custom_name_end - t->custom_name_begin) != 0)
                    {
                        ::fast_io::operations::print_freestanding<false>(
                            ::std::forward<s>(stm),
                            " <",
                            ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->custom_name_begin, t->custom_name_end)),
                            ">");
                    }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    " <- ",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                    ".",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }

                // table
                for(::std::size_t count{}; auto const t: wasmmod.importsec.table_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                " - "
                                                                "table"
                                                                "[",
                                                                count++,
                                                                "] type=",
                                                                t->extern_type.table.elem_type,
                                                                " initial=",
                                                                t->extern_type.table.limits.min);
                    if(t->extern_type.table.limits.present_max)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), " max=", t->extern_type.table.limits.max);
                    }

                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    " <- ",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                    ".",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }

                // memory
                for(::std::size_t count{}; auto const t: wasmmod.importsec.memory_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                " - "
                                                                "memory"
                                                                "[",
                                                                count++,
                                                                "] initial=",
                                                                t->extern_type.memory.limits.min);
                    if(t->extern_type.memory.limits.present_max)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), " max=", t->extern_type.memory.limits.max);
                    }

                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    " <- ",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                    ".",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }

                // global
                for(::std::size_t count{}; auto const t: wasmmod.importsec.global_types)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                " - "
                                                                "global"
                                                                "[",
                                                                count++,
                                                                "] ",
                                                                t->extern_type.global.type,
                                                                " mutable=",
                                                                t->extern_type.global.is_mutable,
                                                                " <- ",
                                                                ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                ".",
                                                                ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }

                // tag
                for(::std::size_t count{}; auto const t: wasmmod.importsec.tag_types)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                " - "
                                                                "tag"
                                                                "[",
                                                                count++,
                                                                "] attribute=",
                                                                ::fast_io::mnp::hex0x<true>(t->extern_type.exception.attribute),
                                                                " type=",
                                                                t->extern_type.exception.type - func_type_table_base,
                                                                " <- ",
                                                                ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                ".",
                                                                ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }
            }

            if(wasmmod.functionsec.sec_begin) [[likely]]
            {
                // Function
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 "\n" "Function[",
                                                                 wasmmod.functionsec.function_count,
                                                                 "] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.functionsec.sec_begin - wasm_file_begin),
                                                                 " end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.functionsec.sec_end - wasm_file_begin),
                                                                 " size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.functionsec.sec_end - wasmmod.functionsec.sec_begin),
                                                                 "):\n");

                for(::std::size_t count{wasmmod.importsec.func_types.size()}; auto const& t: wasmmod.functionsec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                " - "
                                                                "func"
                                                                "[",
                                                                count++,
                                                                "] sig=",
                                                                t.func_type - func_type_table_base);
                    if((t.custom_name_end - t.custom_name_begin) != 0)
                    {
                        ::fast_io::operations::print_freestanding<false>(
                            ::std::forward<s>(stm),
                            " <",
                            ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t.custom_name_begin, t.custom_name_end)),
                            ">");
                    }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
                }
            }

            if(wasmmod.tablesec.sec_begin) [[likely]]
            {
                // Table
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 "\n" "table[",
                                                                 wasmmod.tablesec.table_count,
                                                                 "] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.tablesec.sec_begin - wasm_file_begin),
                                                                 " end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.tablesec.sec_end - wasm_file_begin),
                                                                 " size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.tablesec.sec_end - wasmmod.tablesec.sec_begin),
                                                                 "):\n");

                for(::std::size_t count{wasmmod.importsec.table_types.size()}; auto const& t: wasmmod.tablesec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                " - "
                                                                "table"
                                                                "[",
                                                                count++,
                                                                "] type=",
                                                                t.elem_type,
                                                                " initial=",
                                                                t.limits.min);
                    if(t.limits.present_max) { ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), " max=", t.limits.max); }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
                }
            }

            if(wasmmod.memorysec.sec_begin) [[likely]]
            {
                // Table
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 "\n" "memory[",
                                                                 wasmmod.memorysec.memory_count,
                                                                 "] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.memorysec.sec_begin - wasm_file_begin),
                                                                 " end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.memorysec.sec_end - wasm_file_begin),
                                                                 " size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.memorysec.sec_end - wasmmod.memorysec.sec_begin),
                                                                 "):\n");

                for(::std::size_t count{wasmmod.importsec.memory_types.size()}; auto const& t: wasmmod.memorysec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), " - " "memory" "[", count++, "] initial=", t.limits.min);
                    if(t.limits.present_max) { ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), " max=", t.limits.max); }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
                }
            }

            if(wasmmod.globalsec.sec_begin) [[likely]]
            {
                // Table
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 "\n" "global[",
                                                                 wasmmod.globalsec.global_count,
                                                                 "] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.globalsec.sec_begin - wasm_file_begin),
                                                                 " end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.globalsec.sec_end - wasm_file_begin),
                                                                 " size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.globalsec.sec_end - wasmmod.globalsec.sec_begin),
                                                                 "):\n");

                for(::std::size_t count{wasmmod.importsec.global_types.size()}; auto const& t: wasmmod.globalsec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                " - "
                                                                "global"
                                                                "[",
                                                                count++,
                                                                "] ",
                                                                t.type.type,
                                                                " mutable=",
                                                                t.type.is_mutable);
                    if((t.name_end - t.name_begin) != 0)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                         " <",
                                                                         ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t.name_begin, t.name_end)),
                                                                         ">");
                    }
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), " - init ");

                    switch(t.initializer.type_opcode.op)
                    {
                        case ::uwvm::wasm::op_basic::global_get:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), "global_get=global[", t.initializer.ref, "]");
                            break;
                        }
                        case ::uwvm::wasm::op_basic::i32_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), "i32=", t.initializer.i32);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::i64_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), "i64=", t.initializer.i64);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::f32_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), "f32=", t.initializer.f32);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::f64_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), "f64=", t.initializer.f64);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::ref_null:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), "ref_null=", t.initializer.null_reftype);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::ref_func:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), "ref_func=function[", t.initializer.ref, "]");
                            break;
                        }
                        case ::uwvm::wasm::op_basic::simd:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), "v128=i8x16: ", i8x16(t.initializer.v128));
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
                                                                 "\n" "Export[",
                                                                 wasmmod.exportsec.export_count,
                                                                 "] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.exportsec.sec_begin - wasm_file_begin),
                                                                 " end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.exportsec.sec_end - wasm_file_begin),
                                                                 " size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.exportsec.sec_end - wasmmod.exportsec.sec_begin),
                                                                 "):\n");

                // func

                for(auto const t: wasmmod.exportsec.func_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     " - "
                                                                     "func" 
                                                                     "[",
                                                                     t->index, 
                                                                     "] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                     "\"\n");
                }

                // table
                for(auto const t: wasmmod.exportsec.table_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     " - "
                                                                     "table" 
                                                                     "[",
                                                                     t->index, 
                                                                     "] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                     "\"\n");
                }

                // memory
                for(auto const t: wasmmod.exportsec.memory_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     " - "
                                                                     "memory" 
                                                                     "[",
                                                                     t->index, 
                                                                     "] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                     "\"\n");
                }

                // global
                for(auto const t: wasmmod.exportsec.global_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     " - "
                                                                     "global" 
                                                                     "[",
                                                                     t->index, 
                                                                     "] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                     "\"\n");
                }

                // tag
                for(auto const t: wasmmod.exportsec.tag_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     " - "
                                                                     "tag" 
                                                                     "[",
                                                                     t->index, 
                                                                     "] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                     "\"\n");
                }
            }

            // start
            if(wasmmod.startsec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 "\n" "start",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.startsec.sec_begin - wasm_file_begin),
                                                                 " end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.startsec.sec_end - wasm_file_begin),
                                                                 " size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.startsec.sec_end - wasmmod.startsec.sec_begin),
                                                                 "):\n" 
                                                                 " - "
                                                                 "func[",
                                                                 wasmmod.startsec.index,
                                                                 "]\n");
            }

            // element
            if(wasmmod.elemsec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 "\n" "Elem[",
                                                                 wasmmod.elemsec.elem_segment_count,
                                                                 "] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.elemsec.sec_begin - wasm_file_begin),
                                                                 " end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.elemsec.sec_end - wasm_file_begin),
                                                                 " size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.elemsec.sec_end - wasmmod.elemsec.sec_begin),
                                                                 "):\n");

                // segment
                for(::std::size_t segment_counter{}; auto const& t: wasmmod.elemsec.elem_segments)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    " - segment[",
                                                                    segment_counter++,
                                                                    "] flags=0 table=",
                                                                    t.index,
                                                                    " count=",
                                                                    t.elem_count,
                                                                    " - init i32=",
                                                                    t.initializer.i32);
                    // elems
                    for(::std::size_t elem_counter{}; auto const e: t.elems)
                    {
                        ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), "  - elem[", elem_counter++, "] = ref.func:", e);
                    }
                }
            }

            // code
            if(wasmmod.codesec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 "\n" "Code[",
                                                                 wasmmod.codesec.code_count,
                                                                 "] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.codesec.sec_begin - wasm_file_begin),
                                                                 " end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.codesec.sec_end - wasm_file_begin),
                                                                 " size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.codesec.sec_end - wasmmod.codesec.sec_begin),
                                                                 "):\n");

                for(::std::size_t code_count{wasmmod.importsec.func_types.size()}; auto const& t: wasmmod.codesec.bodies)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), " - " "func" "[", code_count++, "] size=", t.body_size, " local=", t.local_count);
                }
            }

            // data
            if(wasmmod.datasec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 "\n" "Data[",
                                                                 wasmmod.datasec.count,
                                                                 "] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.datasec.sec_begin - wasm_file_begin),
                                                                 " end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.datasec.sec_end - wasm_file_begin),
                                                                 " size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.datasec.sec_end - wasmmod.datasec.sec_begin),
                                                                 "):\n");

                for(::std::size_t count{}; auto const& t: wasmmod.datasec.entries)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    " - " "segment" "[", 
                                                                    count++,
                                                                    "] memory=",
                                                                    t.index,
                                                                    " size=",
                                                                    t.size,
                                                                    " - init i32=",
                                                                    t.offset.i32);
                }
            }

            // custom
            for(auto const& t: wasmmod.cussecs)
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                    "\n"
                                                                    "Custom:\n"
                                                                    " - name: \"", 
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t.name_begin,t.name_end)), 
                                                                    "\"\n");
            }
        }
        else if constexpr(::std::same_as<char_type, wchar_t>)
        {
            // objdump
            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                             // L0
                                                             L"\n",
                                                             // L1
                                                             ::fast_io::mnp::code_cvt(wasmmod.module_name),
                                                             L":\n"
                                                             // L2
                                                             L"file format wasm ",
                                                             ::fast_io::mnp::hex0x(wasmmod.wasm_version),
                                                             L"\n"
                                                             // L3
                                                             L"Section Details:"
                                                             // L4 - L5
                                                             L"\n\n");

            if(wasmmod.typesec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 L"Type[",
                                                                 wasmmod.typesec.type_count,
                                                                 L"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.typesec.sec_begin - wasm_file_begin),
                                                                 L" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.typesec.sec_end - wasm_file_begin),
                                                                 L" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.typesec.sec_end - wasmmod.typesec.sec_begin),
                                                                 L"):\n");

                // Type
                for(::std::size_t count{}; auto const& t: wasmmod.typesec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), L" - type[", count++, L"] (");
                    if(t.parameter_begin != t.parameter_end)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), *t.parameter_begin);
                        for(auto curr_para{t.parameter_begin + 1}; curr_para < t.parameter_end; ++curr_para)
                        {
                            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), L", ", *curr_para);
                        }
                    }

                    if(t.result_begin == t.result_end) { ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), L") -> nil"); }
                    else if(t.result_end - t.result_begin == 1)
                    {
                        ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), L") -> ", *t.result_begin);
                    }
                    else
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), L") -> (", *t.result_begin);
                        for(auto curr_res{t.result_begin + 1}; curr_res < t.result_end; ++curr_res)
                        {
                            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), L", ", *curr_res);
                        }
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), L")\n");
                    }
                }
            }

            auto const func_type_table_base{wasmmod.typesec.types.cbegin()};

            if(wasmmod.importsec.sec_begin) [[likely]]
            {
                // Import
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 L"\n" L"Import[",
                                                                 wasmmod.importsec.import_count,
                                                                 L"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.importsec.sec_begin - wasm_file_begin),
                                                                 L" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.importsec.sec_end - wasm_file_begin),
                                                                 L" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.importsec.sec_end - wasmmod.importsec.sec_begin),
                                                                 L"):\n");

                // func
                for(::std::size_t count{}; auto const t: wasmmod.importsec.func_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                L" - "
                                                                L"func"
                                                                L"[",
                                                                count++,
                                                                L"] sig=",
                                                                t->extern_type.function - func_type_table_base);
                    if((t->custom_name_end - t->custom_name_begin) != 0)
                    {
                        ::fast_io::operations::print_freestanding<false>(
                            ::std::forward<s>(stm),
                            L" <",
                            ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->custom_name_begin, t->custom_name_end)),
                            L">");
                    }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    L" <- ",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                    L".",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }

                // table
                for(::std::size_t count{}; auto const t: wasmmod.importsec.table_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                L" - "
                                                                L"table"
                                                                L"[",
                                                                count++,
                                                                L"] type=",
                                                                t->extern_type.table.elem_type,
                                                                L" initial=",
                                                                t->extern_type.table.limits.min);
                    if(t->extern_type.table.limits.present_max)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), L" max=", t->extern_type.table.limits.max);
                    }

                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    L" <- ",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                    L".",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }

                // memory
                for(::std::size_t count{}; auto const t: wasmmod.importsec.memory_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                L" - "
                                                                L"memory"
                                                                L"[",
                                                                count++,
                                                                L"] initial=",
                                                                t->extern_type.memory.limits.min);
                    if(t->extern_type.memory.limits.present_max)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), L" max=", t->extern_type.memory.limits.max);
                    }

                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    L" <- ",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                    L".",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }

                // global
                for(::std::size_t count{}; auto const t: wasmmod.importsec.global_types)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                L" - "
                                                                L"global"
                                                                L"[",
                                                                count++,
                                                                L"] ",
                                                                t->extern_type.global.type,
                                                                L" mutable=",
                                                                t->extern_type.global.is_mutable,
                                                                L" <- ",
                                                                ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                L".",
                                                                ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }

                // tag
                for(::std::size_t count{}; auto const t: wasmmod.importsec.tag_types)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                L" - "
                                                                L"tag"
                                                                L"[",
                                                                count++,
                                                                L"] attribute=",
                                                                ::fast_io::mnp::hex0x<true>(t->extern_type.exception.attribute),
                                                                L" type=",
                                                                t->extern_type.exception.type - func_type_table_base,
                                                                L" <- ",
                                                                ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                L".",
                                                                ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }
            }

            if(wasmmod.functionsec.sec_begin) [[likely]]
            {
                // Function
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 L"\n" "Function[",
                                                                 wasmmod.functionsec.function_count,
                                                                 L"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.functionsec.sec_begin - wasm_file_begin),
                                                                 L" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.functionsec.sec_end - wasm_file_begin),
                                                                 L" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.functionsec.sec_end - wasmmod.functionsec.sec_begin),
                                                                 L"):\n");

                for(::std::size_t count{wasmmod.importsec.func_types.size()}; auto const& t: wasmmod.functionsec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                L" - "
                                                                L"func"
                                                                L"[",
                                                                count++,
                                                                L"] sig=",
                                                                t.func_type - func_type_table_base);
                    if((t.custom_name_end - t.custom_name_begin) != 0)
                    {
                        ::fast_io::operations::print_freestanding<false>(
                            ::std::forward<s>(stm),
                            L" <",
                            ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t.custom_name_begin, t.custom_name_end)),
                            L">");
                    }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
                }
            }

            if(wasmmod.tablesec.sec_begin) [[likely]]
            {
                // Table
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 L"\n" L"table[",
                                                                 wasmmod.tablesec.table_count,
                                                                 L"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.tablesec.sec_begin - wasm_file_begin),
                                                                 L" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.tablesec.sec_end - wasm_file_begin),
                                                                 L" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.tablesec.sec_end - wasmmod.tablesec.sec_begin),
                                                                 L"):\n");

                for(::std::size_t count{wasmmod.importsec.table_types.size()}; auto const& t: wasmmod.tablesec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                L" - "
                                                                L"table"
                                                                L"[",
                                                                count++,
                                                                L"] type=",
                                                                t.elem_type,
                                                                L" initial=",
                                                                t.limits.min);
                    if(t.limits.present_max) { ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), L" max=", t.limits.max); }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
                }
            }

            if(wasmmod.memorysec.sec_begin) [[likely]]
            {
                // Table
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 L"\n" L"memory[",
                                                                 wasmmod.memorysec.memory_count,
                                                                 L"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.memorysec.sec_begin - wasm_file_begin),
                                                                 L" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.memorysec.sec_end - wasm_file_begin),
                                                                 L" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.memorysec.sec_end - wasmmod.memorysec.sec_begin),
                                                                 L"):\n");

                for(::std::size_t count{wasmmod.importsec.memory_types.size()}; auto const& t: wasmmod.memorysec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), L" - " L"memory" L"[", count++, L"] initial=", t.limits.min);
                    if(t.limits.present_max) { ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), L" max=", t.limits.max); }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
                }
            }

            if(wasmmod.globalsec.sec_begin) [[likely]]
            {
                // Table
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 L"\n" L"global[",
                                                                 wasmmod.globalsec.global_count,
                                                                 L"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.globalsec.sec_begin - wasm_file_begin),
                                                                 L" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.globalsec.sec_end - wasm_file_begin),
                                                                 L" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.globalsec.sec_end - wasmmod.globalsec.sec_begin),
                                                                 L"):\n");

                for(::std::size_t count{wasmmod.importsec.global_types.size()}; auto const& t: wasmmod.globalsec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                L" - "
                                                                L"global"
                                                                L"[",
                                                                count++,
                                                                L"] ",
                                                                t.type.type,
                                                                L" mutable=",
                                                                t.type.is_mutable);
                    if((t.name_end - t.name_begin) != 0)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                         L" <",
                                                                         ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t.name_begin, t.name_end)),
                                                                         L">");
                    }
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), L" - init ");

                    switch(t.initializer.type_opcode.op)
                    {
                        case ::uwvm::wasm::op_basic::global_get:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), L"global_get=global[", t.initializer.ref, L"]");
                            break;
                        }
                        case ::uwvm::wasm::op_basic::i32_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), L"i32=", t.initializer.i32);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::i64_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), L"i64=", t.initializer.i64);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::f32_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), L"f32=", t.initializer.f32);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::f64_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), L"f64=", t.initializer.f64);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::ref_null:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), L"ref_null=", t.initializer.null_reftype);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::ref_func:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), L"ref_func=function[", t.initializer.ref, L"]");
                            break;
                        }
                        case ::uwvm::wasm::op_basic::simd:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), L"v128=i8x16: ", i8x16(t.initializer.v128));
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
                                                                 L"\n" L"Export[",
                                                                 wasmmod.exportsec.export_count,
                                                                 L"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.exportsec.sec_begin - wasm_file_begin),
                                                                 L" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.exportsec.sec_end - wasm_file_begin),
                                                                 L" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.exportsec.sec_end - wasmmod.exportsec.sec_begin),
                                                                 L"):\n");

                // func

                for(auto const t: wasmmod.exportsec.func_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     L" - "
                                                                     L"func" 
                                                                     L"[",
                                                                     t->index, 
                                                                     L"] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                     L"\"\n");
                }

                // table
                for(auto const t: wasmmod.exportsec.table_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     L" - "
                                                                     L"table" 
                                                                     L"[",
                                                                     t->index, 
                                                                     L"] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                     L"\"\n");
                }

                // memory
                for(auto const t: wasmmod.exportsec.memory_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     L" - "
                                                                     L"memory" 
                                                                     L"[",
                                                                     t->index, 
                                                                     L"] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                     L"\"\n");
                }

                // global
                for(auto const t: wasmmod.exportsec.global_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     L" - "
                                                                     L"global" 
                                                                     L"[",
                                                                     t->index, 
                                                                     L"] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                    L"\"\n");
                }

                // tag
                for(auto const t: wasmmod.exportsec.tag_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     L" - "
                                                                     L"tag" 
                                                                     L"[",
                                                                     t->index, 
                                                                     L"] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                     L"\"\n");
                }
            }

            // start
            if(wasmmod.startsec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 L"\n" "start",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.startsec.sec_begin - wasm_file_begin),
                                                                 L" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.startsec.sec_end - wasm_file_begin),
                                                                 L" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.startsec.sec_end - wasmmod.startsec.sec_begin),
                                                                 L"):\n" 
                                                                 L" - "
                                                                 L"func[",
                                                                 wasmmod.startsec.index,
                                                                 L"]\n");
            }

            // element
            if(wasmmod.elemsec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 L"\n" L"Elem[",
                                                                 wasmmod.elemsec.elem_segment_count,
                                                                 L"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.elemsec.sec_begin - wasm_file_begin),
                                                                 L" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.elemsec.sec_end - wasm_file_begin),
                                                                 L" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.elemsec.sec_end - wasmmod.elemsec.sec_begin),
                                                                 L"):\n");

                // segment
                for(::std::size_t segment_counter{}; auto const& t: wasmmod.elemsec.elem_segments)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    L" - segment[",
                                                                    segment_counter++,
                                                                    L"] flags=0 table=",
                                                                    t.index,
                                                                    L" count=",
                                                                    t.elem_count,
                                                                    L" - init i32=",
                                                                    t.initializer.i32);
                    // elems
                    for(::std::size_t elem_counter{}; auto const e: t.elems)
                    {
                        ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), L"  - elem[", elem_counter++, L"] = ref.func:", e);
                    }
                }
            }

            // code
            if(wasmmod.codesec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 L"\n" L"Code[",
                                                                 wasmmod.codesec.code_count,
                                                                 L"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.codesec.sec_begin - wasm_file_begin),
                                                                 L" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.codesec.sec_end - wasm_file_begin),
                                                                 L" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.codesec.sec_end - wasmmod.codesec.sec_begin),
                                                                 L"):\n");

                for(::std::size_t code_count{wasmmod.importsec.func_types.size()}; auto const& t: wasmmod.codesec.bodies)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), L" - " L"func" L"[", code_count++, L"] size=", t.body_size, L" local=", t.local_count);
                }
            }

            // data
            if(wasmmod.datasec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 L"\n" L"Data[",
                                                                 wasmmod.datasec.count,
                                                                 L"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.datasec.sec_begin - wasm_file_begin),
                                                                 L" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.datasec.sec_end - wasm_file_begin),
                                                                 L" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.datasec.sec_end - wasmmod.datasec.sec_begin),
                                                                 L"):\n");

                for(::std::size_t count{}; auto const& t: wasmmod.datasec.entries)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    L" - " L"segment" L"[", 
                                                                    count++,
                                                                    L"] memory=",
                                                                    t.index,
                                                                    L" size=",
                                                                    t.size,
                                                                    L" - init i32=",
                                                                    t.offset.i32);
                }
            }

            // custom
            for(auto const& t: wasmmod.cussecs)
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                    L"\n"
                                                                    L"Custom:\n"
                                                                    L" - name: \"", 
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t.name_begin,t.name_end)), 
                                                                    L"\"\n");
            }
        }
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
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                u8" - "
                                                                u8"func"
                                                                u8"[",
                                                                count++,
                                                                u8"] sig=",
                                                                t->extern_type.function - func_type_table_base);
                    if((t->custom_name_end - t->custom_name_begin) != 0)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                         u8" <",
                                                                         ::fast_io::mnp::strvw(t->custom_name_begin, t->custom_name_end),
                                                                         u8">");
                    }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    u8" <- ",
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
                    if((t.custom_name_end - t.custom_name_begin) != 0)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                         u8" <",
                                                                         ::fast_io::mnp::strvw(t.custom_name_begin, t.custom_name_end),
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
                        case ::uwvm::wasm::op_basic::simd:
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
                                                                     u8"memory" 
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

            // element
            if(wasmmod.elemsec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u8"\n" u8"Elem[",
                                                                 wasmmod.elemsec.elem_segment_count,
                                                                 u8"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.elemsec.sec_begin - wasm_file_begin),
                                                                 u8" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.elemsec.sec_end - wasm_file_begin),
                                                                 u8" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.elemsec.sec_end - wasmmod.elemsec.sec_begin),
                                                                 u8"):\n");

                // segment
                for(::std::size_t segment_counter{}; auto const& t: wasmmod.elemsec.elem_segments)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    u8" - segment[",
                                                                    segment_counter++,
                                                                    u8"] flags=0 table=",
                                                                    t.index,
                                                                    u8" count=",
                                                                    t.elem_count,
                                                                    u8" - init i32=",
                                                                    t.initializer.i32);
                    // elems
                    for(::std::size_t elem_counter{}; auto const e: t.elems)
                    {
                        ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u8"  - elem[", elem_counter++, u8"] = ref.func:", e);
                    }
                }
            }

            // code
            if(wasmmod.codesec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u8"\n" u8"Code[",
                                                                 wasmmod.codesec.code_count,
                                                                 u8"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.codesec.sec_begin - wasm_file_begin),
                                                                 u8" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.codesec.sec_end - wasm_file_begin),
                                                                 u8" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.codesec.sec_end - wasmmod.codesec.sec_begin),
                                                                 u8"):\n");

                for(::std::size_t code_count{wasmmod.importsec.func_types.size()}; auto const& t: wasmmod.codesec.bodies)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u8" - " u8"func" u8"[", code_count++, u8"] size=", t.body_size, u8" local=", t.local_count);
                }
            }

            // data
            if(wasmmod.datasec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u8"\n" u8"Data[",
                                                                 wasmmod.datasec.count,
                                                                 u8"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.datasec.sec_begin - wasm_file_begin),
                                                                 u8" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.datasec.sec_end - wasm_file_begin),
                                                                 u8" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.datasec.sec_end - wasmmod.datasec.sec_begin),
                                                                 u8"):\n");

                for(::std::size_t count{}; auto const& t: wasmmod.datasec.entries)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    u8" - " u8"segment" u8"[", 
                                                                    count++,
                                                                    u8"] memory=",
                                                                    t.index,
                                                                    u8" size=",
                                                                    t.size,
                                                                    u8" - init i32=",
                                                                    t.offset.i32);
                }
            }

            // custom
            for(auto const& t: wasmmod.cussecs)
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                    u8"\n"
                                                                    u8"Custom:\n"
                                                                    u8" - name: \"", 
                                                                    ::fast_io::mnp::strvw(t.name_begin,t.name_end), 
                                                                    u8"\"\n");
            }
        }
        else if constexpr(::std::same_as<char_type, char16_t>)
        {
            // objdump
            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                             // L0
                                                             u"\n",
                                                             // L1
                                                             ::fast_io::mnp::code_cvt(wasmmod.module_name),
                                                             u":\n"
                                                             // L2
                                                             u"file format wasm ",
                                                             ::fast_io::mnp::hex0x(wasmmod.wasm_version),
                                                             u"\n"
                                                             // L3
                                                             u"Section Details:"
                                                             // L4 - L5
                                                             u"\n\n");

            if(wasmmod.typesec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u"Type[",
                                                                 wasmmod.typesec.type_count,
                                                                 u"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.typesec.sec_begin - wasm_file_begin),
                                                                 u" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.typesec.sec_end - wasm_file_begin),
                                                                 u" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.typesec.sec_end - wasmmod.typesec.sec_begin),
                                                                 u"):\n");

                // Type
                for(::std::size_t count{}; auto const& t: wasmmod.typesec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u" - type[", count++, u"] (");
                    if(t.parameter_begin != t.parameter_end)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), *t.parameter_begin);
                        for(auto curr_para{t.parameter_begin + 1}; curr_para < t.parameter_end; ++curr_para)
                        {
                            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u", ", *curr_para);
                        }
                    }

                    if(t.result_begin == t.result_end) { ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u") -> niu"); }
                    else if(t.result_end - t.result_begin == 1)
                    {
                        ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u") -> ", *t.result_begin);
                    }
                    else
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u") -> (", *t.result_begin);
                        for(auto curr_res{t.result_begin + 1}; curr_res < t.result_end; ++curr_res)
                        {
                            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u", ", *curr_res);
                        }
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u")\n");
                    }
                }
            }

            auto const func_type_table_base{wasmmod.typesec.types.cbegin()};

            if(wasmmod.importsec.sec_begin) [[likely]]
            {
                // Import
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u"\n" u"Import[",
                                                                 wasmmod.importsec.import_count,
                                                                 u"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.importsec.sec_begin - wasm_file_begin),
                                                                 u" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.importsec.sec_end - wasm_file_begin),
                                                                 u" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.importsec.sec_end - wasmmod.importsec.sec_begin),
                                                                 u"):\n");

                // func
                for(::std::size_t count{}; auto const t: wasmmod.importsec.func_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                u" - "
                                                                u"func"
                                                                u"[",
                                                                count++,
                                                                u"] sig=",
                                                                t->extern_type.function - func_type_table_base);
                    if((t->custom_name_end - t->custom_name_begin) != 0)
                    {
                        ::fast_io::operations::print_freestanding<false>(
                            ::std::forward<s>(stm),
                            u" <",
                            ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->custom_name_begin, t->custom_name_end)),
                            u">");
                    }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    u" <- ",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                    u".",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }

                // table
                for(::std::size_t count{}; auto const t: wasmmod.importsec.table_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                u" - "
                                                                u"table"
                                                                u"[",
                                                                count++,
                                                                u"] type=",
                                                                t->extern_type.table.elem_type,
                                                                u" initial=",
                                                                t->extern_type.table.limits.min);
                    if(t->extern_type.table.limits.present_max)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u" max=", t->extern_type.table.limits.max);
                    }

                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    u" <- ",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                    u".",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }

                // memory
                for(::std::size_t count{}; auto const t: wasmmod.importsec.memory_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                u" - "
                                                                u"memory"
                                                                u"[",
                                                                count++,
                                                                u"] initial=",
                                                                t->extern_type.memory.limits.min);
                    if(t->extern_type.memory.limits.present_max)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u" max=", t->extern_type.memory.limits.max);
                    }

                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    u" <- ",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                    u".",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }

                // global
                for(::std::size_t count{}; auto const t: wasmmod.importsec.global_types)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                u" - "
                                                                u"globau"
                                                                u"[",
                                                                count++,
                                                                u"] ",
                                                                t->extern_type.global.type,
                                                                u" mutable=",
                                                                t->extern_type.global.is_mutable,
                                                                u" <- ",
                                                                ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                u".",
                                                                ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }

                // tag
                for(::std::size_t count{}; auto const t: wasmmod.importsec.tag_types)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                u" - "
                                                                u"tag"
                                                                u"[",
                                                                count++,
                                                                u"] attribute=",
                                                                ::fast_io::mnp::hex0x<true>(t->extern_type.exception.attribute),
                                                                u" type=",
                                                                t->extern_type.exception.type - func_type_table_base,
                                                                u" <- ",
                                                                ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                u".",
                                                                ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }
            }

            if(wasmmod.functionsec.sec_begin) [[likely]]
            {
                // Function
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u"\n" "Function[",
                                                                 wasmmod.functionsec.function_count,
                                                                 u"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.functionsec.sec_begin - wasm_file_begin),
                                                                 u" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.functionsec.sec_end - wasm_file_begin),
                                                                 u" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.functionsec.sec_end - wasmmod.functionsec.sec_begin),
                                                                 u"):\n");

                for(::std::size_t count{wasmmod.importsec.func_types.size()}; auto const& t: wasmmod.functionsec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                u" - "
                                                                u"func"
                                                                u"[",
                                                                count++,
                                                                u"] sig=",
                                                                t.func_type - func_type_table_base);
                    if((t.custom_name_end - t.custom_name_begin) != 0)
                    {
                        ::fast_io::operations::print_freestanding<false>(
                            ::std::forward<s>(stm),
                            u" <",
                            ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t.custom_name_begin, t.custom_name_end)),
                            u">");
                    }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
                }
            }

            if(wasmmod.tablesec.sec_begin) [[likely]]
            {
                // Table
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u"\n" u"table[",
                                                                 wasmmod.tablesec.table_count,
                                                                 u"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.tablesec.sec_begin - wasm_file_begin),
                                                                 u" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.tablesec.sec_end - wasm_file_begin),
                                                                 u" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.tablesec.sec_end - wasmmod.tablesec.sec_begin),
                                                                 u"):\n");

                for(::std::size_t count{wasmmod.importsec.table_types.size()}; auto const& t: wasmmod.tablesec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                u" - "
                                                                u"table"
                                                                u"[",
                                                                count++,
                                                                u"] type=",
                                                                t.elem_type,
                                                                u" initial=",
                                                                t.limits.min);
                    if(t.limits.present_max) { ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u" max=", t.limits.max); }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
                }
            }

            if(wasmmod.memorysec.sec_begin) [[likely]]
            {
                // Table
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u"\n" u"memory[",
                                                                 wasmmod.memorysec.memory_count,
                                                                 u"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.memorysec.sec_begin - wasm_file_begin),
                                                                 u" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.memorysec.sec_end - wasm_file_begin),
                                                                 u" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.memorysec.sec_end - wasmmod.memorysec.sec_begin),
                                                                 u"):\n");

                for(::std::size_t count{wasmmod.importsec.memory_types.size()}; auto const& t: wasmmod.memorysec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u" - " u"memory" u"[", count++, u"] initial=", t.limits.min);
                    if(t.limits.present_max) { ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u" max=", t.limits.max); }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
                }
            }

            if(wasmmod.globalsec.sec_begin) [[likely]]
            {
                // Table
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u"\n" u"global[",
                                                                 wasmmod.globalsec.global_count,
                                                                 u"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.globalsec.sec_begin - wasm_file_begin),
                                                                 u" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.globalsec.sec_end - wasm_file_begin),
                                                                 u" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.globalsec.sec_end - wasmmod.globalsec.sec_begin),
                                                                 u"):\n");

                for(::std::size_t count{wasmmod.importsec.global_types.size()}; auto const& t: wasmmod.globalsec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                u" - "
                                                                u"globau"
                                                                u"[",
                                                                count++,
                                                                u"] ",
                                                                t.type.type,
                                                                u" mutable=",
                                                                t.type.is_mutable);
                    if((t.name_end - t.name_begin) != 0)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                         u" <",
                                                                         ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t.name_begin, t.name_end)),
                                                                         u">");
                    }
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u" - init ");

                    switch(t.initializer.type_opcode.op)
                    {
                        case ::uwvm::wasm::op_basic::global_get:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u"global_get=global[", t.initializer.ref, u"]");
                            break;
                        }
                        case ::uwvm::wasm::op_basic::i32_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u"i32=", t.initializer.i32);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::i64_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u"i64=", t.initializer.i64);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::f32_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u"f32=", t.initializer.f32);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::f64_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u"f64=", t.initializer.f64);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::ref_null:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u"ref_null=", t.initializer.null_reftype);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::ref_func:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u"ref_func=function[", t.initializer.ref, u"]");
                            break;
                        }
                        case ::uwvm::wasm::op_basic::simd:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u"v128=i8x16: ", i8x16(t.initializer.v128));
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
                                                                 u"\n" u"Export[",
                                                                 wasmmod.exportsec.export_count,
                                                                 u"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.exportsec.sec_begin - wasm_file_begin),
                                                                 u" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.exportsec.sec_end - wasm_file_begin),
                                                                 u" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.exportsec.sec_end - wasmmod.exportsec.sec_begin),
                                                                 u"):\n");

                // func

                for(auto const t: wasmmod.exportsec.func_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     u" - "
                                                                     u"func" 
                                                                     u"[",
                                                                     t->index, 
                                                                     u"] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                     u"\"\n");
                }

                // table
                for(auto const t: wasmmod.exportsec.table_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     u" - "
                                                                     u"table" 
                                                                     u"[",
                                                                     t->index, 
                                                                     u"] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                     u"\"\n");
                }

                // memory
                for(auto const t: wasmmod.exportsec.memory_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     u" - "
                                                                     u"memory" 
                                                                     u"[",
                                                                     t->index, 
                                                                     u"] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                     u"\"\n");
                }

                // global
                for(auto const t: wasmmod.exportsec.global_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     u" - "
                                                                     u"globau" 
                                                                     u"[",
                                                                     t->index, 
                                                                     u"] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                    u"\"\n");
                }

                // tag
                for(auto const t: wasmmod.exportsec.tag_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     u" - "
                                                                     u"tag" 
                                                                     u"[",
                                                                     t->index, 
                                                                     u"] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                     u"\"\n");
                }
            }

            // start
            if(wasmmod.startsec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u"\n" "start",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.startsec.sec_begin - wasm_file_begin),
                                                                 u" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.startsec.sec_end - wasm_file_begin),
                                                                 u" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.startsec.sec_end - wasmmod.startsec.sec_begin),
                                                                 u"):\n" 
                                                                 u" - "
                                                                 u"func[",
                                                                 wasmmod.startsec.index,
                                                                 u"]\n");
            }

            // element
            if(wasmmod.elemsec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u"\n" u"Elem[",
                                                                 wasmmod.elemsec.elem_segment_count,
                                                                 u"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.elemsec.sec_begin - wasm_file_begin),
                                                                 u" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.elemsec.sec_end - wasm_file_begin),
                                                                 u" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.elemsec.sec_end - wasmmod.elemsec.sec_begin),
                                                                 u"):\n");

                // segment
                for(::std::size_t segment_counter{}; auto const& t: wasmmod.elemsec.elem_segments)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    u" - segment[",
                                                                    segment_counter++,
                                                                    u"] flags=0 table=",
                                                                    t.index,
                                                                    u" count=",
                                                                    t.elem_count,
                                                                    u" - init i32=",
                                                                    t.initializer.i32);
                    // elems
                    for(::std::size_t elem_counter{}; auto const e: t.elems)
                    {
                        ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u"  - elem[", elem_counter++, u"] = ref.func:", e);
                    }
                }
            }

            // code
            if(wasmmod.codesec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u"\n" u"Code[",
                                                                 wasmmod.codesec.code_count,
                                                                 u"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.codesec.sec_begin - wasm_file_begin),
                                                                 u" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.codesec.sec_end - wasm_file_begin),
                                                                 u" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.codesec.sec_end - wasmmod.codesec.sec_begin),
                                                                 u"):\n");

                for(::std::size_t code_count{wasmmod.importsec.func_types.size()}; auto const& t: wasmmod.codesec.bodies)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u" - " u"func" u"[", code_count++, u"] size=", t.body_size, u" local=", t.local_count);
                }
            }

            // data
            if(wasmmod.datasec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u"\n" u"Data[",
                                                                 wasmmod.datasec.count,
                                                                 u"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.datasec.sec_begin - wasm_file_begin),
                                                                 u" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.datasec.sec_end - wasm_file_begin),
                                                                 u" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.datasec.sec_end - wasmmod.datasec.sec_begin),
                                                                 u"):\n");

                for(::std::size_t count{}; auto const& t: wasmmod.datasec.entries)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    u" - " u"segment" u"[", 
                                                                    count++,
                                                                    u"] memory=",
                                                                    t.index,
                                                                    u" size=",
                                                                    t.size,
                                                                    u" - init i32=",
                                                                    t.offset.i32);
                }
            }

            // custom
            for(auto const& t: wasmmod.cussecs)
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                    u"\n"
                                                                    u"Custom:\n"
                                                                    u" - name: \"", 
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t.name_begin,t.name_end)), 
                                                                    u"\"\n");
            }
        }
        else if constexpr(::std::same_as<char_type, char32_t>)
        {
            // objdump
            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                             // L0
                                                             U"\n",
                                                             // L1
                                                             ::fast_io::mnp::code_cvt(wasmmod.module_name),
                                                             U":\n"
                                                             // L2
                                                             U"file format wasm ",
                                                             ::fast_io::mnp::hex0x(wasmmod.wasm_version),
                                                             U"\n"
                                                             // L3
                                                             U"Section Details:"
                                                             // L4 - L5
                                                             U"\n\n");

            if(wasmmod.typesec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 U"Type[",
                                                                 wasmmod.typesec.type_count,
                                                                 U"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.typesec.sec_begin - wasm_file_begin),
                                                                 U" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.typesec.sec_end - wasm_file_begin),
                                                                 U" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.typesec.sec_end - wasmmod.typesec.sec_begin),
                                                                 U"):\n");

                // Type
                for(::std::size_t count{}; auto const& t: wasmmod.typesec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), U" - type[", count++, U"] (");
                    if(t.parameter_begin != t.parameter_end)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), *t.parameter_begin);
                        for(auto curr_para{t.parameter_begin + 1}; curr_para < t.parameter_end; ++curr_para)
                        {
                            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), U", ", *curr_para);
                        }
                    }

                    if(t.result_begin == t.result_end) { ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), U") -> niU"); }
                    else if(t.result_end - t.result_begin == 1)
                    {
                        ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), U") -> ", *t.result_begin);
                    }
                    else
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), U") -> (", *t.result_begin);
                        for(auto curr_res{t.result_begin + 1}; curr_res < t.result_end; ++curr_res)
                        {
                            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), U", ", *curr_res);
                        }
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), U")\n");
                    }
                }
            }

            auto const func_type_table_base{wasmmod.typesec.types.cbegin()};

            if(wasmmod.importsec.sec_begin) [[likely]]
            {
                // Import
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 U"\n" U"Import[",
                                                                 wasmmod.importsec.import_count,
                                                                 U"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.importsec.sec_begin - wasm_file_begin),
                                                                 U" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.importsec.sec_end - wasm_file_begin),
                                                                 U" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.importsec.sec_end - wasmmod.importsec.sec_begin),
                                                                 U"):\n");

                // func
                for(::std::size_t count{}; auto const t: wasmmod.importsec.func_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                U" - "
                                                                U"func"
                                                                U"[",
                                                                count++,
                                                                U"] sig=",
                                                                t->extern_type.function - func_type_table_base);
                    if((t->custom_name_end - t->custom_name_begin) != 0)
                    {
                        ::fast_io::operations::print_freestanding<false>(
                            ::std::forward<s>(stm),
                            U" <",
                            ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->custom_name_begin, t->custom_name_end)),
                            U">");
                    }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    U" <- ",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                    U".",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }

                // table
                for(::std::size_t count{}; auto const t: wasmmod.importsec.table_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                U" - "
                                                                U"table"
                                                                U"[",
                                                                count++,
                                                                U"] type=",
                                                                t->extern_type.table.elem_type,
                                                                U" initial=",
                                                                t->extern_type.table.limits.min);
                    if(t->extern_type.table.limits.present_max)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), U" max=", t->extern_type.table.limits.max);
                    }

                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    U" <- ",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                    U".",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }

                // memory
                for(::std::size_t count{}; auto const t: wasmmod.importsec.memory_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                U" - "
                                                                U"memory"
                                                                U"[",
                                                                count++,
                                                                U"] initial=",
                                                                t->extern_type.memory.limits.min);
                    if(t->extern_type.memory.limits.present_max)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), U" max=", t->extern_type.memory.limits.max);
                    }

                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    U" <- ",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                    U".",
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }

                // global
                for(::std::size_t count{}; auto const t: wasmmod.importsec.global_types)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                U" - "
                                                                U"globaU"
                                                                U"[",
                                                                count++,
                                                                U"] ",
                                                                t->extern_type.global.type,
                                                                U" mutable=",
                                                                t->extern_type.global.is_mutable,
                                                                U" <- ",
                                                                ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                U".",
                                                                ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }

                // tag
                for(::std::size_t count{}; auto const t: wasmmod.importsec.tag_types)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                U" - "
                                                                U"tag"
                                                                U"[",
                                                                count++,
                                                                U"] attribute=",
                                                                ::fast_io::mnp::hex0x<true>(t->extern_type.exception.attribute),
                                                                U" type=",
                                                                t->extern_type.exception.type - func_type_table_base,
                                                                U" <- ",
                                                                ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->module_begin, t->module_end)),
                                                                U".",
                                                                ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)));
                }
            }

            if(wasmmod.functionsec.sec_begin) [[likely]]
            {
                // Function
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 U"\n" "Function[",
                                                                 wasmmod.functionsec.function_count,
                                                                 U"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.functionsec.sec_begin - wasm_file_begin),
                                                                 U" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.functionsec.sec_end - wasm_file_begin),
                                                                 U" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.functionsec.sec_end - wasmmod.functionsec.sec_begin),
                                                                 U"):\n");

                for(::std::size_t count{wasmmod.importsec.func_types.size()}; auto const& t: wasmmod.functionsec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                U" - "
                                                                U"func"
                                                                U"[",
                                                                count++,
                                                                U"] sig=",
                                                                t.func_type - func_type_table_base);
                    if((t.custom_name_end - t.custom_name_begin) != 0)
                    {
                        ::fast_io::operations::print_freestanding<false>(
                            ::std::forward<s>(stm),
                            U" <",
                            ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t.custom_name_begin, t.custom_name_end)),
                            U">");
                    }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
                }
            }

            if(wasmmod.tablesec.sec_begin) [[likely]]
            {
                // Table
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 U"\n" U"table[",
                                                                 wasmmod.tablesec.table_count,
                                                                 U"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.tablesec.sec_begin - wasm_file_begin),
                                                                 U" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.tablesec.sec_end - wasm_file_begin),
                                                                 U" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.tablesec.sec_end - wasmmod.tablesec.sec_begin),
                                                                 U"):\n");

                for(::std::size_t count{wasmmod.importsec.table_types.size()}; auto const& t: wasmmod.tablesec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                U" - "
                                                                U"table"
                                                                U"[",
                                                                count++,
                                                                U"] type=",
                                                                t.elem_type,
                                                                U" initial=",
                                                                t.limits.min);
                    if(t.limits.present_max) { ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), U" max=", t.limits.max); }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
                }
            }

            if(wasmmod.memorysec.sec_begin) [[likely]]
            {
                // Table
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 U"\n" U"memory[",
                                                                 wasmmod.memorysec.memory_count,
                                                                 U"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.memorysec.sec_begin - wasm_file_begin),
                                                                 U" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.memorysec.sec_end - wasm_file_begin),
                                                                 U" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.memorysec.sec_end - wasmmod.memorysec.sec_begin),
                                                                 U"):\n");

                for(::std::size_t count{wasmmod.importsec.memory_types.size()}; auto const& t: wasmmod.memorysec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), U" - " U"memory" U"[", count++, U"] initial=", t.limits.min);
                    if(t.limits.present_max) { ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), U" max=", t.limits.max); }
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
                }
            }

            if(wasmmod.globalsec.sec_begin) [[likely]]
            {
                // Table
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 U"\n" U"global[",
                                                                 wasmmod.globalsec.global_count,
                                                                 U"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.globalsec.sec_begin - wasm_file_begin),
                                                                 U" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.globalsec.sec_end - wasm_file_begin),
                                                                 U" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.globalsec.sec_end - wasmmod.globalsec.sec_begin),
                                                                 U"):\n");

                for(::std::size_t count{wasmmod.importsec.global_types.size()}; auto const& t: wasmmod.globalsec.types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                U" - "
                                                                U"globaU"
                                                                U"[",
                                                                count++,
                                                                U"] ",
                                                                t.type.type,
                                                                U" mutable=",
                                                                t.type.is_mutable);
                    if((t.name_end - t.name_begin) != 0)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                         U" <",
                                                                         ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t.name_begin, t.name_end)),
                                                                         U">");
                    }
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), U" - init ");

                    switch(t.initializer.type_opcode.op)
                    {
                        case ::uwvm::wasm::op_basic::global_get:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), U"global_get=global[", t.initializer.ref, U"]");
                            break;
                        }
                        case ::uwvm::wasm::op_basic::i32_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), U"i32=", t.initializer.i32);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::i64_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), U"i64=", t.initializer.i64);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::f32_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), U"f32=", t.initializer.f32);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::f64_const:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), U"f64=", t.initializer.f64);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::ref_null:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), U"ref_null=", t.initializer.null_reftype);
                            break;
                        }
                        case ::uwvm::wasm::op_basic::ref_func:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), U"ref_func=function[", t.initializer.ref, U"]");
                            break;
                        }
                        case ::uwvm::wasm::op_basic::simd:
                        {
                            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), U"v128=i8x16: ", i8x16(t.initializer.v128));
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
                                                                 U"\n" U"Export[",
                                                                 wasmmod.exportsec.export_count,
                                                                 U"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.exportsec.sec_begin - wasm_file_begin),
                                                                 U" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.exportsec.sec_end - wasm_file_begin),
                                                                 U" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.exportsec.sec_end - wasmmod.exportsec.sec_begin),
                                                                 U"):\n");

                // func

                for(auto const t: wasmmod.exportsec.func_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     U" - "
                                                                     U"func" 
                                                                     U"[",
                                                                     t->index, 
                                                                     U"] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                     U"\"\n");
                }

                // table
                for(auto const t: wasmmod.exportsec.table_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     U" - "
                                                                     U"table" 
                                                                     U"[",
                                                                     t->index, 
                                                                     U"] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                     U"\"\n");
                }

                // memory
                for(auto const t: wasmmod.exportsec.memory_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     U" - "
                                                                     U"memory" 
                                                                     U"[",
                                                                     t->index, 
                                                                     U"] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                     U"\"\n");
                }

                // global
                for(auto const t: wasmmod.exportsec.global_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     U" - "
                                                                     U"globaU" 
                                                                     U"[",
                                                                     t->index, 
                                                                     U"] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                    U"\"\n");
                }

                // tag
                for(auto const t: wasmmod.exportsec.tag_types)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), 
                                                                     U" - "
                                                                     U"tag" 
                                                                     U"[",
                                                                     t->index, 
                                                                     U"] -> \"",
                                                                     ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t->name_begin, t->name_end)),
                                                                     U"\"\n");
                }
            }

            // start
            if(wasmmod.startsec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 U"\n" "start",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.startsec.sec_begin - wasm_file_begin),
                                                                 U" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.startsec.sec_end - wasm_file_begin),
                                                                 U" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.startsec.sec_end - wasmmod.startsec.sec_begin),
                                                                 U"):\n" 
                                                                 U" - "
                                                                 U"func[",
                                                                 wasmmod.startsec.index,
                                                                 U"]\n");
            }

            // element
            if(wasmmod.elemsec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 U"\n" U"Elem[",
                                                                 wasmmod.elemsec.elem_segment_count,
                                                                 U"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.elemsec.sec_begin - wasm_file_begin),
                                                                 U" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.elemsec.sec_end - wasm_file_begin),
                                                                 U" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.elemsec.sec_end - wasmmod.elemsec.sec_begin),
                                                                 U"):\n");

                // segment
                for(::std::size_t segment_counter{}; auto const& t: wasmmod.elemsec.elem_segments)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    U" - segment[",
                                                                    segment_counter++,
                                                                    U"] flags=0 table=",
                                                                    t.index,
                                                                    U" count=",
                                                                    t.elem_count,
                                                                    U" - init i32=",
                                                                    t.initializer.i32);
                    // elems
                    for(::std::size_t elem_counter{}; auto const e: t.elems)
                    {
                        ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), U"  - elem[", elem_counter++, U"] = ref.func:", e);
                    }
                }
            }

            // code
            if(wasmmod.codesec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 U"\n" U"Code[",
                                                                 wasmmod.codesec.code_count,
                                                                 U"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.codesec.sec_begin - wasm_file_begin),
                                                                 U" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.codesec.sec_end - wasm_file_begin),
                                                                 U" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.codesec.sec_end - wasmmod.codesec.sec_begin),
                                                                 U"):\n");

                for(::std::size_t code_count{wasmmod.importsec.func_types.size()}; auto const& t: wasmmod.codesec.bodies)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), U" - " U"func" U"[", code_count++, U"] size=", t.body_size, U" local=", t.local_count);
                }
            }

            // data
            if(wasmmod.datasec.sec_begin) [[likely]]
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 U"\n" U"Data[",
                                                                 wasmmod.datasec.count,
                                                                 U"] (start=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.datasec.sec_begin - wasm_file_begin),
                                                                 U" end=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.datasec.sec_end - wasm_file_begin),
                                                                 U" size=",
                                                                 ::fast_io::mnp::hex0x<true>(wasmmod.datasec.sec_end - wasmmod.datasec.sec_begin),
                                                                 U"):\n");

                for(::std::size_t count{}; auto const& t: wasmmod.datasec.entries)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                    U" - " U"segment" U"[", 
                                                                    count++,
                                                                    U"] memory=",
                                                                    t.index,
                                                                    U" size=",
                                                                    t.size,
                                                                    U" - init i32=",
                                                                    t.offset.i32);
                }
            }

            // custom
            for(auto const& t: wasmmod.cussecs)
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                    U"\n"
                                                                    U"Custom:\n"
                                                                    U" - name: \"", 
                                                                    ::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(t.name_begin,t.name_end)), 
                                                                    U"\"\n");
            }
        }

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
