; NOTE: Assertions have been autogenerated by utils/update_mir_test_checks.py
; RUN: llc -mtriple=powerpc64le-unknown-linux-gnu -global-isel \
; RUN:     -verify-machineinstrs -stop-after=irtranslator < %s | FileCheck %s

; Pass up to twelve vector arguments in registers.
define void @test_vec1(<4 x i32> %a, <4 x i32> %b, <4 x i32> %c, <4 x i32> %d, <4 x i32> %e, <4 x i32> %f, <4 x i32> %g, <4 x i32> %h, <4 x i32> %i, <4 x i32> %j, <4 x i32> %k, <4 x i32> %l) {
  ; CHECK-LABEL: name: test_vec1
  ; CHECK: bb.1.entry:
  ; CHECK-NEXT:   liveins: $v2, $v3, $v4, $v5, $v6, $v7, $v8, $v9, $v10, $v11, $v12, $v13
  ; CHECK-NEXT: {{  $}}
  ; CHECK-NEXT:   [[COPY:%[0-9]+]]:_(<4 x s32>) = COPY $v2
  ; CHECK-NEXT:   [[COPY1:%[0-9]+]]:_(<4 x s32>) = COPY $v3
  ; CHECK-NEXT:   [[COPY2:%[0-9]+]]:_(<4 x s32>) = COPY $v4
  ; CHECK-NEXT:   [[COPY3:%[0-9]+]]:_(<4 x s32>) = COPY $v5
  ; CHECK-NEXT:   [[COPY4:%[0-9]+]]:_(<4 x s32>) = COPY $v6
  ; CHECK-NEXT:   [[COPY5:%[0-9]+]]:_(<4 x s32>) = COPY $v7
  ; CHECK-NEXT:   [[COPY6:%[0-9]+]]:_(<4 x s32>) = COPY $v8
  ; CHECK-NEXT:   [[COPY7:%[0-9]+]]:_(<4 x s32>) = COPY $v9
  ; CHECK-NEXT:   [[COPY8:%[0-9]+]]:_(<4 x s32>) = COPY $v10
  ; CHECK-NEXT:   [[COPY9:%[0-9]+]]:_(<4 x s32>) = COPY $v11
  ; CHECK-NEXT:   [[COPY10:%[0-9]+]]:_(<4 x s32>) = COPY $v12
  ; CHECK-NEXT:   [[COPY11:%[0-9]+]]:_(<4 x s32>) = COPY $v13
  ; CHECK-NEXT:   BLR8 implicit $lr8, implicit $rm
entry:
  ret void
}

define void @test_vec2(<2 x i64> %a, <2 x i64> %b, <2 x i64> %c, <2 x i64> %d, <2 x i64> %e, <2 x i64> %f, <2 x i64> %g, <2 x i64> %h, <2 x i64> %i, <2 x i64> %j, <2 x i64> %k, <2 x i64> %l) {
  ; CHECK-LABEL: name: test_vec2
  ; CHECK: bb.1.entry:
  ; CHECK-NEXT:   liveins: $v2, $v3, $v4, $v5, $v6, $v7, $v8, $v9, $v10, $v11, $v12, $v13
  ; CHECK-NEXT: {{  $}}
  ; CHECK-NEXT:   [[COPY:%[0-9]+]]:_(<2 x s64>) = COPY $v2
  ; CHECK-NEXT:   [[COPY1:%[0-9]+]]:_(<2 x s64>) = COPY $v3
  ; CHECK-NEXT:   [[COPY2:%[0-9]+]]:_(<2 x s64>) = COPY $v4
  ; CHECK-NEXT:   [[COPY3:%[0-9]+]]:_(<2 x s64>) = COPY $v5
  ; CHECK-NEXT:   [[COPY4:%[0-9]+]]:_(<2 x s64>) = COPY $v6
  ; CHECK-NEXT:   [[COPY5:%[0-9]+]]:_(<2 x s64>) = COPY $v7
  ; CHECK-NEXT:   [[COPY6:%[0-9]+]]:_(<2 x s64>) = COPY $v8
  ; CHECK-NEXT:   [[COPY7:%[0-9]+]]:_(<2 x s64>) = COPY $v9
  ; CHECK-NEXT:   [[COPY8:%[0-9]+]]:_(<2 x s64>) = COPY $v10
  ; CHECK-NEXT:   [[COPY9:%[0-9]+]]:_(<2 x s64>) = COPY $v11
  ; CHECK-NEXT:   [[COPY10:%[0-9]+]]:_(<2 x s64>) = COPY $v12
  ; CHECK-NEXT:   [[COPY11:%[0-9]+]]:_(<2 x s64>) = COPY $v13
  ; CHECK-NEXT:   BLR8 implicit $lr8, implicit $rm
entry:
  ret void
}

define void @test_vec3(<8 x i16> %a, <8 x i16> %b, <8 x i16> %c, <8 x i16> %d, <8 x i16> %e, <8 x i16> %f, <8 x i16> %g, <8 x i16> %h, <8 x i16> %i, <8 x i16> %j, <8 x i16> %k, <8 x i16> %l) {
  ; CHECK-LABEL: name: test_vec3
  ; CHECK: bb.1.entry:
  ; CHECK-NEXT:   liveins: $v2, $v3, $v4, $v5, $v6, $v7, $v8, $v9, $v10, $v11, $v12, $v13
  ; CHECK-NEXT: {{  $}}
  ; CHECK-NEXT:   [[COPY:%[0-9]+]]:_(<8 x s16>) = COPY $v2
  ; CHECK-NEXT:   [[COPY1:%[0-9]+]]:_(<8 x s16>) = COPY $v3
  ; CHECK-NEXT:   [[COPY2:%[0-9]+]]:_(<8 x s16>) = COPY $v4
  ; CHECK-NEXT:   [[COPY3:%[0-9]+]]:_(<8 x s16>) = COPY $v5
  ; CHECK-NEXT:   [[COPY4:%[0-9]+]]:_(<8 x s16>) = COPY $v6
  ; CHECK-NEXT:   [[COPY5:%[0-9]+]]:_(<8 x s16>) = COPY $v7
  ; CHECK-NEXT:   [[COPY6:%[0-9]+]]:_(<8 x s16>) = COPY $v8
  ; CHECK-NEXT:   [[COPY7:%[0-9]+]]:_(<8 x s16>) = COPY $v9
  ; CHECK-NEXT:   [[COPY8:%[0-9]+]]:_(<8 x s16>) = COPY $v10
  ; CHECK-NEXT:   [[COPY9:%[0-9]+]]:_(<8 x s16>) = COPY $v11
  ; CHECK-NEXT:   [[COPY10:%[0-9]+]]:_(<8 x s16>) = COPY $v12
  ; CHECK-NEXT:   [[COPY11:%[0-9]+]]:_(<8 x s16>) = COPY $v13
  ; CHECK-NEXT:   BLR8 implicit $lr8, implicit $rm
entry:
  ret void
}

define void @test_vec4(<16 x i8> %a, <16 x i8> %b, <16 x i8> %c, <16 x i8> %d, <16 x i8> %e, <16 x i8> %f, <16 x i8> %g, <16 x i8> %h, <16 x i8> %i, <16 x i8> %j, <16 x i8> %k, <16 x i8> %l) {
  ; CHECK-LABEL: name: test_vec4
  ; CHECK: bb.1.entry:
  ; CHECK-NEXT:   liveins: $v2, $v3, $v4, $v5, $v6, $v7, $v8, $v9, $v10, $v11, $v12, $v13
  ; CHECK-NEXT: {{  $}}
  ; CHECK-NEXT:   [[COPY:%[0-9]+]]:_(<16 x s8>) = COPY $v2
  ; CHECK-NEXT:   [[COPY1:%[0-9]+]]:_(<16 x s8>) = COPY $v3
  ; CHECK-NEXT:   [[COPY2:%[0-9]+]]:_(<16 x s8>) = COPY $v4
  ; CHECK-NEXT:   [[COPY3:%[0-9]+]]:_(<16 x s8>) = COPY $v5
  ; CHECK-NEXT:   [[COPY4:%[0-9]+]]:_(<16 x s8>) = COPY $v6
  ; CHECK-NEXT:   [[COPY5:%[0-9]+]]:_(<16 x s8>) = COPY $v7
  ; CHECK-NEXT:   [[COPY6:%[0-9]+]]:_(<16 x s8>) = COPY $v8
  ; CHECK-NEXT:   [[COPY7:%[0-9]+]]:_(<16 x s8>) = COPY $v9
  ; CHECK-NEXT:   [[COPY8:%[0-9]+]]:_(<16 x s8>) = COPY $v10
  ; CHECK-NEXT:   [[COPY9:%[0-9]+]]:_(<16 x s8>) = COPY $v11
  ; CHECK-NEXT:   [[COPY10:%[0-9]+]]:_(<16 x s8>) = COPY $v12
  ; CHECK-NEXT:   [[COPY11:%[0-9]+]]:_(<16 x s8>) = COPY $v13
  ; CHECK-NEXT:   BLR8 implicit $lr8, implicit $rm
entry:
  ret void
}

define void @test_vec5(<4 x float> %a, <4 x float> %b, <4 x float> %c, <4 x float> %d, <4 x float> %e, <4 x float> %f, <4 x float> %g, <4 x float> %h, <4 x float> %i, <4 x float> %j, <4 x float> %k, <4 x float> %l) {
  ; CHECK-LABEL: name: test_vec5
  ; CHECK: bb.1.entry:
  ; CHECK-NEXT:   liveins: $v2, $v3, $v4, $v5, $v6, $v7, $v8, $v9, $v10, $v11, $v12, $v13
  ; CHECK-NEXT: {{  $}}
  ; CHECK-NEXT:   [[COPY:%[0-9]+]]:_(<4 x s32>) = COPY $v2
  ; CHECK-NEXT:   [[COPY1:%[0-9]+]]:_(<4 x s32>) = COPY $v3
  ; CHECK-NEXT:   [[COPY2:%[0-9]+]]:_(<4 x s32>) = COPY $v4
  ; CHECK-NEXT:   [[COPY3:%[0-9]+]]:_(<4 x s32>) = COPY $v5
  ; CHECK-NEXT:   [[COPY4:%[0-9]+]]:_(<4 x s32>) = COPY $v6
  ; CHECK-NEXT:   [[COPY5:%[0-9]+]]:_(<4 x s32>) = COPY $v7
  ; CHECK-NEXT:   [[COPY6:%[0-9]+]]:_(<4 x s32>) = COPY $v8
  ; CHECK-NEXT:   [[COPY7:%[0-9]+]]:_(<4 x s32>) = COPY $v9
  ; CHECK-NEXT:   [[COPY8:%[0-9]+]]:_(<4 x s32>) = COPY $v10
  ; CHECK-NEXT:   [[COPY9:%[0-9]+]]:_(<4 x s32>) = COPY $v11
  ; CHECK-NEXT:   [[COPY10:%[0-9]+]]:_(<4 x s32>) = COPY $v12
  ; CHECK-NEXT:   [[COPY11:%[0-9]+]]:_(<4 x s32>) = COPY $v13
  ; CHECK-NEXT:   BLR8 implicit $lr8, implicit $rm
entry:
  ret void
}

define void @test_vec6(<2 x double> %a, <2 x double> %b, <2 x double> %c, <2 x double> %d, <2 x double> %e, <2 x double> %f, <2 x double> %g, <2 x double> %h, <2 x double> %i, <2 x double> %j, <2 x double> %k, <2 x double> %l) {
  ; CHECK-LABEL: name: test_vec6
  ; CHECK: bb.1.entry:
  ; CHECK-NEXT:   liveins: $v2, $v3, $v4, $v5, $v6, $v7, $v8, $v9, $v10, $v11, $v12, $v13
  ; CHECK-NEXT: {{  $}}
  ; CHECK-NEXT:   [[COPY:%[0-9]+]]:_(<2 x s64>) = COPY $v2
  ; CHECK-NEXT:   [[COPY1:%[0-9]+]]:_(<2 x s64>) = COPY $v3
  ; CHECK-NEXT:   [[COPY2:%[0-9]+]]:_(<2 x s64>) = COPY $v4
  ; CHECK-NEXT:   [[COPY3:%[0-9]+]]:_(<2 x s64>) = COPY $v5
  ; CHECK-NEXT:   [[COPY4:%[0-9]+]]:_(<2 x s64>) = COPY $v6
  ; CHECK-NEXT:   [[COPY5:%[0-9]+]]:_(<2 x s64>) = COPY $v7
  ; CHECK-NEXT:   [[COPY6:%[0-9]+]]:_(<2 x s64>) = COPY $v8
  ; CHECK-NEXT:   [[COPY7:%[0-9]+]]:_(<2 x s64>) = COPY $v9
  ; CHECK-NEXT:   [[COPY8:%[0-9]+]]:_(<2 x s64>) = COPY $v10
  ; CHECK-NEXT:   [[COPY9:%[0-9]+]]:_(<2 x s64>) = COPY $v11
  ; CHECK-NEXT:   [[COPY10:%[0-9]+]]:_(<2 x s64>) = COPY $v12
  ; CHECK-NEXT:   [[COPY11:%[0-9]+]]:_(<2 x s64>) = COPY $v13
  ; CHECK-NEXT:   BLR8 implicit $lr8, implicit $rm
entry:
  ret void
}

define void @test_vec7(<1 x i128> %a, <1 x i128> %b, <1 x i128> %c, <1 x i128> %d, <1 x i128> %e, <1 x i128> %f, <1 x i128> %g, <1 x i128> %h, <1 x i128> %i, <1 x i128> %j, <1 x i128> %k, <1 x i128> %l) {
  ; CHECK-LABEL: name: test_vec7
  ; CHECK: bb.1.entry:
  ; CHECK-NEXT:   liveins: $v2, $v3, $v4, $v5, $v6, $v7, $v8, $v9, $v10, $v11, $v12, $v13
  ; CHECK-NEXT: {{  $}}
  ; CHECK-NEXT:   [[COPY:%[0-9]+]]:_(s128) = COPY $v2
  ; CHECK-NEXT:   [[COPY1:%[0-9]+]]:_(s128) = COPY $v3
  ; CHECK-NEXT:   [[COPY2:%[0-9]+]]:_(s128) = COPY $v4
  ; CHECK-NEXT:   [[COPY3:%[0-9]+]]:_(s128) = COPY $v5
  ; CHECK-NEXT:   [[COPY4:%[0-9]+]]:_(s128) = COPY $v6
  ; CHECK-NEXT:   [[COPY5:%[0-9]+]]:_(s128) = COPY $v7
  ; CHECK-NEXT:   [[COPY6:%[0-9]+]]:_(s128) = COPY $v8
  ; CHECK-NEXT:   [[COPY7:%[0-9]+]]:_(s128) = COPY $v9
  ; CHECK-NEXT:   [[COPY8:%[0-9]+]]:_(s128) = COPY $v10
  ; CHECK-NEXT:   [[COPY9:%[0-9]+]]:_(s128) = COPY $v11
  ; CHECK-NEXT:   [[COPY10:%[0-9]+]]:_(s128) = COPY $v12
  ; CHECK-NEXT:   [[COPY11:%[0-9]+]]:_(s128) = COPY $v13
  ; CHECK-NEXT:   BLR8 implicit $lr8, implicit $rm
entry:
  ret void
}
