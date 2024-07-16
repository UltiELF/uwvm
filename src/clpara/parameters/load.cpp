#ifdef UWVM_TIMER
    #include <fast_io_driver/timer.h>
#endif
#include "load.h"
#include "../parameters.h"
#include "../../run/plugin.h"
#include "../../wasm/custom.h"

#if __has_cpp_attribute(__gnu__::__cold__)
[[__gnu__::__cold__]]
#endif
::uwvm::cmdline::parameter_return_type(::uwvm::parameter::details::load_callback)(::uwvm::cmdline::parameter_parsing_results* sres,
                                                                                  ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>& pres) noexcept
{
#if(defined(_WIN32) || defined(__CYGWIN__)) ||                                                                                                                 \
    ((!defined(_WIN32) || defined(__WINE__)) && (defined(__CYGWIN__) || (!defined(__NEWLIB__) && !defined(__wasi__))))

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
                            u8"[--load|-l] "
                            u8"\033[32m"
                            u8"<file>"
                            u8"\033[0m"
                            u8"\n\n");
        return ::uwvm::cmdline::parameter_return_type::return_m1_imme;
    }

    sresp1->type = ::uwvm::cmdline::parameter_parsing_results_type::occupied_arg;

    #ifdef __cpp_exceptions
    try
    #endif
    {
    #ifdef UWVM_TIMER
        ::fast_io::timer plugin_loader_timer{u8"uwvm: [timer] plugin loader"};
    #endif
        auto const& ndl{::uwvm::plugins.emplace_back(sresp1->str, fast_io::dll_mode::posix_rtld_lazy)};
        ::uwvm::wasm::custom::import_custom_section_from_dll(ndl);
    }
    #ifdef __cpp_exceptions
    catch(::fast_io::error e)
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
                                u8"Unable to load dynamic library: ",
                                e,
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
        ::fast_io::fast_terminate();
    }
    #endif

    return ::uwvm::cmdline::parameter_return_type::def;
#else

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
                            u8"Using a platform that does not support dll loader."
                            u8"\033[0m"
                            u8"\n\n");
    return ::uwvm::cmdline::parameter_return_type::return_m1_imme;
#endif
}
