#pragma once
#include <cstddef>
#include <limits>
#include <fast_io.h>
#ifdef UWVM_TIMER
    #include <fast_io_driver/timer.h>
#endif
#include <io_device.h>
#include "../../check_index.h"
#include "../../../wasm/section/function.h"
#include "../../../clpara/parameters/enable-memory64.h"
#include "type.h"

namespace uwvm
{
    inline ::uwvm::wasm::section::function_section global_function_section{};

}  // namespace uwvm

#include "function/function_normal.h"