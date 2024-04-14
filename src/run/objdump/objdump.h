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

        template <typename s>
        inline constexpr void print_define(::fast_io::io_reserve_type_t<char8_t, objdump_printer>, s&& stm, objdump_printer) noexcept
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

            // wasm file
            auto const wasm_file_begin{reinterpret_cast<::std::byte const*>(::uwvm::wasm_file_loader.cbegin())};
            auto const wasm_file_end{reinterpret_cast<::std::byte const*>(::uwvm::wasm_file_loader.cend())};

            // objdump
            ::fast_io::operations::print_freestanding<false>(
                ::std::forward<s>(stm),
                // L0
                u8"\n",
                // L1
                ::fast_io::mnp::code_cvt(::uwvm::wasm_file_name),
                u8":\n"
                // L2
                u8"file format wasm ",
                ::fast_io::mnp::hex0x(::uwvm::wasm_version),
                u8"\n"
                // L3
                u8"Section Details:"
                // L4 - L5
                u8"\n\n"
                // L6
                u8"Type[",
                ::uwvm::global_type_section.type_count,
                u8"] (start=",
                ::fast_io::mnp::hex0x<true>(::uwvm::global_type_section.sec_begin - wasm_file_begin),
                u8" end=",
                ::fast_io::mnp::hex0x<true>(::uwvm::global_type_section.sec_end - wasm_file_begin),
                u8" size=",
                ::fast_io::mnp::hex0x<true>(::uwvm::global_type_section.sec_end - ::uwvm::global_type_section.sec_begin),
                u8"):\n");

            // Type
            for(::std::size_t count{}; auto const& t: ::uwvm::global_type_section.types)
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8" - type[", count++, u8"] (");
                if(t.parameter_begin != t.parameter_end)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), ::uwvm::wasm::get_value_u8name(*t.parameter_begin));
                    for(auto curr_para{t.parameter_begin + 1}; curr_para < t.parameter_end; ++curr_para)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8", ", ::uwvm::wasm::get_value_u8name(*curr_para));
                    }
                }

                if(t.result_begin == t.result_end) { ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u8") -> nil"); }
                else if(t.result_end - t.result_begin == 1)
                {
                    ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm), u8") -> ", ::uwvm::wasm::get_value_u8name(*t.result_begin));
                }
                else
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8") -> (", ::uwvm::wasm::get_value_u8name(*t.result_begin));
                    for(auto curr_res{t.result_begin + 1}; curr_res < t.result_end; ++curr_res)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8", ", ::uwvm::wasm::get_value_u8name(*curr_res));
                    }
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8")\n");
                }
            }

            // Import
            ::fast_io::operations::print_freestanding<false>(
                ::std::forward<s>(stm),
                u8"\n" u8"Import[",
                ::uwvm::global_import_section.import_count,
                u8"] (start=",
                ::fast_io::mnp::hex0x<true>(::uwvm::global_import_section.sec_begin - wasm_file_begin),
                u8" end=",
                ::fast_io::mnp::hex0x<true>(::uwvm::global_import_section.sec_end - wasm_file_begin),
                u8" size=",
                ::fast_io::mnp::hex0x<true>(::uwvm::global_import_section.sec_end - ::uwvm::global_import_section.sec_begin),
                u8"):\n");

            // func
            auto const func_type_table_base{::uwvm::global_type_section.types.cbegin()};
            for(::std::size_t count{}; auto const t: ::uwvm::global_import_section.func_types)
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
            for(::std::size_t count{}; auto const t: ::uwvm::global_import_section.table_types)
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                u8" - "
                                                                u8"table"
                                                                u8"[",
                                                                count++,
                                                                u8"] type=",
                                                                ::uwvm::wasm::get_value_u8name(t->extern_type.table.elem_type),
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
            for(::std::size_t count{}; auto const t: ::uwvm::global_import_section.memory_types)
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
            for(::std::size_t count{}; auto const t: ::uwvm::global_import_section.global_types)
            {
                ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                u8" - "
                                                                u8"global"
                                                                u8"[",
                                                                count++,
                                                                u8"] ",
                                                                ::uwvm::wasm::get_value_u8name(t->extern_type.global.type),
                                                                u8" mutable=",
                                                                t->extern_type.global.is_mutable,
                                                                u8" <- ",
                                                                ::fast_io::mnp::strvw(t->module_begin, t->module_end),
                                                                u8".",
                                                                ::fast_io::mnp::strvw(t->name_begin, t->name_end));
            }

            // tag
            for(::std::size_t count{}; auto const t: ::uwvm::global_import_section.tag_types)
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

            // Function
            ::fast_io::operations::print_freestanding<false>(
                ::std::forward<s>(stm),
                u8"\n" u8"Function[",
                ::uwvm::global_function_section.function_count,
                u8"] (start=",
                ::fast_io::mnp::hex0x<true>(::uwvm::global_function_section.sec_begin - wasm_file_begin),
                u8" end=",
                ::fast_io::mnp::hex0x<true>(::uwvm::global_function_section.sec_end - wasm_file_begin),
                u8" size=",
                ::fast_io::mnp::hex0x<true>(::uwvm::global_function_section.sec_end - ::uwvm::global_function_section.sec_begin),
                u8"):\n");

            for(::std::size_t count{::uwvm::global_import_section.func_types.size()}; auto const& t: ::uwvm::global_function_section.types)
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

            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
        }
    }  // namespace details

    inline void u8objdump() noexcept
    {
        ::fast_io::basic_obuf<decltype(::uwvm::u8out)> u8outbuf{::uwvm::u8out};
#ifdef UWVM_TIMER
        ::fast_io::timer objdump_timer{u8"objdump"};
#endif
        ::fast_io::print(u8outbuf, details::objdump_printer{});
    }
}  // namespace uwvm
