#pragma once

#if(defined(__linux) || defined(__linux__) || defined(__gnu_linux__)) || defined(__CYGWIN__) || defined(__sun)
    #define UWVM_SUPPORT_INSTALL_PATH
#elif defined(_WIN32)
    #define UWVM_SUPPORT_INSTALL_PATH
#elif defined(__DragonFly__) || defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__NetBSD__) || defined(BSD) || defined(_SYSTYPE_BSD)
    #define UWVM_SUPPORT_INSTALL_PATH
#elif defined(__OpenBSD__)
    #define UWVM_SUPPORT_INSTALL_PATH
#elif defined(__APPLE__)
    #define UWVM_SUPPORT_INSTALL_PATH
#endif
