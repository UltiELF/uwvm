; NOTE: Assertions have been autogenerated by utils/update_test_checks.py
; RUN: opt < %s -passes=instsimplify -S | FileCheck %s

declare void @llvm.assume(i1)
define <2 x i1> @select_v_ne_z(<2 x i8> %v, <2 x i8> %yy) {
; CHECK-LABEL: @select_v_ne_z(
; CHECK-NEXT:    ret <2 x i1> zeroinitializer
;
  %y = or <2 x i8> %yy, <i8 1, i8 1>
  %cmp = icmp ne <2 x i8> %v, zeroinitializer
  %s = select <2 x i1> %cmp, <2 x i8> %v, <2 x i8> %y
  %r = icmp eq <2 x i8> %s, zeroinitializer
  ret <2 x i1> %r
}

define i1 @select_v_ne_fail(i8 %v, i8 %y) {
; CHECK-LABEL: @select_v_ne_fail(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    [[CMP:%.*]] = icmp ne i8 [[V:%.*]], 1
; CHECK-NEXT:    [[S:%.*]] = select i1 [[CMP]], i8 [[V]], i8 [[Y]]
; CHECK-NEXT:    [[R:%.*]] = icmp eq i8 [[S]], 0
; CHECK-NEXT:    ret i1 [[R]]
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp ne i8 %v, 1
  %s = select i1 %cmp, i8 %v, i8 %y
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @select_v_eq_nz(i8 %v, i8 %y) {
; CHECK-LABEL: @select_v_eq_nz(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    ret i1 false
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp eq i8 44, %v
  %s = select i1 %cmp, i8 %v, i8 %y
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @inv_select_v_ugt_nz(i8 %v, i8 %C, i8 %y) {
; CHECK-LABEL: @inv_select_v_ugt_nz(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    ret i1 false
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp ugt i8 14, %v
  %s = select i1 %cmp, i8 %y, i8 %v
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @inv_select_v_ugt_fail(i8 %v, i8 %C, i8 %y) {
; CHECK-LABEL: @inv_select_v_ugt_fail(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    ret i1 false
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp ugt i8 -2, %v
  %s = select i1 %cmp, i8 %y, i8 %v
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define <2 x i1> @select_v_ult(<2 x i8> %v, <2 x i8> %C, <2 x i8> %yy) {
; CHECK-LABEL: @select_v_ult(
; CHECK-NEXT:    ret <2 x i1> zeroinitializer
;
  %y = or <2 x i8> %yy, <i8 1, i8 1>
  %cmp = icmp ult <2 x i8> %C, %v
  %s = select <2 x i1> %cmp, <2 x i8> %v, <2 x i8> %y
  %r = icmp eq <2 x i8> %s, zeroinitializer
  ret <2 x i1> %r
}

define <2 x i1> @select_v_uge_nz(<2 x i8> %v, <2 x i8> %yy) {
; CHECK-LABEL: @select_v_uge_nz(
; CHECK-NEXT:    ret <2 x i1> zeroinitializer
;
  %y = or <2 x i8> %yy, <i8 1, i8 1>
  %cmp = icmp uge <2 x i8> %v, <i8 1, i8 1>
  %s = select <2 x i1> %cmp, <2 x i8> %v, <2 x i8> %y
  %r = icmp eq <2 x i8> %s, zeroinitializer
  ret <2 x i1> %r
}

define i1 @inv_select_v_ule(i8 %v, i8 %y) {
; CHECK-LABEL: @inv_select_v_ule(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    ret i1 false
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp ule i8 %v, 4
  %s = select i1 %cmp, i8 %y, i8 %v
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @select_v_sgt_nonneg(i8 %v, i8 %y) {
; CHECK-LABEL: @select_v_sgt_nonneg(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    ret i1 false
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp sgt i8 %v, 0
  %s = select i1 %cmp, i8 %v, i8 %y
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @select_v_sgt_fail(i8 %v, i8 %y) {
; CHECK-LABEL: @select_v_sgt_fail(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    [[CMP:%.*]] = icmp sgt i8 [[V:%.*]], -1
; CHECK-NEXT:    [[S:%.*]] = select i1 [[CMP]], i8 [[V]], i8 [[Y]]
; CHECK-NEXT:    [[R:%.*]] = icmp eq i8 [[S]], 0
; CHECK-NEXT:    ret i1 [[R]]
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp sgt i8 %v, -1
  %s = select i1 %cmp, i8 %v, i8 %y
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @inv_select_v_sgt_neg(i8 %v, i8 %y) {
; CHECK-LABEL: @inv_select_v_sgt_neg(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    ret i1 false
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp sgt i8 %v, -1
  %s = select i1 %cmp, i8 %y, i8 %v
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @inv_select_v_sgt_nonneg_nz(i8 %v, i8 %C, i8 %y) {
; CHECK-LABEL: @inv_select_v_sgt_nonneg_nz(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    ret i1 false
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp sgt i8 99, %v
  %s = select i1 %cmp, i8 %y, i8 %v
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @select_v_slt_nonneg(i8 %v, i8 %y) {
; CHECK-LABEL: @select_v_slt_nonneg(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    ret i1 false
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp slt i8 0, %v
  %s = select i1 %cmp, i8 %v, i8 %y
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @select_v_slt_nonneg_fail(i8 %v, i8 %C, i8 %y) {
; CHECK-LABEL: @select_v_slt_nonneg_fail(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    [[CMP:%.*]] = icmp slt i8 -1, [[V:%.*]]
; CHECK-NEXT:    [[S:%.*]] = select i1 [[CMP]], i8 [[V]], i8 [[Y]]
; CHECK-NEXT:    [[R:%.*]] = icmp eq i8 [[S]], 0
; CHECK-NEXT:    ret i1 [[R]]
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp slt i8 -1, %v
  %s = select i1 %cmp, i8 %v, i8 %y
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @select_v_slt_neg(i8 %v, i8 %y) {
; CHECK-LABEL: @select_v_slt_neg(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    ret i1 false
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp slt i8 %v, -1
  %s = select i1 %cmp, i8 %v, i8 %y
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @select_v_sge_nonneg_nz(i8 %v, i8 %y) {
; CHECK-LABEL: @select_v_sge_nonneg_nz(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    ret i1 false
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp sge i8 %v, 1
  %s = select i1 %cmp, i8 %v, i8 %y
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @select_v_sge_neg(i8 %v, i8 %y) {
; CHECK-LABEL: @select_v_sge_neg(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    ret i1 false
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp sge i8 -3, %v
  %s = select i1 %cmp, i8 %v, i8 %y
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @inv_select_v_sge_z(i8 %v, i8 %y) {
; CHECK-LABEL: @inv_select_v_sge_z(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    ret i1 false
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp sge i8 %v, 0
  %s = select i1 %cmp, i8 %y, i8 %v
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @inv_select_v_sge_fail(i8 %v, i8 %C, i8 %y) {
; CHECK-LABEL: @inv_select_v_sge_fail(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    ret i1 false
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp sge i8 %v, -44
  %s = select i1 %cmp, i8 %y, i8 %v
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @select_v_sle_neg(i8 %v, i8 %y) {
; CHECK-LABEL: @select_v_sle_neg(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    ret i1 false
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp sle i8 %v, -1
  %s = select i1 %cmp, i8 %v, i8 %y
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @select_v_sle_nonneg_nz(i8 %v, i8 %y) {
; CHECK-LABEL: @select_v_sle_nonneg_nz(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    ret i1 false
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp sle i8 1, %v
  %s = select i1 %cmp, i8 %v, i8 %y
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @select_v_sle_fail(i8 %v, i8 %y) {
; CHECK-LABEL: @select_v_sle_fail(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    [[CMP:%.*]] = icmp sle i8 0, [[V:%.*]]
; CHECK-NEXT:    [[S:%.*]] = select i1 [[CMP]], i8 [[V]], i8 [[Y]]
; CHECK-NEXT:    [[R:%.*]] = icmp eq i8 [[S]], 0
; CHECK-NEXT:    ret i1 [[R]]
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp sle i8 0, %v
  %s = select i1 %cmp, i8 %v, i8 %y
  %r = icmp eq i8 %s, 0
  ret i1 %r
}

define i1 @inv_select_v_sle_nonneg(i8 %v, i8 %y) {
; CHECK-LABEL: @inv_select_v_sle_nonneg(
; CHECK-NEXT:    [[YNZ:%.*]] = icmp ne i8 [[Y:%.*]], 0
; CHECK-NEXT:    call void @llvm.assume(i1 [[YNZ]])
; CHECK-NEXT:    ret i1 false
;
  %ynz = icmp ne i8 %y, 0
  call void @llvm.assume(i1 %ynz)
  %cmp = icmp sle i8 %v, 0
  %s = select i1 %cmp, i8 %y, i8 %v
  %r = icmp eq i8 %s, 0
  ret i1 %r
}