﻿#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>

extern "C" {
struct ident_t;

using kmp_int32 = int32_t;
using kmp_int64 = int64_t;
using kmp_routine_entry_t = kmp_int32 (*)(kmp_int32, void *);
using kmp_intptr_t = intptr_t;

typedef struct kmp_depend_info {
  kmp_intptr_t base_addr;
  size_t len;
  union {
    unsigned char flag;
    struct {
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
      unsigned all : 1;
      unsigned unused : 3;
      unsigned set : 1;
      unsigned mtx : 1;
      unsigned out : 1;
      unsigned in : 1;
#else
      unsigned in : 1;
      unsigned out : 1;
      unsigned mtx : 1;
      unsigned set : 1;
      unsigned unused : 3;
      unsigned all : 1;
#endif
    } flags;
  };
} kmp_depend_info_t;

typedef union kmp_cmplrdata {
  kmp_int32 priority;
  kmp_routine_entry_t destructors;
} kmp_cmplrdata_t;

typedef struct kmp_task {
  void *shareds;
  kmp_routine_entry_t routine;
  kmp_int32 part_id;
  kmp_cmplrdata_t data1;
  kmp_cmplrdata_t data2;
} kmp_task_t;

int32_t __kmpc_global_thread_num(void *);
kmp_task_t *__kmpc_omp_task_alloc(ident_t *, kmp_int32, kmp_int32, size_t,
                                  size_t, kmp_routine_entry_t);
kmp_task_t *__kmpc_omp_target_task_alloc(ident_t *, kmp_int32, kmp_int32,
                                         size_t, size_t, kmp_routine_entry_t,
                                         kmp_int64);
kmp_int32 __kmpc_omp_taskwait(ident_t *, kmp_int32);
kmp_int32 __kmpc_omp_task(ident_t *, kmp_int32, kmp_task_t *);
kmp_int32 __kmpc_omp_task_with_deps(ident_t *loc_ref, kmp_int32 gtid,
                                    kmp_task_t *new_task, kmp_int32 ndeps,
                                    kmp_depend_info_t *dep_list,
                                    kmp_int32 ndeps_noalias,
                                    kmp_depend_info_t *noalias_dep_list);
void __kmpc_taskgroup(ident_t *, kmp_int32);
void __kmpc_end_taskgroup(ident_t *, kmp_int32);
}

static kmp_int32 get_num_hidden_helper_threads() {
  static kmp_int32 __kmp_hidden_helper_threads_num = 8;
  if (const char *env = std::getenv("LIBOMP_NUM_HIDDEN_HELPER_THREADS")) {
    return std::stoi(env);
  }
  return __kmp_hidden_helper_threads_num;
}
