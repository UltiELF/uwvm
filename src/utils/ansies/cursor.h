#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <fast_io.h>

namespace uwvm
{

    namespace ansi_escape_sequences
    {
        /**
         * @brief It is used to store the terminal control sequences.
         */
        struct tsc
        {
            // Control characters variables:
            inline static constexpr decltype(auto) bell{u8"\x07"};  // Bell sound
            inline static constexpr decltype(auto) bksp{u8"\x08"};  // Backspace
            inline static constexpr decltype(auto) tab{u8"\x09"};   // Tab
            inline static constexpr decltype(auto) lfd{u8"\x0A"};   // Line feed
            inline static constexpr decltype(auto) ffd{u8"\x0C"};   // Form feed
            inline static constexpr decltype(auto) crt{u8"\x0D"};   // Carriage return

            // Control sequences variables:
            inline static constexpr decltype(auto) hcrs{u8"\u001b[?25l"};  // Hide cursor
            inline static constexpr decltype(auto) scrs{u8"\u001b[?25h"};  // Show cursor
        };

        /**
         * @brief It is used to store the cursor commands.
         */
        struct crs
        {
            ::std::uint_least32_t feat{};
            enum class cursor_variables : char8_t
            {
                up = u8'A',
                down = u8'B',
                right = u8'C',
                left = u8'D'
            } cursor_var{};
        };

        template <::std::integral char_type>
            requires (sizeof(char_type) == sizeof(char8_t))
        inline constexpr ::std::size_t print_reserve_size(::fast_io::io_reserve_type_t<char_type, crs>) noexcept
        {
            constexpr ::std::size_t real_size{print_reserve_size(::fast_io::io_reserve_type<char_type, ::std::uint_least32_t>)};
            constexpr ::std::size_t size{3 + real_size};
            return size;
        }

        namespace details
        {
            template <::std::integral char_type>
                requires (sizeof(char_type) == sizeof(char8_t))
            inline constexpr char_type* crs_print_reserve_impl(char_type* iter, crs::cursor_variables cv, ::std::uint_least32_t f) noexcept
            {
                ::fast_io::freestanding::my_memcpy(iter, u8"\u001b[", 2u);
                char_type* curr_pos{iter + 2u};
                curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, ::std::uint_least32_t>, curr_pos, f);
                *(curr_pos++) = static_cast<char_type>(cv);
                return curr_pos;
            }
        }  // namespace details

        template <::std::integral char_type>
            requires (sizeof(char_type) == sizeof(char8_t))
        inline constexpr char_type* print_reserve_define(::fast_io::io_reserve_type_t<char_type, crs>, char_type* iter, crs c) noexcept
        {
            return details::crs_print_reserve_impl(iter, c.cursor_var, c.feat);
        }

        /**
         * @brief  It is used to store the terminal control sequences for clear line / screen.
         */

        struct tcsc
        {
            ::std::uint_least32_t feat{};
            enum class cursor_sequences_variables : char8_t
            {
                csc = u8'J',
                cln = u8'K',
            } cursor_seq_var{};
        };

        template <::std::integral char_type>
            requires (sizeof(char_type) == sizeof(char8_t))
        inline constexpr ::std::size_t print_reserve_size(::fast_io::io_reserve_type_t<char_type, tcsc>) noexcept
        {
            constexpr ::std::size_t real_size{print_reserve_size(::fast_io::io_reserve_type<char_type, ::std::uint_least32_t>)};
            constexpr ::std::size_t size{3 + real_size};
            return size;
        }

        namespace details
        {
            template <::std::integral char_type>
                requires (sizeof(char_type) == sizeof(char8_t))
            inline constexpr char_type* tcsc_print_reserve_impl(char_type* iter, tcsc::cursor_sequences_variables csv, ::std::uint_least32_t f) noexcept
            {
                ::fast_io::freestanding::my_memcpy(iter, u8"\u001b[", 2u);
                char_type* curr_pos{iter + 2u};
                curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, ::std::uint_least32_t>, curr_pos, f);
                *(curr_pos++) = static_cast<char_type>(csv);
                return curr_pos;
            }
        }  // namespace details

        template <::std::integral char_type>
            requires (sizeof(char_type) == sizeof(char8_t))
        inline constexpr char_type* print_reserve_define(::fast_io::io_reserve_type_t<char_type, tcsc>, char_type* iter, tcsc tc) noexcept
        {
            return details::crs_print_reserve_impl(iter, tc.cursor_seq_var, tc.feat);
        }

    }  // namespace ansi_escape_sequences
}  // namespace uwvm
