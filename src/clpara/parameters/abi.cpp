#include "abi.h"
#include "../../run/abi.h"
#include <io_device.h>

::uwvm::cmdline::parameter_return_type(::uwvm::parameter::details::abi_callback)(::std::size_t sres,
                                                                                 ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>& pres) noexcept
{
    if(sres == pres.size() - 1 || pres[sres + 1].type != ::uwvm::cmdline::parameter_parsing_results_type::arg) [[unlikely]]
    {
        ::fast_io::io::perrln(::uwvm::u8err,
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
                              "[bare|emscripten|wasi]"
                              u8"\033[0m");
        return ::uwvm::cmdline::parameter_return_type::return_imme;
    }
    auto& pres_sresp1{pres[sres + 1]};
    pres_sresp1.type = ::uwvm::cmdline::parameter_parsing_results_type::occupied_arg;
    if(auto s1s{pres_sresp1.str}; s1s == ::fast_io::os_c_str_arr("bare")) { ::uwvm::wasm_abi = ::uwvm::abi::bare; }
    else if(s1s == ::fast_io::os_c_str_arr("emscripten")) { ::uwvm::wasm_abi = ::uwvm::abi::emscripten; }
    else if(s1s == ::fast_io::os_c_str_arr("wasi")) { ::uwvm::wasm_abi = ::uwvm::abi::wasi; }
    else
    {
        ::fast_io::io::perrln(::uwvm::u8err,
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
                              "[bare|emscripten|wasi]"
                              u8"\033[0m");
        return ::uwvm::cmdline::parameter_return_type::return_imme;
    }
    return ::uwvm::cmdline::parameter_return_type::def;
}
