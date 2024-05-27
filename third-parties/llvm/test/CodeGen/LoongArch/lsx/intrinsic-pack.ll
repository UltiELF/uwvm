; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc --mtriple=loongarch64 --mattr=+lsx < %s | FileCheck %s

declare <16 x i8> @llvm.loongarch.lsx.vpackev.b(<16 x i8>, <16 x i8>)

define <16 x i8> @lsx_vpackev_b(<16 x i8> %va, <16 x i8> %vb) nounwind {
; CHECK-LABEL: lsx_vpackev_b:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    vpackev.b $vr0, $vr0, $vr1
; CHECK-NEXT:    ret
entry:
  %res = call <16 x i8> @llvm.loongarch.lsx.vpackev.b(<16 x i8> %va, <16 x i8> %vb)
  ret <16 x i8> %res
}

declare <8 x i16> @llvm.loongarch.lsx.vpackev.h(<8 x i16>, <8 x i16>)

define <8 x i16> @lsx_vpackev_h(<8 x i16> %va, <8 x i16> %vb) nounwind {
; CHECK-LABEL: lsx_vpackev_h:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    vpackev.h $vr0, $vr0, $vr1
; CHECK-NEXT:    ret
entry:
  %res = call <8 x i16> @llvm.loongarch.lsx.vpackev.h(<8 x i16> %va, <8 x i16> %vb)
  ret <8 x i16> %res
}

declare <4 x i32> @llvm.loongarch.lsx.vpackev.w(<4 x i32>, <4 x i32>)

define <4 x i32> @lsx_vpackev_w(<4 x i32> %va, <4 x i32> %vb) nounwind {
; CHECK-LABEL: lsx_vpackev_w:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    vpackev.w $vr0, $vr0, $vr1
; CHECK-NEXT:    ret
entry:
  %res = call <4 x i32> @llvm.loongarch.lsx.vpackev.w(<4 x i32> %va, <4 x i32> %vb)
  ret <4 x i32> %res
}

declare <2 x i64> @llvm.loongarch.lsx.vpackev.d(<2 x i64>, <2 x i64>)

define <2 x i64> @lsx_vpackev_d(<2 x i64> %va, <2 x i64> %vb) nounwind {
; CHECK-LABEL: lsx_vpackev_d:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    vpackev.d $vr0, $vr0, $vr1
; CHECK-NEXT:    ret
entry:
  %res = call <2 x i64> @llvm.loongarch.lsx.vpackev.d(<2 x i64> %va, <2 x i64> %vb)
  ret <2 x i64> %res
}

declare <16 x i8> @llvm.loongarch.lsx.vpackod.b(<16 x i8>, <16 x i8>)

define <16 x i8> @lsx_vpackod_b(<16 x i8> %va, <16 x i8> %vb) nounwind {
; CHECK-LABEL: lsx_vpackod_b:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    vpackod.b $vr0, $vr0, $vr1
; CHECK-NEXT:    ret
entry:
  %res = call <16 x i8> @llvm.loongarch.lsx.vpackod.b(<16 x i8> %va, <16 x i8> %vb)
  ret <16 x i8> %res
}

declare <8 x i16> @llvm.loongarch.lsx.vpackod.h(<8 x i16>, <8 x i16>)

define <8 x i16> @lsx_vpackod_h(<8 x i16> %va, <8 x i16> %vb) nounwind {
; CHECK-LABEL: lsx_vpackod_h:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    vpackod.h $vr0, $vr0, $vr1
; CHECK-NEXT:    ret
entry:
  %res = call <8 x i16> @llvm.loongarch.lsx.vpackod.h(<8 x i16> %va, <8 x i16> %vb)
  ret <8 x i16> %res
}

declare <4 x i32> @llvm.loongarch.lsx.vpackod.w(<4 x i32>, <4 x i32>)

define <4 x i32> @lsx_vpackod_w(<4 x i32> %va, <4 x i32> %vb) nounwind {
; CHECK-LABEL: lsx_vpackod_w:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    vpackod.w $vr0, $vr0, $vr1
; CHECK-NEXT:    ret
entry:
  %res = call <4 x i32> @llvm.loongarch.lsx.vpackod.w(<4 x i32> %va, <4 x i32> %vb)
  ret <4 x i32> %res
}

declare <2 x i64> @llvm.loongarch.lsx.vpackod.d(<2 x i64>, <2 x i64>)

define <2 x i64> @lsx_vpackod_d(<2 x i64> %va, <2 x i64> %vb) nounwind {
; CHECK-LABEL: lsx_vpackod_d:
; CHECK:       # %bb.0: # %entry
; CHECK-NEXT:    vpackod.d $vr0, $vr0, $vr1
; CHECK-NEXT:    ret
entry:
  %res = call <2 x i64> @llvm.loongarch.lsx.vpackod.d(<2 x i64> %va, <2 x i64> %vb)
  ret <2 x i64> %res
}