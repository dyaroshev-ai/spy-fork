//==================================================================================================
/**
  SPY - C++ Informations Broker
  Copyright : SPY Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
// Compiled, never run, once per SVE flavour and vector length by test/CMakeLists.txt: detection is
// all preprocessor, so one aarch64 job covers every -msve-vector-bits without a preset for each.
#include <spy/spy.hpp>

#if !defined(__ARM_FEATURE_SVE)
#error "this test is meant to be compiled with SVE enabled"
#endif

#if defined(__ARM_FEATURE_SVE_BITS) && (__ARM_FEATURE_SVE_BITS > 0)
static_assert(spy::simd_instruction_set.has_fixed_cardinal());
static_assert(spy::simd_instruction_set.width == __ARM_FEATURE_SVE_BITS);
#if defined(__ARM_FEATURE_SVE2)
static_assert(spy::simd_instruction_set == spy::fixed_sve2_);
#else
static_assert(spy::simd_instruction_set == spy::fixed_sve_);
#endif
#else
static_assert(!spy::simd_instruction_set.has_fixed_cardinal());
#if defined(__ARM_FEATURE_SVE2)
static_assert(spy::simd_instruction_set == spy::sve2_);
#else
static_assert(spy::simd_instruction_set == spy::sve_);
#endif
#endif

int main() {}
