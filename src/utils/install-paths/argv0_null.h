#pragma once

namespace uwvm::path
{
#if __has_cpp_attribute(__gnu__::__cold__)
    [[__gnu__::__cold__]]
#endif
    inline void
        get_module_install_path_from_argv0(char const* argv0) noexcept
    {
    }
}  // namespace uwvm::path
