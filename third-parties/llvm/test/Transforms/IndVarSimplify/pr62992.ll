; NOTE: Assertions have been autogenerated by utils/update_test_checks.py UTC_ARGS: --version 2
; RUN: opt -S -passes=indvars < %s | FileCheck %s

declare void @use(i1)

; Make sure the division does not get expanded into the preheader.

define i32 @test(i32 %arg) {
; CHECK-LABEL: define i32 @test
; CHECK-SAME: (i32 [[ARG:%.*]]) {
; CHECK-NEXT:  entry:
; CHECK-NEXT:    br label [[LOOP:%.*]]
; CHECK:       loop:
; CHECK-NEXT:    br i1 false, label [[IF:%.*]], label [[LOOP_LATCH:%.*]]
; CHECK:       if:
; CHECK-NEXT:    [[DIV:%.*]] = udiv i32 7, [[ARG]]
; CHECK-NEXT:    [[CMP2:%.*]] = icmp ult i32 1, [[DIV]]
; CHECK-NEXT:    call void @use(i1 [[CMP2]])
; CHECK-NEXT:    br label [[LOOP_LATCH]]
; CHECK:       loop.latch:
; CHECK-NEXT:    br i1 false, label [[LOOP]], label [[EXIT:%.*]]
; CHECK:       exit:
; CHECK-NEXT:    ret i32 1
;
entry:
  br label %loop

loop:
  %iv = phi i32 [ 0, %entry ], [ %iv.next, %loop.latch ]
  %iv.next = add i32 %iv, 1
  %cmp = icmp eq i32 %iv, 1
  br i1 %cmp, label %if, label %loop.latch

if:
  %div = udiv i32 7, %arg
  %cmp2 = icmp ult i32 %iv.next, %div
  call void @use(i1 %cmp2)
  br label %loop.latch

loop.latch:
  br i1 false, label %loop, label %exit

exit:
  %inc.lcssa = phi i32 [ %iv.next, %loop.latch ]
  ret i32 %inc.lcssa
}