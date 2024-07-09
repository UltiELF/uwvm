#pragma once
#include <cstdint>
#include <cstddef>
#include <fast_io.h>
#include <fast_io_dsal/string_view.h>
#include <unfinished.h>
#include "../run/wasm_file.h"
#include "interpreter/import.h"
#include "wasm.h"

// wasi
#include "interpreter/wasi/int_cl.h"

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

        // init import
        auto const& wasmmod{::uwvm::global_wasm_module};
        auto const import_size{wasmmod.importsec.func_types.size()};

        ::uwvm::vm::interpreter::imports = ::fast_io::vector<::uwvm::vm::interpreter::int_import_func_p>{import_size};

        // bare
        if(::uwvm::wasm_abi == ::uwvm::wasm::abi::bare) [[unlikely]]
        {
            if(::uwvm::vm::start_func == static_cast<::std::size_t>(-1)) [[unlikely]]
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
                                u8"Bare running must specify start func."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }
            return;
        }
        
        // init import
        auto ft_p{wasmmod.importsec.func_types.cbegin()};
        auto const wasi_map_cend{::uwvm::vm::interpreter::wasi::wasi_map.cend()};
        for(auto& i: ::uwvm::vm::interpreter::imports)
        {
            ::fast_io::u8string_view const module_name{(*ft_p)->module_begin, static_cast<::std::size_t>((*ft_p)->module_end - (*ft_p)->module_begin)};
            ::fast_io::u8string_view const sub_name{(*ft_p)->name_begin, static_cast<::std::size_t>((*ft_p)->name_end - (*ft_p)->name_begin)};

            if(module_name.contains(u8"wasi"))
            {
                auto const f{::uwvm::vm::interpreter::wasi::wasi_map.find(sub_name)};
                if(f != wasi_map_cend) [[likely]] { i = f->second; }
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
            else 
            { 
                ::uwvm::unfinished();
            }

            ++ft_p;
        }
    }
}  // namespace uwvm::vm
