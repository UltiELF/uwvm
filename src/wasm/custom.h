#pragma once
#include "custom/basic.h"
#include "custom/builtin.h"
#include "custom/manager.h"
#if(defined(_WIN32) || defined(__CYGWIN__)) ||                                                                                                                 \
    ((!defined(_WIN32) || defined(__WINE__)) && (defined(__CYGWIN__) || (!defined(__NEWLIB__) && !defined(__wasi__))))
    #include "custom/import.h"
#endif
