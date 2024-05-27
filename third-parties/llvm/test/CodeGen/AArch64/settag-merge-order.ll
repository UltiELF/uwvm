; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py UTC_ARGS: --version 2
; RUN: llc < %s -mtriple=aarch64 -mattr=+mte -aarch64-order-frame-objects=1 | FileCheck %s

declare void @use(ptr %p)
declare void @llvm.aarch64.settag(ptr %p, i64 %a)
declare void @llvm.aarch64.settag.zero(ptr %p, i64 %a)

; Two loops of size 256; the second loop updates SP.
; After frame reordering, two loops can be merged into one.
define void @stg128_128_gap_128_128() {
; CHECK-LABEL: stg128_128_gap_128_128:
; CHECK:       // %bb.0: // %entry
; CHECK-NEXT:    stp x29, x30, [sp, #-16]! // 16-byte Folded Spill
; CHECK-NEXT:    sub sp, sp, #544
; CHECK-NEXT:    .cfi_def_cfa_offset 560
; CHECK-NEXT:    .cfi_offset w30, -8
; CHECK-NEXT:    .cfi_offset w29, -16
; CHECK-NEXT:    add x0, sp, #512
; CHECK-NEXT:    bl use
; CHECK-NEXT:    mov x8, #512 // =0x200
; CHECK-NEXT:  .LBB0_1: // %entry
; CHECK-NEXT:    // =>This Inner Loop Header: Depth=1
; CHECK-NEXT:    st2g sp, [sp], #32
; CHECK-NEXT:    subs x8, x8, #32
; CHECK-NEXT:    b.ne .LBB0_1
; CHECK-NEXT:  // %bb.2: // %entry
; CHECK-NEXT:    add sp, sp, #32
; CHECK-NEXT:    ldp x29, x30, [sp], #16 // 16-byte Folded Reload
; CHECK-NEXT:    ret
entry:
  %a = alloca i8, i32 128, align 16
  %a2 = alloca i8, i32 128, align 16
  %b = alloca i8, i32 32, align 16
  %c = alloca i8, i32 128, align 16
  %c2 = alloca i8, i32 128, align 16
  call void @use(ptr %b)
  call void @llvm.aarch64.settag(ptr %a, i64 128)
  call void @llvm.aarch64.settag(ptr %a2, i64 128)
  call void @llvm.aarch64.settag(ptr %c, i64 128)
  call void @llvm.aarch64.settag(ptr %c2, i64 128)
  ret void
}

define void @stg2(i1 %flag) {
; CHECK-LABEL: stg2:
; CHECK:       // %bb.0: // %entry
; CHECK-NEXT:    str x29, [sp, #-32]! // 8-byte Folded Spill
; CHECK-NEXT:    stp x30, x19, [sp, #16] // 16-byte Folded Spill
; CHECK-NEXT:    sub sp, sp, #608
; CHECK-NEXT:    .cfi_def_cfa_offset 640
; CHECK-NEXT:    .cfi_offset w19, -8
; CHECK-NEXT:    .cfi_offset w30, -16
; CHECK-NEXT:    .cfi_offset w29, -32
; CHECK-NEXT:    mov w19, w0
; CHECK-NEXT:    add x0, sp, #576
; CHECK-NEXT:    bl use
; CHECK-NEXT:    tbz w19, #0, .LBB1_4
; CHECK-NEXT:  // %bb.1: // %if.then
; CHECK-NEXT:    add x9, sp, #256
; CHECK-NEXT:    mov x8, #320 // =0x140
; CHECK-NEXT:  .LBB1_2: // %if.then
; CHECK-NEXT:    // =>This Inner Loop Header: Depth=1
; CHECK-NEXT:    st2g x9, [x9], #32
; CHECK-NEXT:    subs x8, x8, #32
; CHECK-NEXT:    b.ne .LBB1_2
; CHECK-NEXT:  // %bb.3: // %if.then
; CHECK-NEXT:    b .LBB1_7
; CHECK-NEXT:  .LBB1_4: // %if.else
; CHECK-NEXT:    mov x9, sp
; CHECK-NEXT:    mov x8, #256 // =0x100
; CHECK-NEXT:  .LBB1_5: // %if.else
; CHECK-NEXT:    // =>This Inner Loop Header: Depth=1
; CHECK-NEXT:    st2g x9, [x9], #32
; CHECK-NEXT:    subs x8, x8, #32
; CHECK-NEXT:    b.ne .LBB1_5
; CHECK-NEXT:  // %bb.6: // %if.else
; CHECK-NEXT:  .LBB1_7: // %if.end
; CHECK-NEXT:    mov x8, #576 // =0x240
; CHECK-NEXT:  .LBB1_8: // %if.end
; CHECK-NEXT:    // =>This Inner Loop Header: Depth=1
; CHECK-NEXT:    st2g sp, [sp], #32
; CHECK-NEXT:    subs x8, x8, #32
; CHECK-NEXT:    b.ne .LBB1_8
; CHECK-NEXT:  // %bb.9: // %if.end
; CHECK-NEXT:    add sp, sp, #32
; CHECK-NEXT:    ldp x30, x19, [sp, #16] // 16-byte Folded Reload
; CHECK-NEXT:    ldr x29, [sp], #32 // 8-byte Folded Reload
; CHECK-NEXT:    ret
entry:
  %a = alloca i8, i32 160, align 16
  %a2 = alloca i8, i32 160, align 16
  %b = alloca i8, i32 32, align 16
  %c = alloca i8, i32 128, align 16
  %c2 = alloca i8, i32 128, align 16
  call void @use(ptr %b)
  br i1 %flag, label %if.then, label %if.else

if.then:
  call void @llvm.aarch64.settag(ptr %a, i64 160)
  call void @llvm.aarch64.settag(ptr %a2, i64 160)
  br label %if.end

if.else:
  call void @llvm.aarch64.settag(ptr %c, i64 128)
  call void @llvm.aarch64.settag(ptr %c2, i64 128)
  br label %if.end

if.end:
  call void @llvm.aarch64.settag(ptr %a, i64 160)
  call void @llvm.aarch64.settag(ptr %a2, i64 160)
  call void @llvm.aarch64.settag(ptr %c, i64 128)
  call void @llvm.aarch64.settag(ptr %c2, i64 128)

  ret void
}