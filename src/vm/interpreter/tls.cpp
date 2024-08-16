// duplicate symbol: thread-local initialization routine
// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=115686

#include "astgen.h"
#include "astrun.h"

#if !(defined(__wasi__) && !defined(UWVM_ENABLE_WASI_THREADS))
thread_local ::fast_io::tlc::stack<::uwvm::vm::interpreter::details::d_flow_t, ::fast_io::tlc::vector<::uwvm::vm::interpreter::details::d_flow_t>>(
    ::uwvm::vm::interpreter::details::ga_flow){};

thread_local ::uwvm::vm::interpreter::stack_machine(::uwvm::vm::interpreter::uwvm_sm){};

thread_local ::fast_io::tlc::stack<::fast_io::tlc::u8string, ::fast_io::tlc::vector<::fast_io::tlc::u8string>>(::uwvm::vm::interpreter::int_call_stack){};

#endif
