#include "wasi-mount-root.h"
#include "../../vm/wasi/root.h"

::uwvm::cmdline::parameter_return_type(::uwvm::parameter::details::wasi_mount_root_callback)(
    ::uwvm::cmdline::parameter_parsing_results* sres,
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
                            u8"[--wasi-mount-root|-Wmr] "
                            u8"\033[32m"
                            u8"<path>"
                            u8"\033[0m"
                            u8"\n\n");
        return ::uwvm::cmdline::parameter_return_type::return_m1_imme;
    }

    sresp1->type = ::uwvm::cmdline::parameter_parsing_results_type::occupied_arg;

    auto const str{sresp1->str};

#ifdef __cpp_exceptions
    try
#endif
    {
        ::uwvm::vm::wasi::root_path = ::fast_io::dir_file{str};
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
                                u8"Unable to open path: ",
                                e,
                                u8"\n\n"
                                u8"\033[0m");
        return ::uwvm::cmdline::parameter_return_type::return_m1_imme;
    }
#endif

    return ::uwvm::cmdline::parameter_return_type::def;
}
