; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc -aarch64-sve-vector-bits-min=256  < %s | FileCheck %s -check-prefixes=CHECK,VBITS_GE_256
; RUN: llc -aarch64-sve-vector-bits-min=512  < %s | FileCheck %s -check-prefixes=CHECK,VBITS_GE_512
; RUN: llc -aarch64-sve-vector-bits-min=2048 < %s | FileCheck %s -check-prefixes=CHECK,VBITS_GE_512

target triple = "aarch64-unknown-linux-gnu"

;
; RBIT
;

define <8 x i8> @bitreverse_v8i8(<8 x i8> %op) vscale_range(2,0) #0 {
; CHECK-LABEL: bitreverse_v8i8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.b, vl8
; CHECK-NEXT:    // kill: def $d0 killed $d0 def $z0
; CHECK-NEXT:    rbit z0.b, p0/m, z0.b
; CHECK-NEXT:    // kill: def $d0 killed $d0 killed $z0
; CHECK-NEXT:    ret
  %res = call <8 x i8> @llvm.bitreverse.v8i8(<8 x i8> %op)
  ret <8 x i8> %res
}

define <16 x i8> @bitreverse_v16i8(<16 x i8> %op) vscale_range(2,0) #0 {
; CHECK-LABEL: bitreverse_v16i8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.b, vl16
; CHECK-NEXT:    // kill: def $q0 killed $q0 def $z0
; CHECK-NEXT:    rbit z0.b, p0/m, z0.b
; CHECK-NEXT:    // kill: def $q0 killed $q0 killed $z0
; CHECK-NEXT:    ret
  %res = call <16 x i8> @llvm.bitreverse.v16i8(<16 x i8> %op)
  ret <16 x i8> %res
}

define void @bitreverse_v32i8(ptr %a) vscale_range(2,0) #0 {
; CHECK-LABEL: bitreverse_v32i8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.b, vl32
; CHECK-NEXT:    ld1b { z0.b }, p0/z, [x0]
; CHECK-NEXT:    rbit z0.b, p0/m, z0.b
; CHECK-NEXT:    st1b { z0.b }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <32 x i8>, ptr %a
  %res = call <32 x i8> @llvm.bitreverse.v32i8(<32 x i8> %op)
  store <32 x i8> %res, ptr %a
  ret void
}

define void @bitreverse_v64i8(ptr %a) #0 {
; VBITS_GE_256-LABEL: bitreverse_v64i8:
; VBITS_GE_256:       // %bb.0:
; VBITS_GE_256-NEXT:    ptrue p0.b, vl32
; VBITS_GE_256-NEXT:    mov w8, #32 // =0x20
; VBITS_GE_256-NEXT:    ld1b { z0.b }, p0/z, [x0, x8]
; VBITS_GE_256-NEXT:    ld1b { z1.b }, p0/z, [x0]
; VBITS_GE_256-NEXT:    rbit z0.b, p0/m, z0.b
; VBITS_GE_256-NEXT:    rbit z1.b, p0/m, z1.b
; VBITS_GE_256-NEXT:    st1b { z0.b }, p0, [x0, x8]
; VBITS_GE_256-NEXT:    st1b { z1.b }, p0, [x0]
; VBITS_GE_256-NEXT:    ret
;
; VBITS_GE_512-LABEL: bitreverse_v64i8:
; VBITS_GE_512:       // %bb.0:
; VBITS_GE_512-NEXT:    ptrue p0.b, vl64
; VBITS_GE_512-NEXT:    ld1b { z0.b }, p0/z, [x0]
; VBITS_GE_512-NEXT:    rbit z0.b, p0/m, z0.b
; VBITS_GE_512-NEXT:    st1b { z0.b }, p0, [x0]
; VBITS_GE_512-NEXT:    ret
  %op = load <64 x i8>, ptr %a
  %res = call <64 x i8> @llvm.bitreverse.v64i8(<64 x i8> %op)
  store <64 x i8> %res, ptr %a
  ret void
}

define void @bitreverse_v128i8(ptr %a) vscale_range(8,0) #0 {
; CHECK-LABEL: bitreverse_v128i8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.b, vl128
; CHECK-NEXT:    ld1b { z0.b }, p0/z, [x0]
; CHECK-NEXT:    rbit z0.b, p0/m, z0.b
; CHECK-NEXT:    st1b { z0.b }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <128 x i8>, ptr %a
  %res = call <128 x i8> @llvm.bitreverse.v128i8(<128 x i8> %op)
  store <128 x i8> %res, ptr %a
  ret void
}

define void @bitreverse_v256i8(ptr %a) vscale_range(16,0) #0 {
; CHECK-LABEL: bitreverse_v256i8:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.b, vl256
; CHECK-NEXT:    ld1b { z0.b }, p0/z, [x0]
; CHECK-NEXT:    rbit z0.b, p0/m, z0.b
; CHECK-NEXT:    st1b { z0.b }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <256 x i8>, ptr %a
  %res = call <256 x i8> @llvm.bitreverse.v256i8(<256 x i8> %op)
  store <256 x i8> %res, ptr %a
  ret void
}

define <4 x i16> @bitreverse_v4i16(<4 x i16> %op) vscale_range(2,0) #0 {
; CHECK-LABEL: bitreverse_v4i16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.h, vl4
; CHECK-NEXT:    // kill: def $d0 killed $d0 def $z0
; CHECK-NEXT:    rbit z0.h, p0/m, z0.h
; CHECK-NEXT:    // kill: def $d0 killed $d0 killed $z0
; CHECK-NEXT:    ret
  %res = call <4 x i16> @llvm.bitreverse.v4i16(<4 x i16> %op)
  ret <4 x i16> %res
}

define <8 x i16> @bitreverse_v8i16(<8 x i16> %op) vscale_range(2,0) #0 {
; CHECK-LABEL: bitreverse_v8i16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.h, vl8
; CHECK-NEXT:    // kill: def $q0 killed $q0 def $z0
; CHECK-NEXT:    rbit z0.h, p0/m, z0.h
; CHECK-NEXT:    // kill: def $q0 killed $q0 killed $z0
; CHECK-NEXT:    ret
  %res = call <8 x i16> @llvm.bitreverse.v8i16(<8 x i16> %op)
  ret <8 x i16> %res
}

define void @bitreverse_v16i16(ptr %a) vscale_range(2,0) #0 {
; CHECK-LABEL: bitreverse_v16i16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.h, vl16
; CHECK-NEXT:    ld1h { z0.h }, p0/z, [x0]
; CHECK-NEXT:    rbit z0.h, p0/m, z0.h
; CHECK-NEXT:    st1h { z0.h }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <16 x i16>, ptr %a
  %res = call <16 x i16> @llvm.bitreverse.v16i16(<16 x i16> %op)
  store <16 x i16> %res, ptr %a
  ret void
}

define void @bitreverse_v32i16(ptr %a) #0 {
; VBITS_GE_256-LABEL: bitreverse_v32i16:
; VBITS_GE_256:       // %bb.0:
; VBITS_GE_256-NEXT:    ptrue p0.h, vl16
; VBITS_GE_256-NEXT:    mov x8, #16 // =0x10
; VBITS_GE_256-NEXT:    ld1h { z0.h }, p0/z, [x0, x8, lsl #1]
; VBITS_GE_256-NEXT:    ld1h { z1.h }, p0/z, [x0]
; VBITS_GE_256-NEXT:    rbit z0.h, p0/m, z0.h
; VBITS_GE_256-NEXT:    rbit z1.h, p0/m, z1.h
; VBITS_GE_256-NEXT:    st1h { z0.h }, p0, [x0, x8, lsl #1]
; VBITS_GE_256-NEXT:    st1h { z1.h }, p0, [x0]
; VBITS_GE_256-NEXT:    ret
;
; VBITS_GE_512-LABEL: bitreverse_v32i16:
; VBITS_GE_512:       // %bb.0:
; VBITS_GE_512-NEXT:    ptrue p0.h, vl32
; VBITS_GE_512-NEXT:    ld1h { z0.h }, p0/z, [x0]
; VBITS_GE_512-NEXT:    rbit z0.h, p0/m, z0.h
; VBITS_GE_512-NEXT:    st1h { z0.h }, p0, [x0]
; VBITS_GE_512-NEXT:    ret
  %op = load <32 x i16>, ptr %a
  %res = call <32 x i16> @llvm.bitreverse.v32i16(<32 x i16> %op)
  store <32 x i16> %res, ptr %a
  ret void
}

define void @bitreverse_v64i16(ptr %a) vscale_range(8,0) #0 {
; CHECK-LABEL: bitreverse_v64i16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.h, vl64
; CHECK-NEXT:    ld1h { z0.h }, p0/z, [x0]
; CHECK-NEXT:    rbit z0.h, p0/m, z0.h
; CHECK-NEXT:    st1h { z0.h }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <64 x i16>, ptr %a
  %res = call <64 x i16> @llvm.bitreverse.v64i16(<64 x i16> %op)
  store <64 x i16> %res, ptr %a
  ret void
}

define void @bitreverse_v128i16(ptr %a) vscale_range(16,0) #0 {
; CHECK-LABEL: bitreverse_v128i16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.h, vl128
; CHECK-NEXT:    ld1h { z0.h }, p0/z, [x0]
; CHECK-NEXT:    rbit z0.h, p0/m, z0.h
; CHECK-NEXT:    st1h { z0.h }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <128 x i16>, ptr %a
  %res = call <128 x i16> @llvm.bitreverse.v128i16(<128 x i16> %op)
  store <128 x i16> %res, ptr %a
  ret void
}

define <2 x i32> @bitreverse_v2i32(<2 x i32> %op) vscale_range(2,0) #0 {
; CHECK-LABEL: bitreverse_v2i32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s, vl2
; CHECK-NEXT:    // kill: def $d0 killed $d0 def $z0
; CHECK-NEXT:    rbit z0.s, p0/m, z0.s
; CHECK-NEXT:    // kill: def $d0 killed $d0 killed $z0
; CHECK-NEXT:    ret
  %res = call <2 x i32> @llvm.bitreverse.v2i32(<2 x i32> %op)
  ret <2 x i32> %res
}

define <4 x i32> @bitreverse_v4i32(<4 x i32> %op) vscale_range(2,0) #0 {
; CHECK-LABEL: bitreverse_v4i32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s, vl4
; CHECK-NEXT:    // kill: def $q0 killed $q0 def $z0
; CHECK-NEXT:    rbit z0.s, p0/m, z0.s
; CHECK-NEXT:    // kill: def $q0 killed $q0 killed $z0
; CHECK-NEXT:    ret
  %res = call <4 x i32> @llvm.bitreverse.v4i32(<4 x i32> %op)
  ret <4 x i32> %res
}

define void @bitreverse_v8i32(ptr %a) vscale_range(2,0) #0 {
; CHECK-LABEL: bitreverse_v8i32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s, vl8
; CHECK-NEXT:    ld1w { z0.s }, p0/z, [x0]
; CHECK-NEXT:    rbit z0.s, p0/m, z0.s
; CHECK-NEXT:    st1w { z0.s }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <8 x i32>, ptr %a
  %res = call <8 x i32> @llvm.bitreverse.v8i32(<8 x i32> %op)
  store <8 x i32> %res, ptr %a
  ret void
}

define void @bitreverse_v16i32(ptr %a) #0 {
; VBITS_GE_256-LABEL: bitreverse_v16i32:
; VBITS_GE_256:       // %bb.0:
; VBITS_GE_256-NEXT:    ptrue p0.s, vl8
; VBITS_GE_256-NEXT:    mov x8, #8 // =0x8
; VBITS_GE_256-NEXT:    ld1w { z0.s }, p0/z, [x0, x8, lsl #2]
; VBITS_GE_256-NEXT:    ld1w { z1.s }, p0/z, [x0]
; VBITS_GE_256-NEXT:    rbit z0.s, p0/m, z0.s
; VBITS_GE_256-NEXT:    rbit z1.s, p0/m, z1.s
; VBITS_GE_256-NEXT:    st1w { z0.s }, p0, [x0, x8, lsl #2]
; VBITS_GE_256-NEXT:    st1w { z1.s }, p0, [x0]
; VBITS_GE_256-NEXT:    ret
;
; VBITS_GE_512-LABEL: bitreverse_v16i32:
; VBITS_GE_512:       // %bb.0:
; VBITS_GE_512-NEXT:    ptrue p0.s, vl16
; VBITS_GE_512-NEXT:    ld1w { z0.s }, p0/z, [x0]
; VBITS_GE_512-NEXT:    rbit z0.s, p0/m, z0.s
; VBITS_GE_512-NEXT:    st1w { z0.s }, p0, [x0]
; VBITS_GE_512-NEXT:    ret
  %op = load <16 x i32>, ptr %a
  %res = call <16 x i32> @llvm.bitreverse.v16i32(<16 x i32> %op)
  store <16 x i32> %res, ptr %a
  ret void
}

define void @bitreverse_v32i32(ptr %a) vscale_range(8,0) #0 {
; CHECK-LABEL: bitreverse_v32i32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s, vl32
; CHECK-NEXT:    ld1w { z0.s }, p0/z, [x0]
; CHECK-NEXT:    rbit z0.s, p0/m, z0.s
; CHECK-NEXT:    st1w { z0.s }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <32 x i32>, ptr %a
  %res = call <32 x i32> @llvm.bitreverse.v32i32(<32 x i32> %op)
  store <32 x i32> %res, ptr %a
  ret void
}

define void @bitreverse_v64i32(ptr %a) vscale_range(16,0) #0 {
; CHECK-LABEL: bitreverse_v64i32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s, vl64
; CHECK-NEXT:    ld1w { z0.s }, p0/z, [x0]
; CHECK-NEXT:    rbit z0.s, p0/m, z0.s
; CHECK-NEXT:    st1w { z0.s }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <64 x i32>, ptr %a
  %res = call <64 x i32> @llvm.bitreverse.v64i32(<64 x i32> %op)
  store <64 x i32> %res, ptr %a
  ret void
}

define <1 x i64> @bitreverse_v1i64(<1 x i64> %op) vscale_range(2,0) #0 {
; CHECK-LABEL: bitreverse_v1i64:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d, vl1
; CHECK-NEXT:    // kill: def $d0 killed $d0 def $z0
; CHECK-NEXT:    rbit z0.d, p0/m, z0.d
; CHECK-NEXT:    // kill: def $d0 killed $d0 killed $z0
; CHECK-NEXT:    ret
  %res = call <1 x i64> @llvm.bitreverse.v1i64(<1 x i64> %op)
  ret <1 x i64> %res
}

define <2 x i64> @bitreverse_v2i64(<2 x i64> %op) vscale_range(2,0) #0 {
; CHECK-LABEL: bitreverse_v2i64:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d, vl2
; CHECK-NEXT:    // kill: def $q0 killed $q0 def $z0
; CHECK-NEXT:    rbit z0.d, p0/m, z0.d
; CHECK-NEXT:    // kill: def $q0 killed $q0 killed $z0
; CHECK-NEXT:    ret
  %res = call <2 x i64> @llvm.bitreverse.v2i64(<2 x i64> %op)
  ret <2 x i64> %res
}

define void @bitreverse_v4i64(ptr %a) vscale_range(2,0) #0 {
; CHECK-LABEL: bitreverse_v4i64:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d, vl4
; CHECK-NEXT:    ld1d { z0.d }, p0/z, [x0]
; CHECK-NEXT:    rbit z0.d, p0/m, z0.d
; CHECK-NEXT:    st1d { z0.d }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <4 x i64>, ptr %a
  %res = call <4 x i64> @llvm.bitreverse.v4i64(<4 x i64> %op)
  store <4 x i64> %res, ptr %a
  ret void
}

define void @bitreverse_v8i64(ptr %a) #0 {
; VBITS_GE_256-LABEL: bitreverse_v8i64:
; VBITS_GE_256:       // %bb.0:
; VBITS_GE_256-NEXT:    ptrue p0.d, vl4
; VBITS_GE_256-NEXT:    mov x8, #4 // =0x4
; VBITS_GE_256-NEXT:    ld1d { z0.d }, p0/z, [x0, x8, lsl #3]
; VBITS_GE_256-NEXT:    ld1d { z1.d }, p0/z, [x0]
; VBITS_GE_256-NEXT:    rbit z0.d, p0/m, z0.d
; VBITS_GE_256-NEXT:    rbit z1.d, p0/m, z1.d
; VBITS_GE_256-NEXT:    st1d { z0.d }, p0, [x0, x8, lsl #3]
; VBITS_GE_256-NEXT:    st1d { z1.d }, p0, [x0]
; VBITS_GE_256-NEXT:    ret
;
; VBITS_GE_512-LABEL: bitreverse_v8i64:
; VBITS_GE_512:       // %bb.0:
; VBITS_GE_512-NEXT:    ptrue p0.d, vl8
; VBITS_GE_512-NEXT:    ld1d { z0.d }, p0/z, [x0]
; VBITS_GE_512-NEXT:    rbit z0.d, p0/m, z0.d
; VBITS_GE_512-NEXT:    st1d { z0.d }, p0, [x0]
; VBITS_GE_512-NEXT:    ret
  %op = load <8 x i64>, ptr %a
  %res = call <8 x i64> @llvm.bitreverse.v8i64(<8 x i64> %op)
  store <8 x i64> %res, ptr %a
  ret void
}

define void @bitreverse_v16i64(ptr %a) vscale_range(8,0) #0 {
; CHECK-LABEL: bitreverse_v16i64:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d, vl16
; CHECK-NEXT:    ld1d { z0.d }, p0/z, [x0]
; CHECK-NEXT:    rbit z0.d, p0/m, z0.d
; CHECK-NEXT:    st1d { z0.d }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <16 x i64>, ptr %a
  %res = call <16 x i64> @llvm.bitreverse.v16i64(<16 x i64> %op)
  store <16 x i64> %res, ptr %a
  ret void
}

define void @bitreverse_v32i64(ptr %a) vscale_range(16,0) #0 {
; CHECK-LABEL: bitreverse_v32i64:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d, vl32
; CHECK-NEXT:    ld1d { z0.d }, p0/z, [x0]
; CHECK-NEXT:    rbit z0.d, p0/m, z0.d
; CHECK-NEXT:    st1d { z0.d }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <32 x i64>, ptr %a
  %res = call <32 x i64> @llvm.bitreverse.v32i64(<32 x i64> %op)
  store <32 x i64> %res, ptr %a
  ret void
}

;
; REVB
;

; Don't use SVE for 64-bit vectors.
define <4 x i16> @bswap_v4i16(<4 x i16> %op) vscale_range(2,0) #0 {
; CHECK-LABEL: bswap_v4i16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    rev16 v0.8b, v0.8b
; CHECK-NEXT:    ret
  %res = call <4 x i16> @llvm.bswap.v4i16(<4 x i16> %op)
  ret <4 x i16> %res
}

; Don't use SVE for 128-bit vectors.
define <8 x i16> @bswap_v8i16(<8 x i16> %op) vscale_range(2,0) #0 {
; CHECK-LABEL: bswap_v8i16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    rev16 v0.16b, v0.16b
; CHECK-NEXT:    ret
  %res = call <8 x i16> @llvm.bswap.v8i16(<8 x i16> %op)
  ret <8 x i16> %res
}

define void @bswap_v16i16(ptr %a) vscale_range(2,0) #0 {
; CHECK-LABEL: bswap_v16i16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.h, vl16
; CHECK-NEXT:    ld1h { z0.h }, p0/z, [x0]
; CHECK-NEXT:    revb z0.h, p0/m, z0.h
; CHECK-NEXT:    st1h { z0.h }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <16 x i16>, ptr %a
  %res = call <16 x i16> @llvm.bswap.v16i16(<16 x i16> %op)
  store <16 x i16> %res, ptr %a
  ret void
}

define void @bswap_v32i16(ptr %a) #0 {
; VBITS_GE_256-LABEL: bswap_v32i16:
; VBITS_GE_256:       // %bb.0:
; VBITS_GE_256-NEXT:    ptrue p0.h, vl16
; VBITS_GE_256-NEXT:    mov x8, #16 // =0x10
; VBITS_GE_256-NEXT:    ld1h { z0.h }, p0/z, [x0, x8, lsl #1]
; VBITS_GE_256-NEXT:    ld1h { z1.h }, p0/z, [x0]
; VBITS_GE_256-NEXT:    revb z0.h, p0/m, z0.h
; VBITS_GE_256-NEXT:    revb z1.h, p0/m, z1.h
; VBITS_GE_256-NEXT:    st1h { z0.h }, p0, [x0, x8, lsl #1]
; VBITS_GE_256-NEXT:    st1h { z1.h }, p0, [x0]
; VBITS_GE_256-NEXT:    ret
;
; VBITS_GE_512-LABEL: bswap_v32i16:
; VBITS_GE_512:       // %bb.0:
; VBITS_GE_512-NEXT:    ptrue p0.h, vl32
; VBITS_GE_512-NEXT:    ld1h { z0.h }, p0/z, [x0]
; VBITS_GE_512-NEXT:    revb z0.h, p0/m, z0.h
; VBITS_GE_512-NEXT:    st1h { z0.h }, p0, [x0]
; VBITS_GE_512-NEXT:    ret
  %op = load <32 x i16>, ptr %a
  %res = call <32 x i16> @llvm.bswap.v32i16(<32 x i16> %op)
  store <32 x i16> %res, ptr %a
  ret void
}

define void @bswap_v64i16(ptr %a) vscale_range(8,0) #0 {
; CHECK-LABEL: bswap_v64i16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.h, vl64
; CHECK-NEXT:    ld1h { z0.h }, p0/z, [x0]
; CHECK-NEXT:    revb z0.h, p0/m, z0.h
; CHECK-NEXT:    st1h { z0.h }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <64 x i16>, ptr %a
  %res = call <64 x i16> @llvm.bswap.v64i16(<64 x i16> %op)
  store <64 x i16> %res, ptr %a
  ret void
}

define void @bswap_v128i16(ptr %a) vscale_range(16,0) #0 {
; CHECK-LABEL: bswap_v128i16:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.h, vl128
; CHECK-NEXT:    ld1h { z0.h }, p0/z, [x0]
; CHECK-NEXT:    revb z0.h, p0/m, z0.h
; CHECK-NEXT:    st1h { z0.h }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <128 x i16>, ptr %a
  %res = call <128 x i16> @llvm.bswap.v128i16(<128 x i16> %op)
  store <128 x i16> %res, ptr %a
  ret void
}

; Don't use SVE for 64-bit vectors.
define <2 x i32> @bswap_v2i32(<2 x i32> %op) vscale_range(2,0) #0 {
; CHECK-LABEL: bswap_v2i32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    rev32 v0.8b, v0.8b
; CHECK-NEXT:    ret
  %res = call <2 x i32> @llvm.bswap.v2i32(<2 x i32> %op)
  ret <2 x i32> %res
}

; Don't use SVE for 128-bit vectors.
define <4 x i32> @bswap_v4i32(<4 x i32> %op) vscale_range(2,0) #0 {
; CHECK-LABEL: bswap_v4i32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    rev32 v0.16b, v0.16b
; CHECK-NEXT:    ret
  %res = call <4 x i32> @llvm.bswap.v4i32(<4 x i32> %op)
  ret <4 x i32> %res
}

define void @bswap_v8i32(ptr %a) vscale_range(2,0) #0 {
; CHECK-LABEL: bswap_v8i32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s, vl8
; CHECK-NEXT:    ld1w { z0.s }, p0/z, [x0]
; CHECK-NEXT:    revb z0.s, p0/m, z0.s
; CHECK-NEXT:    st1w { z0.s }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <8 x i32>, ptr %a
  %res = call <8 x i32> @llvm.bswap.v8i32(<8 x i32> %op)
  store <8 x i32> %res, ptr %a
  ret void
}

define void @bswap_v16i32(ptr %a) #0 {
; VBITS_GE_256-LABEL: bswap_v16i32:
; VBITS_GE_256:       // %bb.0:
; VBITS_GE_256-NEXT:    ptrue p0.s, vl8
; VBITS_GE_256-NEXT:    mov x8, #8 // =0x8
; VBITS_GE_256-NEXT:    ld1w { z0.s }, p0/z, [x0, x8, lsl #2]
; VBITS_GE_256-NEXT:    ld1w { z1.s }, p0/z, [x0]
; VBITS_GE_256-NEXT:    revb z0.s, p0/m, z0.s
; VBITS_GE_256-NEXT:    revb z1.s, p0/m, z1.s
; VBITS_GE_256-NEXT:    st1w { z0.s }, p0, [x0, x8, lsl #2]
; VBITS_GE_256-NEXT:    st1w { z1.s }, p0, [x0]
; VBITS_GE_256-NEXT:    ret
;
; VBITS_GE_512-LABEL: bswap_v16i32:
; VBITS_GE_512:       // %bb.0:
; VBITS_GE_512-NEXT:    ptrue p0.s, vl16
; VBITS_GE_512-NEXT:    ld1w { z0.s }, p0/z, [x0]
; VBITS_GE_512-NEXT:    revb z0.s, p0/m, z0.s
; VBITS_GE_512-NEXT:    st1w { z0.s }, p0, [x0]
; VBITS_GE_512-NEXT:    ret
  %op = load <16 x i32>, ptr %a
  %res = call <16 x i32> @llvm.bswap.v16i32(<16 x i32> %op)
  store <16 x i32> %res, ptr %a
  ret void
}

define void @bswap_v32i32(ptr %a) vscale_range(8,0) #0 {
; CHECK-LABEL: bswap_v32i32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s, vl32
; CHECK-NEXT:    ld1w { z0.s }, p0/z, [x0]
; CHECK-NEXT:    revb z0.s, p0/m, z0.s
; CHECK-NEXT:    st1w { z0.s }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <32 x i32>, ptr %a
  %res = call <32 x i32> @llvm.bswap.v32i32(<32 x i32> %op)
  store <32 x i32> %res, ptr %a
  ret void
}

define void @bswap_v64i32(ptr %a) vscale_range(16,0) #0 {
; CHECK-LABEL: bswap_v64i32:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.s, vl64
; CHECK-NEXT:    ld1w { z0.s }, p0/z, [x0]
; CHECK-NEXT:    revb z0.s, p0/m, z0.s
; CHECK-NEXT:    st1w { z0.s }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <64 x i32>, ptr %a
  %res = call <64 x i32> @llvm.bswap.v64i32(<64 x i32> %op)
  store <64 x i32> %res, ptr %a
  ret void
}

; Don't use SVE for 64-bit vectors.
define <1 x i64> @bswap_v1i64(<1 x i64> %op) vscale_range(2,0) #0 {
; CHECK-LABEL: bswap_v1i64:
; CHECK:       // %bb.0:
; CHECK-NEXT:    rev64 v0.8b, v0.8b
; CHECK-NEXT:    ret
  %res = call <1 x i64> @llvm.bswap.v1i64(<1 x i64> %op)
  ret <1 x i64> %res
}

; Don't use SVE for 128-bit vectors.
define <2 x i64> @bswap_v2i64(<2 x i64> %op) vscale_range(2,0) #0 {
; CHECK-LABEL: bswap_v2i64:
; CHECK:       // %bb.0:
; CHECK-NEXT:    rev64 v0.16b, v0.16b
; CHECK-NEXT:    ret
  %res = call <2 x i64> @llvm.bswap.v2i64(<2 x i64> %op)
  ret <2 x i64> %res
}

define void @bswap_v4i64(ptr %a) vscale_range(2,0) #0 {
; CHECK-LABEL: bswap_v4i64:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d, vl4
; CHECK-NEXT:    ld1d { z0.d }, p0/z, [x0]
; CHECK-NEXT:    revb z0.d, p0/m, z0.d
; CHECK-NEXT:    st1d { z0.d }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <4 x i64>, ptr %a
  %res = call <4 x i64> @llvm.bswap.v4i64(<4 x i64> %op)
  store <4 x i64> %res, ptr %a
  ret void
}

define void @bswap_v8i64(ptr %a) #0 {
; VBITS_GE_256-LABEL: bswap_v8i64:
; VBITS_GE_256:       // %bb.0:
; VBITS_GE_256-NEXT:    ptrue p0.d, vl4
; VBITS_GE_256-NEXT:    mov x8, #4 // =0x4
; VBITS_GE_256-NEXT:    ld1d { z0.d }, p0/z, [x0, x8, lsl #3]
; VBITS_GE_256-NEXT:    ld1d { z1.d }, p0/z, [x0]
; VBITS_GE_256-NEXT:    revb z0.d, p0/m, z0.d
; VBITS_GE_256-NEXT:    revb z1.d, p0/m, z1.d
; VBITS_GE_256-NEXT:    st1d { z0.d }, p0, [x0, x8, lsl #3]
; VBITS_GE_256-NEXT:    st1d { z1.d }, p0, [x0]
; VBITS_GE_256-NEXT:    ret
;
; VBITS_GE_512-LABEL: bswap_v8i64:
; VBITS_GE_512:       // %bb.0:
; VBITS_GE_512-NEXT:    ptrue p0.d, vl8
; VBITS_GE_512-NEXT:    ld1d { z0.d }, p0/z, [x0]
; VBITS_GE_512-NEXT:    revb z0.d, p0/m, z0.d
; VBITS_GE_512-NEXT:    st1d { z0.d }, p0, [x0]
; VBITS_GE_512-NEXT:    ret
  %op = load <8 x i64>, ptr %a
  %res = call <8 x i64> @llvm.bswap.v8i64(<8 x i64> %op)
  store <8 x i64> %res, ptr %a
  ret void
}

define void @bswap_v16i64(ptr %a) vscale_range(8,0) #0 {
; CHECK-LABEL: bswap_v16i64:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d, vl16
; CHECK-NEXT:    ld1d { z0.d }, p0/z, [x0]
; CHECK-NEXT:    revb z0.d, p0/m, z0.d
; CHECK-NEXT:    st1d { z0.d }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <16 x i64>, ptr %a
  %res = call <16 x i64> @llvm.bswap.v16i64(<16 x i64> %op)
  store <16 x i64> %res, ptr %a
  ret void
}

define void @bswap_v32i64(ptr %a) vscale_range(16,0) #0 {
; CHECK-LABEL: bswap_v32i64:
; CHECK:       // %bb.0:
; CHECK-NEXT:    ptrue p0.d, vl32
; CHECK-NEXT:    ld1d { z0.d }, p0/z, [x0]
; CHECK-NEXT:    revb z0.d, p0/m, z0.d
; CHECK-NEXT:    st1d { z0.d }, p0, [x0]
; CHECK-NEXT:    ret
  %op = load <32 x i64>, ptr %a
  %res = call <32 x i64> @llvm.bswap.v32i64(<32 x i64> %op)
  store <32 x i64> %res, ptr %a
  ret void
}

attributes #0 = { "target-features"="+sve" }

declare <8 x i8> @llvm.bitreverse.v8i8(<8 x i8>)
declare <16 x i8> @llvm.bitreverse.v16i8(<16 x i8>)
declare <32 x i8> @llvm.bitreverse.v32i8(<32 x i8>)
declare <64 x i8> @llvm.bitreverse.v64i8(<64 x i8>)
declare <128 x i8> @llvm.bitreverse.v128i8(<128 x i8>)
declare <256 x i8> @llvm.bitreverse.v256i8(<256 x i8>)
declare <4 x i16> @llvm.bitreverse.v4i16(<4 x i16>)
declare <8 x i16> @llvm.bitreverse.v8i16(<8 x i16>)
declare <16 x i16> @llvm.bitreverse.v16i16(<16 x i16>)
declare <32 x i16> @llvm.bitreverse.v32i16(<32 x i16>)
declare <64 x i16> @llvm.bitreverse.v64i16(<64 x i16>)
declare <128 x i16> @llvm.bitreverse.v128i16(<128 x i16>)
declare <2 x i32> @llvm.bitreverse.v2i32(<2 x i32>)
declare <4 x i32> @llvm.bitreverse.v4i32(<4 x i32>)
declare <8 x i32> @llvm.bitreverse.v8i32(<8 x i32>)
declare <16 x i32> @llvm.bitreverse.v16i32(<16 x i32>)
declare <32 x i32> @llvm.bitreverse.v32i32(<32 x i32>)
declare <64 x i32> @llvm.bitreverse.v64i32(<64 x i32>)
declare <1 x i64> @llvm.bitreverse.v1i64(<1 x i64>)
declare <2 x i64> @llvm.bitreverse.v2i64(<2 x i64>)
declare <4 x i64> @llvm.bitreverse.v4i64(<4 x i64>)
declare <8 x i64> @llvm.bitreverse.v8i64(<8 x i64>)
declare <16 x i64> @llvm.bitreverse.v16i64(<16 x i64>)
declare <32 x i64> @llvm.bitreverse.v32i64(<32 x i64>)

declare <4 x i16> @llvm.bswap.v4i16(<4 x i16>)
declare <8 x i16> @llvm.bswap.v8i16(<8 x i16>)
declare <16 x i16> @llvm.bswap.v16i16(<16 x i16>)
declare <32 x i16> @llvm.bswap.v32i16(<32 x i16>)
declare <64 x i16> @llvm.bswap.v64i16(<64 x i16>)
declare <128 x i16> @llvm.bswap.v128i16(<128 x i16>)
declare <2 x i32> @llvm.bswap.v2i32(<2 x i32>)
declare <4 x i32> @llvm.bswap.v4i32(<4 x i32>)
declare <8 x i32> @llvm.bswap.v8i32(<8 x i32>)
declare <16 x i32> @llvm.bswap.v16i32(<16 x i32>)
declare <32 x i32> @llvm.bswap.v32i32(<32 x i32>)
declare <64 x i32> @llvm.bswap.v64i32(<64 x i32>)
declare <1 x i64> @llvm.bswap.v1i64(<1 x i64>)
declare <2 x i64> @llvm.bswap.v2i64(<2 x i64>)
declare <4 x i64> @llvm.bswap.v4i64(<4 x i64>)
declare <8 x i64> @llvm.bswap.v8i64(<8 x i64>)
declare <16 x i64> @llvm.bswap.v16i64(<16 x i64>)
declare <32 x i64> @llvm.bswap.v32i64(<32 x i64>)