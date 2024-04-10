#pragma once
#include <version>

// __cpp_concepts
#ifndef __cpp_concepts
    #error "uwvm requires at least C++20 standard compiler."
#endif

// __cpp_lib_three_way_comparison
#if 0
    #ifndef __cpp_lib_three_way_comparison
    // libcxx not supported yet: https://github.com/llvm/llvm-project/blob/main/libcxx%2Finclude%2Fversion
        #error "unsupported c++ std library"
    #endif
#endif
