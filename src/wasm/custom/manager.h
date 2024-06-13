#pragma once

namespace uvwm::wasm::custom
{
    using handlefunc_may_alias_ptr =
#if __has_cpp_attribute(__gnu__::__may_alias__)
        [[__gnu__::__may_alias__]]
#endif
        bool (*)(::uwvm::wasm::section::custom_section);

    struct csfunc_return_struct
    {
        handlefunc_may_alias_ptr func{};
        char8_t const* name_begin{};
        char8_t const* name_end{};
    };

    using csfunc_may_alias_ptr =
#if __has_cpp_attribute(__gnu__::__may_alias__)
        [[__gnu__::__may_alias__]]
#endif
        csfunc_return_struct const* (*)();

}
