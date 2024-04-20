#include "enable-relaxed-simd.h"
#include "../../run/features.h"

::uwvm::cmdline::parameter_return_type(uwvm::parameter::details::enable_relaxed_simd_callback)(
    ::uwvm::cmdline::parameter_parsing_results*,
    ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>&) noexcept
{
    ::uwvm::features::enable_fixed_width_simd = true;
    return ::uwvm::cmdline::parameter_return_type();
}