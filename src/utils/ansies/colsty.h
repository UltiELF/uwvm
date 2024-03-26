#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <concepts>

#include <fast_io.h>

namespace uwvm
{

    namespace ansi_escape_sequences
    {

        /**
         * @brief It is used to store the colors. Note: "bg" is the
         * prefix of the background color features and "bd" is the
         * one of the bold color features.
         */
        struct col
        {
            // Color variables:
            constexpr inline static decltype(auto) black{u8"\033[30m"};
            constexpr inline static decltype(auto) red{u8"\033[31m"};
            constexpr inline static decltype(auto) green{u8"\033[32m"};
            constexpr inline static decltype(auto) orange{u8"\033[33m"};
            constexpr inline static decltype(auto) blue{u8"\033[34m"};
            constexpr inline static decltype(auto) purple{u8"\033[35m"};
            constexpr inline static decltype(auto) cyan{u8"\033[36m"};
            constexpr inline static decltype(auto) gray{u8"\033[37m"};

#ifdef __MSDOS__
            constexpr inline static decltype(auto) dk_gray{black};
            constexpr inline static decltype(auto) lt_red{red};
            constexpr inline static decltype(auto) lt_green{green};
            constexpr inline static decltype(auto) yellow{orange};
            constexpr inline static decltype(auto) lt_blue{blue};
            constexpr inline static decltype(auto) lt_purple{purple};
            constexpr inline static decltype(auto) lt_cyan{cyan};
            constexpr inline static decltype(auto) white{gray};
#else
            constexpr inline static decltype(auto) dk_gray{u8"\033[90m"};
            constexpr inline static decltype(auto) lt_red{u8"\033[91m"};
            constexpr inline static decltype(auto) lt_green{u8"\033[92m"};
            constexpr inline static decltype(auto) yellow{u8"\033[93m"};
            constexpr inline static decltype(auto) lt_blue{u8"\033[94m"};
            constexpr inline static decltype(auto) lt_purple{u8"\033[95m"};
            constexpr inline static decltype(auto) lt_cyan{u8"\033[96m"};
            constexpr inline static decltype(auto) white{u8"\033[97m"};
#endif  // __MSDOS__

            // Background color variables:
            constexpr inline static decltype(auto) bg_black{u8"\033[40m"};
            constexpr inline static decltype(auto) bg_red{u8"\033[41m"};
            constexpr inline static decltype(auto) bg_green{u8"\033[42m"};
            constexpr inline static decltype(auto) bg_orange{u8"\033[43m"};
            constexpr inline static decltype(auto) bg_cyan{u8"\033[44m"};
            constexpr inline static decltype(auto) bg_purple{u8"\033[45m"};
            constexpr inline static decltype(auto) bg_blue{u8"\033[46m"};
            constexpr inline static decltype(auto) bg_gray{u8"\033[47m"};

#ifdef __MSDOS__
            constexpr inline static decltype(auto) bg_dk_gray{bg_black};
            constexpr inline static decltype(auto) bg_lt_red{bg_red};
            constexpr inline static decltype(auto) bg_lt_green{bg_green};
            constexpr inline static decltype(auto) bg_yellow{bg_orange};
            constexpr inline static decltype(auto) bg_lt_blue{bg_cyan};
            constexpr inline static decltype(auto) bg_lt_purple{bg_purple};
            constexpr inline static decltype(auto) bg_lt_cyan{bg_blue};
            constexpr inline static decltype(auto) bg_white{bg_gray};
#else
            constexpr inline static decltype(auto) bg_dk_gray{u8"\033[100m"};
            constexpr inline static decltype(auto) bg_lt_red{u8"\033[101m"};
            constexpr inline static decltype(auto) bg_lt_green{u8"\033[102m"};
            constexpr inline static decltype(auto) bg_yellow{u8"\033[103m"};
            constexpr inline static decltype(auto) bg_lt_blue{u8"\033[104m"};
            constexpr inline static decltype(auto) bg_lt_purple{u8"\033[105m"};
            constexpr inline static decltype(auto) bg_lt_cyan{u8"\033[106m"};
            constexpr inline static decltype(auto) bg_white{u8"\033[107m"};
#endif  // __MSDOS__

            // Bold color variables:
            constexpr inline static decltype(auto) bd_black{u8"\033[1;30m"};
            constexpr inline static decltype(auto) bd_red{u8"\033[1;31m"};
            constexpr inline static decltype(auto) bd_green{u8"\033[1;32m"};
            constexpr inline static decltype(auto) bd_orange{u8"\033[1;33m"};
            constexpr inline static decltype(auto) bd_blue{u8"\033[1;34m"};
            constexpr inline static decltype(auto) bd_purple{u8"\033[1;35m"};
            constexpr inline static decltype(auto) bd_cyan{u8"\033[1;36m"};
            constexpr inline static decltype(auto) bd_gray{u8"\033[1;37m"};
        };

        /**
         * @brief It is used to store the styles.
         */
        struct sty
        {
            // Style variables:
            constexpr inline static decltype(auto) bold{u8"\033[1m"};
            constexpr inline static decltype(auto) faint{u8"\033[2m"};
            constexpr inline static decltype(auto) italics{u8"\033[3m"};
            constexpr inline static decltype(auto) underlined{u8"\033[4m"};
            constexpr inline static decltype(auto) blink{u8"\033[5m"};
            constexpr inline static decltype(auto) inverse{u8"\033[7m"};
            constexpr inline static decltype(auto) invisible{u8"\033[8m"};
            constexpr inline static decltype(auto) crossed{u8"\033[9m"};
            constexpr inline static decltype(auto) d_underlined{u8"\033[21m"};
        };

        /**
         * @brief It is used to store the reset features commands.
         */
        struct rst
        {
            // Reset total variables:
            constexpr inline static decltype(auto) all{u8"\033[0m"};

#ifdef __MSDOS__
            // Reset color variables:
            constexpr inline static decltype(auto) color{all};
            constexpr inline static decltype(auto) bg_color{all};
            constexpr inline static decltype(auto) bd_color{all};

            // Reset style variables:
            constexpr inline static decltype(auto) bd_ft{all};
            constexpr inline static decltype(auto) italics{all};
            constexpr inline static decltype(auto) underlined{all};
            constexpr inline static decltype(auto) blink{all};
            constexpr inline static decltype(auto) inverse{all};
            constexpr inline static decltype(auto) invisible{all};
            constexpr inline static decltype(auto) crossed{all};
#else
            // Reset color variables:
            constexpr inline static decltype(auto) color{u8"\033[39m"};
            constexpr inline static decltype(auto) bg_color{u8"\033[49m"};
            constexpr inline static decltype(auto) bd_color{u8"\033[22m \033[39m"};

            // Reset style variables:
            constexpr inline static decltype(auto) bd_ft{u8"\033[22m"};
            constexpr inline static decltype(auto) italics{u8"\033[23m"};
            constexpr inline static decltype(auto) underlined{u8"\033[24m"};
            constexpr inline static decltype(auto) blink{u8"\033[25m"};
            constexpr inline static decltype(auto) inverse{u8"\033[27m"};
            constexpr inline static decltype(auto) invisible{u8"\033[28m"};
            constexpr inline static decltype(auto) crossed{u8"\033[29m"};
#endif
        };

        struct RGB
        {
            ::std::uint_least32_t r{};
            ::std::uint_least32_t g{};
            ::std::uint_least32_t b{};
        };

        template <::std::integral char_type>
            requires (sizeof(char_type) == sizeof(char8_t))
        constexpr inline ::std::size_t print_reserve_size(::fast_io::io_reserve_type_t<char_type, RGB>) noexcept
        {
            constexpr ::std::size_t real_size{print_reserve_size(::fast_io::io_reserve_type<char_type, ::std::uint_least32_t>)};
            constexpr ::std::size_t size{10 + 3 * real_size};
            return size;
        }

        namespace details
        {
            template <::std::integral char_type>
                requires (sizeof(char_type) == sizeof(char8_t))
            constexpr inline char_type*
                rgb_print_reserve_impl(char_type* iter, ::std::uint_least32_t r, ::std::uint_least32_t g, ::std::uint_least32_t b) noexcept
            {
                ::fast_io::freestanding::my_memcpy(iter, u8"\x1b[38;2;", 8u);
                char_type* curr_pos{iter + 7u};
                curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, ::std::uint_least32_t>, curr_pos, r);
                *(curr_pos++) = static_cast<char_type>(u8';');
                curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, ::std::uint_least32_t>, curr_pos, g);
                *(curr_pos++) = static_cast<char_type>(u8';');
                curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, ::std::uint_least32_t>, curr_pos, b);
                *(curr_pos++) = static_cast<char_type>(u8'm');
                return curr_pos;
            }
        }  // namespace details

        template <::std::integral char_type>
            requires (sizeof(char_type) == sizeof(char8_t))
        constexpr inline char_type* print_reserve_define(::fast_io::io_reserve_type_t<char_type, RGB>, char_type* iter, RGB rgb) noexcept
        {
            return details::rgb_print_reserve_impl(iter, rgb.r, rgb.g, rgb.b);
        }

    }  // namespace ansi_escape_sequences
}  // namespace uwvm
