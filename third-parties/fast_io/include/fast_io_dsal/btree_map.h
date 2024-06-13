#pragma once
#undef min
#undef max

#if !defined(__cplusplus)
    #error "You must be using a C++ compiler"
#endif

#if !defined(__cplusplus)
    #error "You must be using a C++ compiler"
#endif
#if defined(_MSC_VER) && !defined(__clang__)
    #pragma warning(push)
    #pragma warning(disable : 4464)
    #pragma warning(disable : 4514)
    #pragma warning(disable : 4623)
    #pragma warning(disable : 4626)
    #pragma warning(disable : 4668)
    #pragma warning(disable : 4710)
    #pragma warning(disable : 4820)
    #pragma warning(disable : 5027)
    #pragma warning(disable : 5045)
    #include <cstring>
#endif

#include "../fast_io_core.h"
#include "impl/btree_map.h"

namespace fast_io
{

    template <::std::three_way_comparable key_type, typename value_type>
    using btree_map = ::fast_io::containers::btree_map<key_type, value_type>;

    namespace tlc
    {
        template <::std::three_way_comparable key_type, typename value_type>
        using btree_map = ::fast_io::containers::btree_map<key_type, value_type>;

    }  // namespace tlc

}  // namespace fast_io

#if defined(_MSC_VER) && !defined(__clang__)
    #pragma warning(pop)
#endif
