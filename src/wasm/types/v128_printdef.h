#pragma once
#include <fast_io.h>
#include <fast_io_dsal/array.h>
#include "basic_value.h"

namespace uwvm::wasm
{
    namespace details
    {
        struct v128_print_struct
        {
            ::uwvm::wasm::wasm_v128 v128{};
        };

        template <::std::integral char_type>
        inline constexpr ::std::size_t print_reserve_size(::fast_io::io_reserve_type_t<char_type, v128_print_struct>) noexcept
        {
            return 50u;
        }

        template <::std::integral char_type>
        inline constexpr char_type* print_reserve_v128_impl(char_type* iter, v128_print_struct v128_val) noexcept
        {
            constexpr auto space{::fast_io::char_literal_v<u8' ', char_type>};
            auto const v128_i8x16 = ::std::bit_cast<::fast_io::array<::std::uint_least8_t, 16u>>(v128_val);
            char_type* curr_pos{iter};
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, ::std::uint_least8_t>, iter, ::fast_io::mnp::hexupper<false, true>(v128_i64x2[0]));
            ::fast_io::println
            return curr_pos;
        }
    }  // namespace details

}  // namespace uwvm::wasm
