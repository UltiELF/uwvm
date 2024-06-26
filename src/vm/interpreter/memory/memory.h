#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
    #if !defined(__CYGWIN__) && !defined(__WINE__) && !defined(__BIONIC__) && defined(_WIN32_WINDOWS) || true 
        #include "win32.h"
    #else
        #include "nt.h"
    #endif
#elif !defined(__NEWLIB__) && !defined(__MSDOS__) && (!defined(__wasm__) || (defined(__wasi__) && defined(_WASI_EMULATED_MMAN))) && __has_include(<sys/mman.h>)
    #include "posix.h"
#else
    #include "allocator.h"
#endif

#include "storage.h"