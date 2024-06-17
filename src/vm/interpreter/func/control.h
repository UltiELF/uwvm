#pragma once
#include <fast_io.h>
#include <io_device.h>
#include "../ast.h"
#include "../../../run/wasm_file.h"

namespace uwvm::vm::interpreter::func
{
#if __has_cpp_attribute(__gnu__::__cold__)
    [[__gnu__::__cold__]]
#endif
    [[noreturn]] inline void
        unreachable(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
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
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - global_wasm_module.module_begin),
                                u8") "
                                u8"Catch unreachable\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");

        // backtrace

        ::fast_io::fast_terminate();
    }

#if __has_cpp_attribute(__gnu__::__hot__)
    [[__gnu__::__hot__]]
#endif
    inline void block(::std::byte const* curr, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        sm.flow.emplace(curr, ::uwvm::vm::interpreter::flow_control_t::block);
    }

}  // namespace uwvm::vm::interpreter::func
