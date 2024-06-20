#pragma once
#include "ast.h"
#include "aststorge.h"

namespace uwvm::vm::interpreter
{
    inline thread_local ::uwvm::vm::interpreter::stack_machine s{};

    inline void run_ast(ast const& a) noexcept
    {
        if(!a.operators.empty()) [[likely]]
        {
            auto const end_op{::std::to_address(a.operators.end())};
            for(s.curr_op = ::std::to_address(a.operators.begin()); s.curr_op != end_op;)
            {
                if(s.curr_op->int_func) [[likely]] { s.curr_op->int_func(s.curr_op->code_begin, s); }
                else { ++s.curr_op; }
            }
        }
    }
}  // namespace uwvm::vm::interpreter
