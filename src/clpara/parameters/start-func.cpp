#include "start-func.h"
#include "../../vm/wasm.h"

::uwvm::cmdline::parameter_return_type(::uwvm::parameter::details::start_func_callback)(
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
                            u8"[--start-func|-sf] "
                            u8"\033[32m"
                            u8"<size_t>"
                            u8"\033[0m"
                            u8"\n\n");
        return ::uwvm::cmdline::parameter_return_type::return_m1_imme;
    }

    sresp1->type = ::uwvm::cmdline::parameter_parsing_results_type::occupied_arg;

    ::std::size_t func{};

    auto const [next, err]{::fast_io::parse_by_scan(sresp1->str.cbegin(), sresp1->str.cend(), func)};

    if(next != sresp1->str.cend() || err != ::fast_io::parse_code::ok) [[unlikely]]
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
                            u8"[--start-func|-sf] "
                            u8"\033[32m"
                            u8"<size_t>"
                            u8"\033[0m"
                            u8"\n\n");
        return ::uwvm::cmdline::parameter_return_type::return_m1_imme;
    }

    ::uwvm::vm::start_func = func;

    return ::uwvm::cmdline::parameter_return_type::def;
}
