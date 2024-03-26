#pragma once

#include <algorithm>

#include <fast_io_freestanding.h>

#include <cmdline/impl.h>

#include "inc.h"

namespace uwvm
{

    namespace details
    {
        constexpr const inline ::uwvm::cmdline::parameter* parameter_unsort[]{
#include "parainc.h"
        };
    }  // namespace details

    constexpr inline auto parameters{::uwvm::cmdline::parameter_sort(details::parameter_unsort)};

    constexpr inline ::std::size_t parameter_lookup_table_size{::uwvm::cmdline::calculate_all_parameters_size(parameters)};
    constexpr inline auto parameter_lookup_table{::uwvm::cmdline::expand_all_parameters_and_check<parameter_lookup_table_size>(parameters)};
    constexpr inline ::std::size_t parameter_max_name_size{::uwvm::cmdline::calculate_max_para_size(parameter_lookup_table)};

    constexpr inline auto hash_table_size{::uwvm::cmdline::calculate_hash_table_size(parameter_lookup_table)};
    constexpr inline auto hash_table{
        ::uwvm::cmdline::generate_hash_table<hash_table_size.hash_table_size, hash_table_size.extra_size>(parameter_lookup_table)};
}  // namespace uwvm
