#pragma once
#include <fast_io.h>
#include <fast_io_device.h>
#include "../io_device.h"

#include "storge.h"
#include "argv0.h"

#if(defined(__linux) || defined(__linux__) || defined(__gnu_linux__)) || defined(__CYGWIN__) || defined(__sun)
    #include "linux.h"
#elif defined(_WIN32)
    #if defined(_WIN32_WINDOWS)
        #include "win32.h"
    #else
        #include "nt.h"
    #endif
#elif defined(__DragonFly__) || defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__NetBSD__) || defined(BSD) || defined(_SYSTYPE_BSD)
    #include "bsd.h"
#elif defined(__OpenBSD__)
    #include "openbsd.h"
#elif defined(__APPLE__)
    #include "darwin.h"
#else
    #include "null.h"
#endif
