#pragma once
#include <version>
#ifndef __cpp_concepts
    #error "uwvm requires at least C++20 standard compiler."
#endif
#ifndef __cpp_lib_three_way_comparison
    #error "unsupported c++ std library"
#endif
