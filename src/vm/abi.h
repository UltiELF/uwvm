#pragma once
#include <cstdint>
#include <cstddef>
#include <fast_io.h>
#include <fast_io_dsal/string_view.h>
#include <unfinished.h>
#include "../run/wasm_file.h"
#include "interpreter/import.h"
// wasi
#include "interpreter/wasi/int_cl.h"
#if UWVM_ENABLE_UNCHECKED_INTERPRETER &&                                                                                                                       \
    ((defined(_WIN32) || defined(__CYGWIN__)) ||                                                                                                               \
     (!defined(__NEWLIB__) && !defined(__MSDOS__) && (!defined(__wasm__) || (defined(__wasi__) && defined(_WASI_EMULATED_MMAN))) && __has_include(<sys/mman.h>)))
#include "unchecked_interpreter/import.h"
// wasi
#include "unchecked_interpreter/wasi/int_cl.h"
#endif
    #include "wasm.h"


#include "wasi/fd_map.h"

namespace uwvm::vm
{
#if __has_cpp_attribute(__gnu__::__cold__)
    [[__gnu__::__cold__]]
#endif
    inline void
        abi_detect() noexcept
    {
        if(::uwvm::wasm_abi == ::uwvm::wasm::abi::detect) [[likely]]
        {
            ::uwvm::wasm_abi = ::uwvm::wasm::abi::bare;

            for(auto const i: ::uwvm::global_wasm_module.importsec.func_types)
            {
                ::fast_io::u8string_view const module_name{i->module_begin, static_cast<::std::size_t>(i->module_end - i->module_begin)};
                if(module_name.contains(u8"wasi"))
                {
                    ::uwvm::wasm_abi = uwvm::wasm::abi::wasi;
                    break;
                }
            }
        }

        if(::uwvm::running_mode == ::uwvm::mode::interpreter)
        {
            // init import
            auto const& wasmmod{::uwvm::global_wasm_module};
            auto const import_size{wasmmod.importsec.func_types.size()};
            ::uwvm::vm::interpreter::imports = ::fast_io::vector<::uwvm::vm::interpreter::int_import_func_p>{import_size};

            // bare
            if(::uwvm::wasm_abi == ::uwvm::wasm::abi::bare) [[unlikely]] { return; }
            else if(::uwvm::wasm_abi == ::uwvm::wasm::abi::wasi)
            {
                init(::uwvm::vm::wasi::wasm_fd_storages);
                ::uwvm::vm::wasi::wasm_fd_storages.opens.push_back_unchecked(0);
                ::uwvm::vm::wasi::wasm_fd_storages.opens.push_back_unchecked(1);
                ::uwvm::vm::wasi::wasm_fd_storages.opens.push_back_unchecked(2);
            }

            // init import
            auto ft_p{wasmmod.importsec.func_types.cbegin()};
            for(auto& i: ::uwvm::vm::interpreter::imports)
            {
                ::fast_io::u8string_view const module_name{(*ft_p)->module_begin, static_cast<::std::size_t>((*ft_p)->module_end - (*ft_p)->module_begin)};
                ::fast_io::u8string_view const sub_name{(*ft_p)->name_begin, static_cast<::std::size_t>((*ft_p)->name_end - (*ft_p)->name_begin)};

                if(module_name.contains(u8"wasi"))
                {
                    auto const f{::uwvm::vm::interpreter::find_from_hash_table(::uwvm::vm::interpreter::wasi::wasi_hash_table, sub_name)};

                    if(f != nullptr) [[likely]] { i = f; }
                    else
                    {
                        ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"Invalid wasi function."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
                }
                else { ::uwvm::unfinished(); }

                ++ft_p;
            }
        }
#if UWVM_ENABLE_UNCHECKED_INTERPRETER &&                                                                                                                       \
    ((defined(_WIN32) || defined(__CYGWIN__)) ||                                                                                                               \
     (!defined(__NEWLIB__) && !defined(__MSDOS__) && (!defined(__wasm__) || (defined(__wasi__) && defined(_WASI_EMULATED_MMAN))) && __has_include(<sys/mman.h>)))
        else if(::uwvm::running_mode == ::uwvm::mode::unchecked_interpreter)
        {  // init import
            auto const& wasmmod{::uwvm::global_wasm_module};
            auto const import_size{wasmmod.importsec.func_types.size()};
            ::uwvm::vm::unchecked_interpreter::imports = ::fast_io::vector<::uwvm::vm::unchecked_interpreter::int_import_func_p>{import_size};

            // bare
            if(::uwvm::wasm_abi == ::uwvm::wasm::abi::bare) [[unlikely]] { return; }
            else if(::uwvm::wasm_abi == ::uwvm::wasm::abi::wasi)
            {
                init(::uwvm::vm::wasi::wasm_fd_storages);
                ::uwvm::vm::wasi::wasm_fd_storages.opens.push_back_unchecked(0);
                ::uwvm::vm::wasi::wasm_fd_storages.opens.push_back_unchecked(1);
                ::uwvm::vm::wasi::wasm_fd_storages.opens.push_back_unchecked(2);
            }

            // init import
            auto ft_p{wasmmod.importsec.func_types.cbegin()};
            for(auto& i: ::uwvm::vm::unchecked_interpreter::imports)
            {
                ::fast_io::u8string_view const module_name{(*ft_p)->module_begin, static_cast<::std::size_t>((*ft_p)->module_end - (*ft_p)->module_begin)};
                ::fast_io::u8string_view const sub_name{(*ft_p)->name_begin, static_cast<::std::size_t>((*ft_p)->name_end - (*ft_p)->name_begin)};

                if(module_name.contains(u8"wasi"))
                {
                    auto const f{::uwvm::vm::unchecked_interpreter::find_from_hash_table(::uwvm::vm::unchecked_interpreter::wasi::wasi_hash_table, sub_name)};

                    if(f != nullptr) [[likely]] { i = f; }
                    else
                    {
                        ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
    #ifdef __MSDOS__
                                u8"\033[37m"
    #else
                                u8"\033[97m"
    #endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
    #ifdef __MSDOS__
                                u8"\033[37m"
    #else
                                u8"\033[97m"
    #endif
                                u8"Invalid wasi function."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
                }
                else { ::uwvm::unfinished(); }

                ++ft_p;
            }
        }
#endif
    }
}  // namespace uwvm::vm
