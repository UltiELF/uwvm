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
            return 48u;
        }

        template <::std::integral char_type>
        inline constexpr char_type* print_reserve_v128_impl(char_type* iter, v128_print_struct v128_val) noexcept
        {
            using print_alias_hexU8_type = decltype(::fast_io::io_print_alias(::fast_io::mnp::hexupper<false, true>(::std::uint_least8_t{})));
            constexpr auto space{::fast_io::char_literal_v<u8' ', char_type>};
            auto const v128_i8x16 = ::std::bit_cast<::fast_io::array<::std::uint_least8_t, 16u>>(v128_val);
            char_type* curr_pos{iter};
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, print_alias_hexU8_type>,
                                            iter,
                                            ::fast_io::io_print_alias(::fast_io::mnp::hexupper<false, true>(v128_i8x16[0])));
            *(curr_pos++) = space;
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, print_alias_hexU8_type>,
                                            iter,
                                            ::fast_io::io_print_alias(::fast_io::mnp::hexupper<false, true>(v128_i8x16[1])));
            *(curr_pos++) = space;
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, print_alias_hexU8_type>,
                                            iter,
                                            ::fast_io::io_print_alias(::fast_io::mnp::hexupper<false, true>(v128_i8x16[2])));
            *(curr_pos++) = space;
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, print_alias_hexU8_type>,
                                            iter,
                                            ::fast_io::io_print_alias(::fast_io::mnp::hexupper<false, true>(v128_i8x16[3])));
            *(curr_pos++) = space;
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, print_alias_hexU8_type>,
                                            iter,
                                            ::fast_io::io_print_alias(::fast_io::mnp::hexupper<false, true>(v128_i8x16[4])));
            *(curr_pos++) = space;
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, print_alias_hexU8_type>,
                                            iter,
                                            ::fast_io::io_print_alias(::fast_io::mnp::hexupper<false, true>(v128_i8x16[5])));
            *(curr_pos++) = space;
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, print_alias_hexU8_type>,
                                            iter,
                                            ::fast_io::io_print_alias(::fast_io::mnp::hexupper<false, true>(v128_i8x16[6])));
            *(curr_pos++) = space;
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, print_alias_hexU8_type>,
                                            iter,
                                            ::fast_io::io_print_alias(::fast_io::mnp::hexupper<false, true>(v128_i8x16[7])));
            *(curr_pos++) = space;
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, print_alias_hexU8_type>,
                                            iter,
                                            ::fast_io::io_print_alias(::fast_io::mnp::hexupper<false, true>(v128_i8x16[8])));
            *(curr_pos++) = space;
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, print_alias_hexU8_type>,
                                            iter,
                                            ::fast_io::io_print_alias(::fast_io::mnp::hexupper<false, true>(v128_i8x16[9])));
            *(curr_pos++) = space;
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, print_alias_hexU8_type>,
                                            iter,
                                            ::fast_io::io_print_alias(::fast_io::mnp::hexupper<false, true>(v128_i8x16[10])));
            *(curr_pos++) = space;
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, print_alias_hexU8_type>,
                                            iter,
                                            ::fast_io::io_print_alias(::fast_io::mnp::hexupper<false, true>(v128_i8x16[11])));
            *(curr_pos++) = space;
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, print_alias_hexU8_type>,
                                            iter,
                                            ::fast_io::io_print_alias(::fast_io::mnp::hexupper<false, true>(v128_i8x16[12])));
            *(curr_pos++) = space;
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, print_alias_hexU8_type>,
                                            iter,
                                            ::fast_io::io_print_alias(::fast_io::mnp::hexupper<false, true>(v128_i8x16[13])));
            *(curr_pos++) = space;
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, print_alias_hexU8_type>,
                                            iter,
                                            ::fast_io::io_print_alias(::fast_io::mnp::hexupper<false, true>(v128_i8x16[14])));
            *(curr_pos++) = space;
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, print_alias_hexU8_type>,
                                            iter,
                                            ::fast_io::io_print_alias(::fast_io::mnp::hexupper<false, true>(v128_i8x16[15])));
            return curr_pos;
        }

        template <::std::integral char_type>
        inline constexpr char_type* print_reserve_define(::fast_io::io_reserve_type_t<char_type, v128_print_struct>, char_type* iter, v128_print_struct v128_val) noexcept
        {
            return print_reserve_v128_impl(iter, v128_val);
        }

    }  // namespace details

    inline constexpr details::v128_print_struct i8x16(::uwvm::wasm::wasm_v128 v128_val) noexcept { return details::v128_print_struct{v128_val}; }

}  // namespace uwvm::wasm
