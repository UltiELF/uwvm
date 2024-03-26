#pragma once
#include <concepts>

namespace uwvm
{
    template <typename type>
    concept char_type = ::std::same_as<type, char> || ::std::same_as<type, wchar_t> || ::std::same_as<type, char8_t> || ::std::same_as<type, char16_t> ||
                        ::std::same_as<type, char32_t>;
    template <typename type>
    concept fix_length_char_type = ::std::same_as<type, char8_t> || ::std::same_as<type, char16_t> || ::std::same_as<type, char32_t>;
}  // namespace uwvm
