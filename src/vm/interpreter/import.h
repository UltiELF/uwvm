#pragma once
#include <fast_io.h>
#include <fast_io_dsal/vector.h>
#include "ast.h"

namespace uwvm::vm::interpreter
{
    using int_import_func_p = void (*)(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept;

    inline ::fast_io::vector<int_import_func_p> imports{};

}  // namespace uwvm::vm::interpreter

