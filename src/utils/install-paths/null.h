#pragma once
#include <fast_io.h>
#include <fast_io_device.h>

namespace uwvm::path
{
#if __has_cpp_attribute(__gnu__::__cold__)
    [[__gnu__::__cold__]]
#endif
#if __has_cpp_attribute(__gnu__::__pure__)
    [[__gnu__::__pure__]]
#endif
    inline constexpr void
        get_module_install_path() noexcept
    {
    }
}  // namespace uwvm::path
