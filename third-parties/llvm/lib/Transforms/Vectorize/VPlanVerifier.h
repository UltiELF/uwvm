﻿//===-- VPlanVerifier.h -----------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file declares the class VPlanVerifier, which contains utility functions
/// to check the consistency of a VPlan. This includes the following kinds of
/// invariants:
///
/// 1. Region/Block invariants:
///   - Region's entry/exit block must have no predecessors/successors,
///     respectively.
///   - Block's parent must be the region immediately containing the block.
///   - Linked blocks must have a bi-directional link (successor/predecessor).
///   - All predecessors/successors of a block must belong to the same region.
///   - Blocks must have no duplicated successor/predecessor.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_VECTORIZE_VPLANVERIFIER_H
#define LLVM_TRANSFORMS_VECTORIZE_VPLANVERIFIER_H

namespace llvm {
class VPRegionBlock;
class VPlan;

/// Struct with utility functions that can be used to check the consistency and
/// invariants of a VPlan, including the components of its H-CFG.
struct VPlanVerifier {
  /// Verify the invariants of the H-CFG starting from \p TopRegion. The
  /// verification process comprises the following steps:
  /// 1. Region/Block verification: Check the Region/Block verification
  /// invariants for every region in the H-CFG.
  void verifyHierarchicalCFG(const VPRegionBlock *TopRegion) const;

  /// Verify invariants for general VPlans. Currently it checks the following:
  /// 1. all phi-like recipes must be at the beginning of a block, with no other
  /// recipes in between. Note that currently there is still an exception for
  /// VPBlendRecipes.
  static bool verifyPlanIsValid(const VPlan &Plan);
};
} // namespace llvm

#endif //LLVM_TRANSFORMS_VECTORIZE_VPLANVERIFIER_H
