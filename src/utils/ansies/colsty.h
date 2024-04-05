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
            inline static constexpr decltype(auto) black{u8"\033[30m"};
            inline static constexpr decltype(auto) red{u8"\033[31m"};
            inline static constexpr decltype(auto) green{u8"\033[32m"};
            inline static constexpr decltype(auto) orange{u8"\033[33m"};
            inline static constexpr decltype(auto) blue{u8"\033[34m"};
            inline static constexpr decltype(auto) purple{u8"\033[35m"};
            inline static constexpr decltype(auto) cyan{u8"\033[36m"};
            inline static constexpr decltype(auto) gray{u8"\033[37m"};

#ifdef __MSDOS__
            inline static constexpr decltype(auto) dk_gray{black};
            inline static constexpr decltype(auto) lt_red{red};
            inline static constexpr decltype(auto) lt_green{green};
            inline static constexpr decltype(auto) yellow{orange};
            inline static constexpr decltype(auto) lt_blue{blue};
            inline static constexpr decltype(auto) lt_purple{purple};
            inline static constexpr decltype(auto) lt_cyan{cyan};
            inline static constexpr decltype(auto) white{gray};
#else
            inline static constexpr decltype(auto) dk_gray{u8"\033[90m"};
            inline static constexpr decltype(auto) lt_red{u8"\033[91m"};
            inline static constexpr decltype(auto) lt_green{u8"\033[92m"};
            inline static constexpr decltype(auto) yellow{u8"\033[93m"};
            inline static constexpr decltype(auto) lt_blue{u8"\033[94m"};
            inline static constexpr decltype(auto) lt_purple{u8"\033[95m"};
            inline static constexpr decltype(auto) lt_cyan{u8"\033[96m"};
            inline static constexpr decltype(auto) white{u8"\033[97m"};
#endif  // __MSDOS__

            // Background color variables:
            inline static constexpr decltype(auto) bg_black{u8"\033[40m"};
            inline static constexpr decltype(auto) bg_red{u8"\033[41m"};
            inline static constexpr decltype(auto) bg_green{u8"\033[42m"};
            inline static constexpr decltype(auto) bg_orange{u8"\033[43m"};
            inline static constexpr decltype(auto) bg_cyan{u8"\033[44m"};
            inline static constexpr decltype(auto) bg_purple{u8"\033[45m"};
            inline static constexpr decltype(auto) bg_blue{u8"\033[46m"};
            inline static constexpr decltype(auto) bg_gray{u8"\033[47m"};

#ifdef __MSDOS__
            inline static constexpr decltype(auto) bg_dk_gray{bg_black};
            inline static constexpr decltype(auto) bg_lt_red{bg_red};
            inline static constexpr decltype(auto) bg_lt_green{bg_green};
            inline static constexpr decltype(auto) bg_yellow{bg_orange};
            inline static constexpr decltype(auto) bg_lt_blue{bg_cyan};
            inline static constexpr decltype(auto) bg_lt_purple{bg_purple};
            inline static constexpr decltype(auto) bg_lt_cyan{bg_blue};
            inline static constexpr decltype(auto) bg_white{bg_gray};
#else
            inline static constexpr decltype(auto) bg_dk_gray{u8"\033[100m"};
            inline static constexpr decltype(auto) bg_lt_red{u8"\033[101m"};
            inline static constexpr decltype(auto) bg_lt_green{u8"\033[102m"};
            inline static constexpr decltype(auto) bg_yellow{u8"\033[103m"};
            inline static constexpr decltype(auto) bg_lt_blue{u8"\033[104m"};
            inline static constexpr decltype(auto) bg_lt_purple{u8"\033[105m"};
            inline static constexpr decltype(auto) bg_lt_cyan{u8"\033[106m"};
            inline static constexpr decltype(auto) bg_white{u8"\033[107m"};
#endif  // __MSDOS__

            // Bold color variables:
            inline static constexpr decltype(auto) bd_black{u8"\033[1;30m"};
            inline static constexpr decltype(auto) bd_red{u8"\033[1;31m"};
            inline static constexpr decltype(auto) bd_green{u8"\033[1;32m"};
            inline static constexpr decltype(auto) bd_orange{u8"\033[1;33m"};
            inline static constexpr decltype(auto) bd_blue{u8"\033[1;34m"};
            inline static constexpr decltype(auto) bd_purple{u8"\033[1;35m"};
            inline static constexpr decltype(auto) bd_cyan{u8"\033[1;36m"};
            inline static constexpr decltype(auto) bd_gray{u8"\033[1;37m"};
        };

        /**
         * @brief It is used to store the styles.
         */
        struct sty
        {
            // Style variables:
            inline static constexpr decltype(auto) bold{u8"\033[1m"};
            inline static constexpr decltype(auto) faint{u8"\033[2m"};
            inline static constexpr decltype(auto) italics{u8"\033[3m"};
            inline static constexpr decltype(auto) underlined{u8"\033[4m"};
            inline static constexpr decltype(auto) blink{u8"\033[5m"};
            inline static constexpr decltype(auto) inverse{u8"\033[7m"};
            inline static constexpr decltype(auto) invisible{u8"\033[8m"};
            inline static constexpr decltype(auto) crossed{u8"\033[9m"};
            inline static constexpr decltype(auto) d_underlined{u8"\033[21m"};
        };

        /**
         * @brief It is used to store the reset features commands.
         */
        struct rst
        {
            // Reset total variables:
            inline static constexpr decltype(auto) all{u8"\033[0m"};

#ifdef __MSDOS__
            // Reset color variables:
            inline static constexpr decltype(auto) color{all};
            inline static constexpr decltype(auto) bg_color{all};
            inline static constexpr decltype(auto) bd_color{all};

            // Reset style variables:
            inline static constexpr decltype(auto) bd_ft{all};
            inline static constexpr decltype(auto) italics{all};
            inline static constexpr decltype(auto) underlined{all};
            inline static constexpr decltype(auto) blink{all};
            inline static constexpr decltype(auto) inverse{all};
            inline static constexpr decltype(auto) invisible{all};
            inline static constexpr decltype(auto) crossed{all};
#else
            // Reset color variables:
            inline static constexpr decltype(auto) color{u8"\033[39m"};
            inline static constexpr decltype(auto) bg_color{u8"\033[49m"};
            inline static constexpr decltype(auto) bd_color{u8"\033[22m \033[39m"};

            // Reset style variables:
            inline static constexpr decltype(auto) bd_ft{u8"\033[22m"};
            inline static constexpr decltype(auto) italics{u8"\033[23m"};
            inline static constexpr decltype(auto) underlined{u8"\033[24m"};
            inline static constexpr decltype(auto) blink{u8"\033[25m"};
            inline static constexpr decltype(auto) inverse{u8"\033[27m"};
            inline static constexpr decltype(auto) invisible{u8"\033[28m"};
            inline static constexpr decltype(auto) crossed{u8"\033[29m"};
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
        inline constexpr ::std::size_t print_reserve_size(::fast_io::io_reserve_type_t<char_type, RGB>) noexcept
        {
            constexpr ::std::size_t real_size{print_reserve_size(::fast_io::io_reserve_type<char_type, ::std::uint_least32_t>)};
            constexpr ::std::size_t size{10 + 3 * real_size};
            return size;
        }

        namespace details
        {
            template <::std::integral char_type>
                requires (sizeof(char_type) == sizeof(char8_t))
            inline constexpr char_type*
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
        inline constexpr char_type* print_reserve_define(::fast_io::io_reserve_type_t<char_type, RGB>, char_type* iter, RGB rgb) noexcept
        {
            return details::rgb_print_reserve_impl(iter, rgb.r, rgb.g, rgb.b);
        }

    }  // namespace ansi_escape_sequences
}  // namespace uwvm
