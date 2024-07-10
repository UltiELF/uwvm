#pragma once

#include <algorithm>

#include <fast_io_freestanding.h>

#include <cmdline/impl.h>

#include "inc.h"

namespace uwvm
{

    namespace details
    {
        inline constexpr ::uwvm::cmdline::parameter const* parameter_unsort[]{
#include "parainc.h"
        };
    }  // namespace details

    inline constexpr auto parameters{::uwvm::cmdline::parameter_sort(details::parameter_unsort)};

    inline constexpr ::std::size_t parameter_lookup_table_size{::uwvm::cmdline::calculate_all_parameters_size(parameters)};
    inline constexpr auto parameter_lookup_table{::uwvm::cmdline::expand_all_parameters_and_check<parameter_lookup_table_size>(parameters)};
    inline constexpr ::std::size_t parameter_max_name_size{::uwvm::cmdline::calculate_max_para_size(parameter_lookup_table)};

    inline constexpr auto hash_table_size{::uwvm::cmdline::calculate_hash_table_size(parameter_lookup_table)};
    inline constexpr auto hash_table{::uwvm::cmdline::generate_hash_table<hash_table_size.hash_table_size, hash_table_size.extra_size>(parameter_lookup_table)};

    inline constexpr auto sizeof_hash_table{sizeof(hash_table)};
}  // namespace uwvm
