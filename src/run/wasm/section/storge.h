#pragma once
#include <cstddef>
#include "../../../wasm/types.h"
#include "../../../wasm/section.h"

namespace uwvm
{
    inline ::uwvm::wasm::section::type_section global_type_section{};
    inline ::uwvm::wasm::section::function_section global_function_section{};
    inline ::uwvm::wasm::section::import_section global_import_section{};
    inline ::uwvm::wasm::section::table_section global_table_section{};

}
