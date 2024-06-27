// duplicate symbol: thread-local initialization routine

#include "astgen.h"
#include "astrun.h"

thread_local ::fast_io::tlc::stack<::uwvm::vm::interpreter::details::d_flow_t, ::fast_io::tlc::vector<::uwvm::vm::interpreter::details::d_flow_t>>(
    ::uwvm::vm::interpreter::details::ga_flow){};

thread_local ::uwvm::vm::interpreter::stack_machine(::uwvm::vm::interpreter::s){};

