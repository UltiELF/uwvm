#pragma once
#include <cstddef>
#include <limits>
#include <fast_io.h>
#include <io_device.h>

#include "../wasm/section.h"
#include "../wasm_file.h"

namespace uwvm
{
    namespace details
    {
        struct objdump
        {
        };

        template <typename s>
        inline constexpr void print_define(::fast_io::io_reserve_type_t<char8_t, objdump>, s&& stm, objdump) noexcept
        {
            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
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
                                                             u8"]:\n");
            // Type
            for(::std::size_t count{}; const auto& t: ::uwvm::global_type_section.types)
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
                if(t.result_begin != t.result_end)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8") -> ", ::uwvm::wasm::get_value_u8name(*t.result_begin));
                    for(auto curr_res{t.result_begin + 1}; curr_res < t.result_end; ++curr_res)
                    {
                        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8", ", ::uwvm::wasm::get_value_u8name(*curr_res));
                    }
                }
                else {::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8") -> nil"); }
                ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
            }

            // Import
            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8"\n" u8"Import[", ::uwvm::global_import_section.import_count, u8"]:\n");

            // func
            auto const func_type_table_base{__builtin_addressof(::uwvm::global_type_section.types.front_unchecked())};
            for(::std::size_t count{}; const auto t: ::uwvm::global_import_section.func_types)
            {
                ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
                                                                u8" - ",
                                                                ::uwvm::wasm::section::get_import_basic_u8name(t->type),
                                                                u8"[",
                                                                count++,
                                                                u8"] sig = ",
                                                                t->func_index - func_type_table_base,
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
            ::fast_io::fast_terminate(); // todo
            
            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm));
        }
    }  // namespace details

    inline void u8objdump() noexcept
    {
        ::fast_io::basic_obuf<decltype(::uwvm::u8out)> u8outbuf{::uwvm::u8out};
        ::fast_io::print(u8outbuf, details::objdump{});
    }
}  // namespace uwvm
