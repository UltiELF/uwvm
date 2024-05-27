; NOTE: Assertions have been autogenerated by utils/update_mir_test_checks.py
; RUN: llc -mtriple=aarch64-elf -global-isel -stop-after=irtranslator -verify-machineinstrs -o - %s | FileCheck %s

@foo_alias = alias ptr, ptr @callee

define internal ptr @callee() {
  ; CHECK-LABEL: name: callee
  ; CHECK: bb.1.entry:
  ; CHECK-NEXT:   [[C:%[0-9]+]]:_(p0) = G_CONSTANT i64 0
  ; CHECK-NEXT:   $x0 = COPY [[C]](p0)
  ; CHECK-NEXT:   RET_ReallyLR implicit $x0
entry:
  ret ptr null
}

define void @caller() {
  ; CHECK-LABEL: name: caller
  ; CHECK: bb.1.entry:
  ; CHECK-NEXT:   ADJCALLSTACKDOWN 0, 0, implicit-def $sp, implicit $sp
  ; CHECK-NEXT:   BL @foo_alias, csr_aarch64_aapcs, implicit-def $lr, implicit $sp, implicit-def $w0
  ; CHECK-NEXT:   ADJCALLSTACKUP 0, 0, implicit-def $sp, implicit $sp
  ; CHECK-NEXT:   [[COPY:%[0-9]+]]:_(s32) = COPY $w0
  ; CHECK-NEXT:   RET_ReallyLR
entry:
  %0 = call i32 @foo_alias()
  ret void
}