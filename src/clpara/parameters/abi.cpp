﻿#include "abi.h"
#include "../../wasm/abi.h"
#include <io_device.h>

#if __has_cpp_attribute(__gnu__::__cold__)
[[__gnu__::__cold__]]
#endif
::uwvm::cmdline::parameter_return_type(::uwvm::parameter::details::abi_callback)(::uwvm::cmdline::parameter_parsing_results* sres,
                                                                                 ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>& pres) noexcept
{
    auto sresp1{sres + 1};
    if(sresp1 == pres.cend()  || (sres + 1)->type != ::uwvm::cmdline::parameter_parsing_results_type::arg) [[unlikely]]
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
                            u8"[error] "
#ifdef __MSDOS__
                            u8"\033[37m"
#else
                            u8"\033[97m"
#endif
                            u8"Usage: "
                            u8"\033[36m"
                            u8"[--abi|-a] "
                            u8"\033[32m"
                            "[bare|emscripten|wasip1|wasip2]"
                            u8"\033[0m"
                            u8"\n\n");
        return ::uwvm::cmdline::parameter_return_type::return_m1_imme;
    }
 
    auto& pres_sresp1{*sresp1};
    pres_sresp1.type = ::uwvm::cmdline::parameter_parsing_results_type::occupied_arg;
    if(auto s1s{pres_sresp1.str}; s1s == ::fast_io::string_view{"bare"}) { ::uwvm::wasm::wasm_abi = ::uwvm::wasm::abi::bare; }
    else if(s1s == ::fast_io::string_view{"emscripten"}) { ::uwvm::wasm::wasm_abi = ::uwvm::wasm::abi::emscripten; }
    else if(s1s == ::fast_io::string_view{"wasip1"}) { ::uwvm::wasm::wasm_abi = ::uwvm::wasm::abi::wasip1; }
    else if(s1s == ::fast_io::string_view{"wasip2"}) { ::uwvm::wasm::wasm_abi = ::uwvm::wasm::abi::wasip2; }
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
                            u8"[error] "
#ifdef __MSDOS__
                            u8"\033[37m"
#else
                            u8"\033[97m"
#endif
                            u8"Unsupported ABI (\""
#ifdef __MSDOS__
                            u8"\033[35m"
#else
                            u8"\033[95m"
#endif
                            ,
                            ::fast_io::mnp::code_cvt(s1s),
#ifdef __MSDOS__
                            u8"\033[37m"
#else
                            u8"\033[97m"
#endif
                            u8"\"). "
                            u8"Usage: "
                            u8"\033[36m"
                            u8"[--abi|-a] "
                            u8"\033[32m"
                            "[bare|emscripten|wasip1|wasip2]"
                            u8"\033[0m"
                            u8"\n\n");
        return ::uwvm::cmdline::parameter_return_type::return_m1_imme;
    }
    return ::uwvm::cmdline::parameter_return_type::def;
}
