; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py UTC_ARGS: --version 2
; RUN: llc < %s -verify-machineinstrs -mtriple=x86_64-unknown-unknown --show-mc-encoding -mattr=+usermsr | FileCheck %s --check-prefixes=X64

define i64 @test_int_x86_urdmsr(i64 %A) nounwind {
; X64-LABEL: test_int_x86_urdmsr:
; X64:       # %bb.0:
; X64-NEXT:    urdmsr %rdi, %rax # encoding: [0xf2,0x0f,0x38,0xf8,0xc7]
; X64-NEXT:    retq # encoding: [0xc3]
  %ret = call i64 @llvm.x86.urdmsr(i64 %A)
  ret i64 %ret
}

define i64 @test_int_x86_urdmsr_const() nounwind {
; X64-LABEL: test_int_x86_urdmsr_const:
; X64:       # %bb.0:
; X64-NEXT:    urdmsr $123, %rax # encoding: [0xc4,0xe7,0x7b,0xf8,0xc0,0x7b,0x00,0x00,0x00]
; X64-NEXT:    retq # encoding: [0xc3]
  %ret = call i64 @llvm.x86.urdmsr(i64 123)
  ret i64 %ret
}

define i64 @test_int_x86_urdmsr_const_i64() nounwind {
; X64-LABEL: test_int_x86_urdmsr_const_i64:
; X64:       # %bb.0:
; X64-NEXT:    movabsq $8589934591, %rax # encoding: [0x48,0xb8,0xff,0xff,0xff,0xff,0x01,0x00,0x00,0x00]
; X64-NEXT:    # imm = 0x1FFFFFFFF
; X64-NEXT:    urdmsr %rax, %rax # encoding: [0xf2,0x0f,0x38,0xf8,0xc0]
; X64-NEXT:    retq # encoding: [0xc3]
  %ret = call i64 @llvm.x86.urdmsr(i64 8589934591)
  ret i64 %ret
}

declare i64 @llvm.x86.urdmsr(i64 %A)

define void @test_int_x86_uwrmsr(i64 %A, i64 %B) nounwind {
; X64-LABEL: test_int_x86_uwrmsr:
; X64:       # %bb.0:
; X64-NEXT:    uwrmsr %rsi, %rdi # encoding: [0xf3,0x0f,0x38,0xf8,0xfe]
; X64-NEXT:    retq # encoding: [0xc3]
  call void @llvm.x86.uwrmsr(i64 %A, i64 %B)
  ret void
}

define void @test_int_x86_uwrmsr_const(i64 %A) nounwind {
; X64-LABEL: test_int_x86_uwrmsr_const:
; X64:       # %bb.0:
; X64-NEXT:    uwrmsr %rdi, $123 # encoding: [0xc4,0xe7,0x7a,0xf8,0xc7,0x7b,0x00,0x00,0x00]
; X64-NEXT:    retq # encoding: [0xc3]
  call void @llvm.x86.uwrmsr(i64 123, i64 %A)
  ret void
}

define void @test_int_x86_uwrmsr_const_i64(i64 %A) nounwind {
; X64-LABEL: test_int_x86_uwrmsr_const_i64:
; X64:       # %bb.0:
; X64-NEXT:    movabsq $8589934591, %rax # encoding: [0x48,0xb8,0xff,0xff,0xff,0xff,0x01,0x00,0x00,0x00]
; X64-NEXT:    # imm = 0x1FFFFFFFF
; X64-NEXT:    uwrmsr %rdi, %rax # encoding: [0xf3,0x0f,0x38,0xf8,0xc7]
; X64-NEXT:    retq # encoding: [0xc3]
  call void @llvm.x86.uwrmsr(i64 8589934591, i64 %A)
  ret void
}

declare void @llvm.x86.uwrmsr(i64 %A, i64 %B)