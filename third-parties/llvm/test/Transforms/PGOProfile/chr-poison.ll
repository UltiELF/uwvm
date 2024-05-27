; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt < %s -passes='require<profile-summary>,function(chr)' -S | FileCheck %s

; Make sure we freeze poison.
define void @chr_poison(i1 %arg) !prof !15 {
; CHECK-LABEL: @chr_poison(
; CHECK-NEXT:    [[TMP1:%.*]] = freeze i1 poison
; CHECK-NEXT:    [[TMP2:%.*]] = select i1 true, i1 [[TMP1]], i1 false
; CHECK-NEXT:    [[TMP3:%.*]] = freeze i1 [[ARG:%.*]]
; CHECK-NEXT:    [[TMP4:%.*]] = select i1 [[TMP2]], i1 [[TMP3]], i1 false
; CHECK-NEXT:    br i1 [[TMP4]], label [[DOTSPLIT:%.*]], label [[DOTSPLIT_NONCHR:%.*]], !prof [[PROF30:![0-9]+]]
; CHECK:       .split:
; CHECK-NEXT:    br i1 false, label [[BB3:%.*]], label [[BB4:%.*]]
; CHECK:       bb3:
; CHECK-NEXT:    [[SEL:%.*]] = select i1 false, ptr null, ptr null
; CHECK-NEXT:    br label [[BB6:%.*]]
; CHECK:       bb4:
; CHECK-NEXT:    br i1 true, label [[BB6]], label [[BB5:%.*]], !prof [[PROF31:![0-9]+]]
; CHECK:       bb5:
; CHECK-NEXT:    br label [[BB6]]
; CHECK:       bb6:
; CHECK-NEXT:    br i1 false, label [[BB9:%.*]], label [[BB7:%.*]]
; CHECK:       bb7:
; CHECK-NEXT:    br i1 true, label [[BB9]], label [[BB8:%.*]], !prof [[PROF32:![0-9]+]]
; CHECK:       bb8:
; CHECK-NEXT:    br label [[BB9]]
; CHECK:       .split.nonchr:
; CHECK-NEXT:    br i1 false, label [[BB3_NONCHR:%.*]], label [[BB4_NONCHR:%.*]]
; CHECK:       bb3.nonchr:
; CHECK-NEXT:    [[SEL_NONCHR:%.*]] = select i1 false, ptr null, ptr null
; CHECK-NEXT:    br label [[BB6_NONCHR:%.*]]
; CHECK:       bb4.nonchr:
; CHECK-NEXT:    br i1 poison, label [[BB6_NONCHR]], label [[BB5_NONCHR:%.*]], !prof [[PROF31]]
; CHECK:       bb5.nonchr:
; CHECK-NEXT:    br label [[BB6_NONCHR]]
; CHECK:       bb6.nonchr:
; CHECK-NEXT:    br i1 false, label [[BB9]], label [[BB7_NONCHR:%.*]]
; CHECK:       bb7.nonchr:
; CHECK-NEXT:    br i1 [[ARG]], label [[BB9]], label [[BB8_NONCHR:%.*]], !prof [[PROF32]]
; CHECK:       bb8.nonchr:
; CHECK-NEXT:    br label [[BB9]]
; CHECK:       bb9:
; CHECK-NEXT:    ret void
;
  br i1 false, label %bb3, label %bb4

bb3:
  %sel = select i1 false, ptr null, ptr null
  br label %bb6

bb4:
  br i1 poison, label %bb6, label %bb5, !prof !16

bb5:
  br label %bb6

bb6:
  br i1 false, label %bb9, label %bb7

bb7:
  br i1 %arg, label %bb9, label %bb8, !prof !17

bb8:
  br label %bb9

bb9:
  ret void
}

!llvm.module.flags = !{!0}

!0 = !{i32 1, !"ProfileSummary", !1}
!1 = !{!2, !3, !4, !5, !6, !7, !8, !9, !10, !11}
!2 = !{!"ProfileFormat", !"SampleProfile"}
!3 = !{!"TotalCount", i64 2625732223}
!4 = !{!"MaxCount", i64 6099111}
!5 = !{!"MaxInternalCount", i64 0}
!6 = !{!"MaxFunctionCount", i64 1000000}
!7 = !{!"NumCounts", i64 1000000}
!8 = !{!"NumFunctions", i64 14940}
!9 = !{!"IsPartialProfile", i64 0}
!10 = !{!"PartialProfileRatio", double 0.000000e+00}
!11 = !{!"DetailedSummary", !12}
!12 = !{!13, !14}
!13 = !{i32 100000, i64 6083920, i32 44}
!14 = !{i32 999999, i64 1, i32 1000000}
!15 = !{!"function_entry_count", i64 1000}
!16 = !{!"branch_weights", i32 1, i32 0}
!17 = !{!"branch_weights", i32 1, i32 0}