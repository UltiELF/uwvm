#pragma once

struct custom_section_type
{
    unsigned char const* sec_begin{};
    unsigned char const* sec_end{};

    char const* name_begin{};
    char const* name_end{};
};

typedef bool (*handlefunc_ptr)(custom_section_type);

struct csfunc_return_struct_type
{
    handlefunc_ptr func{};
    char const* name_begin{};
};

/*
*       bool test(custom_section_type cst);
* 
*       csfunc_return_struct cs[2]{&test, NULL};
* 
*       csfunc_return_struct const* CustomSection()
*       {
*           return cs;
*       }
*/
