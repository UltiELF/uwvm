#include "mode.h"
#include "../../run/wasm_file.h"
#include <io_device.h>

#if __has_cpp_attribute(__gnu__::__cold__)
[[__gnu__::__cold__]]
#endif
::uwvm::cmdline::parameter_return_type(::uwvm::parameter::details::mode_callback)(::uwvm::cmdline::parameter_parsing_results* sres,
                                                                               ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>& pres) noexcept
{
    auto sresp1{sres + 1};
    if(sresp1 == pres.cend() || sresp1->type != ::uwvm::cmdline::parameter_parsing_results_type::arg) [[unlikely]]
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
                            u8"[--mode|-m] "
                            u8"\033[32m"
                            u8"[objdump, int(default)]"
                            u8"\033[0m"
                            u8"\n\n");
        return ::uwvm::cmdline::parameter_return_type::return_m1_imme;
    }

    sresp1->type = ::uwvm::cmdline::parameter_parsing_results_type::occupied_arg;
    if(auto s1s{sresp1->str}; s1s == "objdump") { ::uwvm::running_mode = ::uwvm::mode::objdump; }
    if(auto s1s{sresp1->str}; s1s == "int") { ::uwvm::running_mode = ::uwvm::mode::interpreter; }

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
                            u8"Unsupported Mode (\""
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
                            u8"[--mode|-m] "
                            u8"\033[32m"
                            u8"[objdump, int(default)]"
                            u8"\033[0m"
                            u8"\n\n");
        return ::uwvm::cmdline::parameter_return_type::return_m1_imme;
    }
    return ::uwvm::cmdline::parameter_return_type::def;
}
