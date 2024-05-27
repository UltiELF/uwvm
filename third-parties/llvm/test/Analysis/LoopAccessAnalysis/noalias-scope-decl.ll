; NOTE: Assertions have been autogenerated by utils/update_analyze_test_checks.py UTC_ARGS: --version 4
; RUN: opt -passes='print<access-info>' -disable-output 2>&1 < %s | FileCheck %s

; PR79137: If the noalias.scope.decl is located inside the loop, we cannot
; assume that the accesses don't alias across iterations.

define void @test_scope_in_loop(ptr %arg, i64 %num) {
; CHECK-LABEL: 'test_scope_in_loop'
; CHECK-NEXT:    loop:
; CHECK-NEXT:      Report: unsafe dependent memory operations in loop. Use #pragma clang loop distribute(enable) to allow loop distribution to attempt to isolate the offending operations into a separate loop
; CHECK-NEXT:  Backward loop carried data dependence.
; CHECK-NEXT:      Dependences:
; CHECK-NEXT:        Backward:
; CHECK-NEXT:            %load.prev = load i8, ptr %prev.ptr, align 1, !alias.scope !0, !noalias !3 ->
; CHECK-NEXT:            store i8 %add, ptr %cur.ptr, align 1, !alias.scope !3
; CHECK-EMPTY:
; CHECK-NEXT:        Forward:
; CHECK-NEXT:            %load.cur = load i8, ptr %cur.ptr, align 1, !alias.scope !3 ->
; CHECK-NEXT:            store i8 %add, ptr %cur.ptr, align 1, !alias.scope !3
; CHECK-EMPTY:
; CHECK-NEXT:      Run-time memory checks:
; CHECK-NEXT:      Grouped accesses:
; CHECK-EMPTY:
; CHECK-NEXT:      Non vectorizable stores to invariant address were not found in loop.
; CHECK-NEXT:      SCEV assumptions:
; CHECK-EMPTY:
; CHECK-NEXT:      Expressions re-written:
;
entry:
  %icmp = icmp ult i64 %num, 2
  br i1 %icmp, label %exit, label %preheader

preheader:
  %arg.1 = getelementptr inbounds i8, ptr %arg, i64 1
  %end = add i64 %num, -2
  br label %loop

loop:
  %prev.ptr = phi ptr [ %cur.ptr, %loop ], [ %arg, %preheader ]
  %iv = phi i64 [ %iv.next, %loop ], [ 0, %preheader ]
  %cur.ptr = getelementptr inbounds i8, ptr %arg.1, i64 %iv
  call void @llvm.experimental.noalias.scope.decl(metadata !0)
  call void @llvm.experimental.noalias.scope.decl(metadata !3)
  %load.prev = load i8, ptr %prev.ptr, align 1, !alias.scope !0, !noalias !3
  %load.cur = load i8, ptr %cur.ptr, align 1, !alias.scope !3
  %add = add i8 %load.cur, %load.prev
  store i8 %add, ptr %cur.ptr, align 1, !alias.scope !3
  %iv.next = add nuw i64 %iv, 1
  %cmp = icmp eq i64 %iv, %end
  br i1 %cmp, label %exit, label %loop

exit:
  ret void
}

define void @test_scope_out_of_loop(ptr %arg, i64 %num) {
; CHECK-LABEL: 'test_scope_out_of_loop'
; CHECK-NEXT:    loop:
; CHECK-NEXT:      Memory dependences are safe
; CHECK-NEXT:      Dependences:
; CHECK-NEXT:      Run-time memory checks:
; CHECK-NEXT:      Grouped accesses:
; CHECK-EMPTY:
; CHECK-NEXT:      Non vectorizable stores to invariant address were not found in loop.
; CHECK-NEXT:      SCEV assumptions:
; CHECK-EMPTY:
; CHECK-NEXT:      Expressions re-written:
;
entry:
  %icmp = icmp ult i64 %num, 2
  br i1 %icmp, label %exit, label %preheader

preheader:
  call void @llvm.experimental.noalias.scope.decl(metadata !0)
  call void @llvm.experimental.noalias.scope.decl(metadata !3)
  %arg.1 = getelementptr inbounds i8, ptr %arg, i64 1
  %end = add i64 %num, -2
  br label %loop

loop:
  %prev.ptr = phi ptr [ %cur.ptr, %loop ], [ %arg, %preheader ]
  %iv = phi i64 [ %iv.next, %loop ], [ 0, %preheader ]
  %cur.ptr = getelementptr inbounds i8, ptr %arg.1, i64 %iv
  %load.prev = load i8, ptr %prev.ptr, align 1, !alias.scope !0, !noalias !3
  %load.cur = load i8, ptr %cur.ptr, align 1, !alias.scope !3
  %add = add i8 %load.cur, %load.prev
  store i8 %add, ptr %cur.ptr, align 1, !alias.scope !3
  %iv.next = add nuw i64 %iv, 1
  %cmp = icmp eq i64 %iv, %end
  br i1 %cmp, label %exit, label %loop

exit:
  ret void
}

declare void @llvm.experimental.noalias.scope.decl(metadata)

!0 = !{!1}
!1 = distinct !{!1, !2}
!2 = distinct !{!2}
!3 = !{!4}
!4 = distinct !{!4, !5}
!5 = distinct !{!5}