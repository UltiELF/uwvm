#pragma once
#include <cstddef>
#include <limits>
#include <fast_io.h>
#ifdef UWVM_TIMER
    #include <fast_io_driver/timer.h>
#endif
#include <io_device.h>

#include "../wasm/section.h"
#include "../wasm_file.h"

namespace uwvm
{
    namespace details
    {
        struct objdump_printer
        {
        };

        template <::std::integral char_type, typename s>
        inline constexpr void print_define(::fast_io::io_reserve_type_t<char_type, objdump_printer>, s&& stm, objdump_printer) noexcept
        {
            // wasm file
            auto const wasm_file_begin{reinterpret_cast<::std::byte const*>(::uwvm::wasm_file_loader.cbegin())};
            auto const wasm_file_end{reinterpret_cast<::std::byte const*>(::uwvm::wasm_file_loader.cend())};

            if constexpr(::std::same_as<char_type, char>) {}
            else if constexpr(::std::same_as<char_type, wchar_t>) {}
            else if constexpr(::std::same_as<char_type, char8_t>)
            {
                // objdump
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 // L0
                                                                 u8"\n",
                                                                 // L1
                                                                 ::fast_io::mnp::code_cvt(::uwvm::wasm_file_name),
                                                                 u8":\n"
                                                                 // L2
                                                                 u8"file format wasm ",
                                                                 ::fast_io::mnp::hex0x(::uwvm::global_wasm_module.wasm_version),
                                                                 u8"\n"
                                                                 // L3
                                                                 u8"Section Details:"
                                                                 // L4 - L5
                                                                 u8"\n\n");

                if(::uwvm::global_wasm_module.typesec.sec_begin) [[likely]]
                {
                    ::fast_io::operations::print_freestanding<false>(
                        ::std::forward<s>(stm),
                        u8"Type[",
                        ::uwvm::global_wasm_module.typesec.type_count,
                        u8"] (start=",
                        ::fast_io::mnp::hex0x<true>(::uwvm::global_wasm_module.typesec.sec_begin - wasm_file_begin),
                        u8" end=",
                        ::fast_io::mnp::hex0x<true>(::uwvm::global_wasm_module.typesec.sec_end - wasm_file_begin),
                        u8" size=",
                        ::fast_io::mnp::hex0x<true>(::uwvm::global_wasm_module.typesec.sec_end - ::uwvm::global_wasm_module.typesec.sec_begin),
                        u8"):\n");

                    // Type
                    for(::std::size_t count{}; auto const& t: ::uwvm::global_wasm_module.typesec.types)
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

                auto const func_type_table_base{::uwvm::global_wasm_module.typesec.types.cbegin()};

                if(::uwvm::global_wasm_module.importsec.sec_begin) [[likely]]
                {
                    // Import
                    ::fast_io::operations::print_freestanding<false>(
                        ::std::forward<s>(stm),
                        u8"\n" u8"Import[",
                        ::uwvm::global_wasm_module.importsec.import_count,
                        u8"] (start=",
                        ::fast_io::mnp::hex0x<true>(::uwvm::global_wasm_module.importsec.sec_begin - wasm_file_begin),
                        u8" end=",
                        ::fast_io::mnp::hex0x<true>(::uwvm::global_wasm_module.importsec.sec_end - wasm_file_begin),
                        u8" size=",
                        ::fast_io::mnp::hex0x<true>(::uwvm::global_wasm_module.importsec.sec_end - ::uwvm::global_wasm_module.importsec.sec_begin),
                        u8"):\n");

                    // func
                    for(::std::size_t count{}; auto const t: ::uwvm::global_wasm_module.importsec.func_types)
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
                    for(::std::size_t count{}; auto const t: ::uwvm::global_wasm_module.importsec.table_types)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                u8" - "
                                                                u8"table"
                                                                u8"[",
                                                                count++,
                                                                u8"] type=",
                                                                t->extern_type.table.elem_type,
                                                                u8" initial=",
                                                                t->extern_type.table.limit.min);
                        if(t->extern_type.table.limit.present_max)
                        {
                            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8" max=", t->extern_type.table.limit.max);
                        }

                        ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                        u8" <- ",
                                                                        ::fast_io::mnp::strvw(t->module_begin, t->module_end),
                                                                        u8".",
                                                                        ::fast_io::mnp::strvw(t->name_begin, t->name_end));
                    }

                    // memory
                    for(::std::size_t count{}; auto const t: ::uwvm::global_wasm_module.importsec.memory_types)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                u8" - "
                                                                u8"memory"
                                                                u8"[",
                                                                count++,
                                                                u8"] initial=",
                                                                t->extern_type.memory.mem_limit.min);
                        if(t->extern_type.memory.mem_limit.present_max)
                        {
                            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8" max=", t->extern_type.memory.mem_limit.max);
                        }

                        ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                        u8" <- ",
                                                                        ::fast_io::mnp::strvw(t->module_begin, t->module_end),
                                                                        u8".",
                                                                        ::fast_io::mnp::strvw(t->name_begin, t->name_end));
                    }

                    // global
                    for(::std::size_t count{}; auto const t: ::uwvm::global_wasm_module.importsec.global_types)
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
                    for(::std::size_t count{}; auto const t: ::uwvm::global_wasm_module.importsec.tag_types)
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

                if(::uwvm::global_wasm_module.functionsec.sec_begin) [[likely]]
                {
                    // Function
                    ::fast_io::operations::print_freestanding<false>(
                        ::std::forward<s>(stm),
                        u8"\n" u8"Function[",
                        ::uwvm::global_wasm_module.functionsec.function_count,
                        u8"] (start=",
                        ::fast_io::mnp::hex0x<true>(::uwvm::global_wasm_module.functionsec.sec_begin - wasm_file_begin),
                        u8" end=",
                        ::fast_io::mnp::hex0x<true>(::uwvm::global_wasm_module.functionsec.sec_end - wasm_file_begin),
                        u8" size=",
                        ::fast_io::mnp::hex0x<true>(::uwvm::global_wasm_module.functionsec.sec_end - ::uwvm::global_wasm_module.functionsec.sec_begin),
                        u8"):\n");

                    for(::std::size_t count{::uwvm::global_wasm_module.importsec.func_types.size()};
                        auto const& t: ::uwvm::global_wasm_module.functionsec.types)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                u8" - "
                                                                u8"func"
                                                                u8"[",
                                                                count++,
                                                                u8"] sig=",
                                                                t.func_type - func_type_table_base,
                                                                u8" <",
                                                                ::fast_io::mnp::strvw(t.name_begin, t.name_end),
                                                                u8">\n");
                    }
                }

                if(::uwvm::global_wasm_module.tablesec.sec_begin) [[likely]]
                {
                    // Table
                    ::fast_io::operations::print_freestanding<false>(
                        ::std::forward<s>(stm),
                        u8"\n" u8"table[",
                        ::uwvm::global_wasm_module.tablesec.table_count,
                        u8"] (start=",
                        ::fast_io::mnp::hex0x<true>(::uwvm::global_wasm_module.tablesec.sec_begin - wasm_file_begin),
                        u8" end=",
                        ::fast_io::mnp::hex0x<true>(::uwvm::global_wasm_module.tablesec.sec_end - wasm_file_begin),
                        u8" size=",
                        ::fast_io::mnp::hex0x<true>(::uwvm::global_wasm_module.tablesec.sec_end - ::uwvm::global_wasm_module.tablesec.sec_begin),
                        u8"):\n");

                    for(::std::size_t count{}; auto const& t: ::uwvm::global_wasm_module.tablesec.types)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                u8" - "
                                                                u8"table"
                                                                u8"[",
                                                                count++,
                                                                u8"] type=",
                                                                t.elem_type,
                                                                u8" initial=",
                                                                t.limit.min);
                        if(t.limit.present_max) { ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8" max=", t.limit.max); }
                        ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
                    }
                }
            }
            else if constexpr(::std::same_as<char_type, char16_t>) {}
            else if constexpr(::std::same_as<char_type, char32_t>) {}

            // ln
            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
        }
    }  // namespace details

    inline void u8objdump() noexcept
    {
        ::fast_io::basic_obuf<decltype(::uwvm::u8out)> u8outbuf{::uwvm::u8out};
#ifdef UWVM_TIMER
        ::fast_io::timer objdump_timer{u8"uwvm: [timer] objdump"};
#endif
        ::fast_io::print(u8outbuf, details::objdump_printer{});
    }
}  // namespace uwvm
