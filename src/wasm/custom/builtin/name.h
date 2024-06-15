#pragma once
#include "../../section/custom.h"
#include "../../module.h"
#include "../../../run/wasm_file.h"
#include "../basic.h"

namespace uwvm::wasm::custom
{

    inline bool scan_name_custom_section(::uwvm::wasm::section::custom_section cs) noexcept
    {
#ifdef UWVM_TIMER
        ::fast_io::timer scan_custom_name_section_timer{u8"uwvm: [timer] scan custom section: name"};
#endif
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
        using value_type_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::uwvm::wasm::value_type*;
        using value_type_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::uwvm::wasm::value_type const*;

        auto& wasmmod{::uwvm::global_wasm_module};

        // curr
        auto curr{reinterpret_cast<::std::byte const*>(cs.name_end)};
        auto const end{cs.sec_end};

        for(; curr < end;)
        {
            // name_type
            ::std::uint_fast8_t name_type{};
            auto const [next_name_type, err_name_type]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                                reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                                ::fast_io::mnp::leb128_get(name_type))};
            switch(err_name_type)
            {
                case ::fast_io::parse_code::ok: break;
                default: [[unlikely]] return false;
            }

            if(name_type > 127) [[unlikely]] { return false; }

            curr = reinterpret_cast<::std::byte const*>(next_name_type);

            // name_payload_len

            ::std::size_t name_payload_len{};  // data_length
            auto const [next_name_payload_len, err_name_payload_len]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                                              reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                                              ::fast_io::mnp::leb128_get(name_payload_len))};
            switch(err_name_payload_len)
            {
                case ::fast_io::parse_code::ok: break;
                default: [[unlikely]] return false;
            }

            if(!::uwvm::wasm::custom::check_custom_index(name_payload_len)) [[unlikely]] { return false; }

            curr = reinterpret_cast<::std::byte const*>(next_name_payload_len);  // data_begin

            auto const data_end{curr + name_payload_len};

            switch(name_type)
            {
                case 0:  // module
                {
                    ::std::size_t name_len{};  // name_len
                    auto const [next_name_len, err_name_len]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                                      reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                                      ::fast_io::mnp::leb128_get(name_len))};
                    switch(err_name_len)
                    {
                        case ::fast_io::parse_code::ok: break;
                        default: [[unlikely]] return false;
                    }

                    if(!::uwvm::wasm::custom::check_custom_index(name_len)) [[unlikely]] { return false; }

                    curr = reinterpret_cast<::std::byte const*>(next_name_len);  // name_begin
                    if(data_end - curr != name_len) [[unlikely]] { return false; }

                    wasmmod.module_name = ::fast_io::u8string_view{reinterpret_cast<char8_t_const_may_alias_ptr>(curr), name_len};

                    curr = data_end;  // next

                    break;
                }
                case 1:  // function
                {
                    // count
                    ::std::size_t count{};  // number of naming in names
                    auto const [next_count, err_count]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                                reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                                ::fast_io::mnp::leb128_get(count))};
                    switch(err_count)
                    {
                        case ::fast_io::parse_code::ok: break;
                        default: [[unlikely]] return false;
                    }

                    auto const import_func_count{wasmmod.importsec.func_types.size()};
                    auto const func_count{import_func_count + wasmmod.functionsec.function_count};

                    if(count > func_count) [[unlikely]] { return false; }

                    curr = reinterpret_cast<::std::byte const*>(next_count);  // name_begin

                    for(::std::size_t i{}; i < count; ++i)
                    {
                        // index
                        ::std::size_t index{};  // number of naming in names
                        auto const [next_index, err_index]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                                    reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                                    ::fast_io::mnp::leb128_get(index))};
                        switch(err_index)
                        {
                            case ::fast_io::parse_code::ok: break;
                            default: [[unlikely]] return false;
                        }

                        if(index >= func_count) [[unlikely]] { return false; }

                        curr = reinterpret_cast<::std::byte const*>(next_index);

                        // name len
                        ::std::size_t name_len{};  // length of name_str in bytes
                        auto const [next_len, err_len]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                                reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                                ::fast_io::mnp::leb128_get(name_len))};
                        switch(err_len)
                        {
                            case ::fast_io::parse_code::ok: break;
                            default: [[unlikely]] return false;
                        }

                        if(!::uwvm::wasm::custom::check_custom_index(name_len)) [[unlikely]] { return false; }

                        curr = reinterpret_cast<::std::byte const*>(next_len);  // name

                        if(data_end - curr < name_len) [[unlikely]] { return false; }

                        if(index >= import_func_count)
                        {
                            auto const func_index{index - import_func_count};
                            auto& type{wasmmod.functionsec.types.index_unchecked(func_index)};
                            type.custom_name_begin = reinterpret_cast<char8_t_const_may_alias_ptr>(curr);
                            type.custom_name_end = reinterpret_cast<char8_t_const_may_alias_ptr>(curr) + name_len;
                        }
                        else
                        {
                            auto const type{const_cast<::uwvm::wasm::section::import_type*>(wasmmod.importsec.func_types.index_unchecked(index))};
                            type->custom_name_begin = reinterpret_cast<char8_t_const_may_alias_ptr>(curr);
                            type->custom_name_end = reinterpret_cast<char8_t_const_may_alias_ptr>(curr) + name_len;
                        }

                        curr += name_len;
                    }

                    break;
                }
                case 2:
                {
                    return false;
                    break;
                }

                default: [[unlikely]] return false;
            }
        }

        return true;
    }

}  // namespace uwvm::wasm::custom
