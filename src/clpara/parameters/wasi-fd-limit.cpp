#include "wasi-fd-limit.h"
#include "../../vm/wasi/fd_limit.h"

::uwvm::cmdline::parameter_return_type(::uwvm::parameter::details::wasi_fd_limit_callback)(
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
                            u8"[--wasi-fd-limit|-Wfl] "
                            u8"\033[32m"
                            u8"<size_t>"
                            u8"\033[0m"
                            u8"\n\n");
        return ::uwvm::cmdline::parameter_return_type::return_m1_imme;
    }

    sresp1->type = ::uwvm::cmdline::parameter_parsing_results_type::occupied_arg;

    auto const str{sresp1->str};

    ::std::size_t limit_num{};

    auto const [next, err]{::fast_io::parse_by_scan(str.cbegin(), str.cend(), limit_num)};

    if(next != str.cend() || err != ::fast_io::parse_code::ok) [[unlikely]]
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
                            u8"[--wasi-fd-limit|-Wfl] "
                            u8"\033[32m"
                            u8"<size_t>"
                            u8"\033[0m"
                            u8"\n\n");
        return ::uwvm::cmdline::parameter_return_type::return_m1_imme;
    }

    if(limit_num < 3) [[unlikely]]
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
                                u8"fd_limit cannot be less than 3."
                                u8"\n\n"
                                u8"\033[0m");
        return ::uwvm::cmdline::parameter_return_type::return_m1_imme;
    }

    ::uwvm::vm::wasi::wasi_fd_limit = limit_num;

    return ::uwvm::cmdline::parameter_return_type::def;
}
