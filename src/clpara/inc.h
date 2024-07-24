#pragma once

#include "parameters/version.h"
#include "parameters/help.h"
#include "parameters/abi.h"
#include "parameters/run.h"
#include "parameters/enable-memory64.h"
#include "parameters/mode.h"
#include "parameters/load.h"
#include "parameters/default-int-stack-size.h"
#include "parameters/default-local-size.h"
#include "parameters/wasi-mount-root.h"
#include "parameters/start-func.h"

// features
#include "parameters/enable-relaxed-simd.h"
#include "parameters/enable-exception-handling.h"
#include "parameters/enable-multi-memory.h"
#include "parameters/enable-fixed-width-simd.h"
#include "parameters/enable-thread.h"
#include "parameters/enable-custom-page-sizes.h"

#ifdef UWVM_DEFAULT_MVP
    #include "parameters/enable-multi-value.h"
    #include "parameters/enable-mutable-global.h"
    #include "parameters/enable-reference-types.h"
    #include "parameters/enable-sign-extension-ops.h"
    #include "parameters/enable-bulk-mem-ops.h"
    #include "parameters/enable-non-trapping-float-to-int-conversions.h"

    // features collection
    #include "parameters/enable-regular-extension.h"
#endif
