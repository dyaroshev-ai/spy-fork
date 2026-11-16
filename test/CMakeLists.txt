##==================================================================================================
##  SPY - C++ Informations Broker
##  Copyright : SPY Project Contributors
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
copa_setup_test_targets()
set(TEST_ROOT "${CMAKE_SOURCE_DIR}/test")
copa_glob_unit(QUIET PATTERN "unit/*.cpp" INTERFACE spy_tests)
copa_glob_unit(QUIET PATTERN "samples/*.cpp" INTERFACE spy_tests)

##======================================================================================================================
## accelerator.cpp and accelerator.cu are the same test compiled twice: as plain C++ spy must
## report no CUDA, as CUDA it must report some. Only the second needs a CUDA compiler to exist.
##======================================================================================================================
if(CMAKE_CUDA_COMPILER)
  copa_make_single_unit(
    INTERFACE spy_tests
    FILES unit/accelerator.cu
    ROOT ${TEST_ROOT}
    NAME unit.accelerator.device.cu)
endif()

##======================================================================================================================
## SVE detection is all preprocessor: compiling one file per flavour and vector length checks every
## -msve-vector-bits the compilers take without a preset, a CI row or an emulator for each. The
## compiler is called directly, so the tests never go through the CMAKE_CROSSCOMPILING_CMD.
##======================================================================================================================
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-march=armv8-a+sve2 -msve-vector-bits=128" SPY_COMPILER_TAKES_SVE)

if(SPY_COMPILER_TAKES_SVE)
  if(CMAKE_CXX_COMPILER_TARGET)
    set(SVE_TARGET "--target=${CMAKE_CXX_COMPILER_TARGET}")
  endif()

  foreach(flavour sve sve2)
    foreach(bits scalable 128 256 512 1024 2048)
      add_test(NAME sve.${flavour}.${bits}
               COMMAND ${CMAKE_CXX_COMPILER} ${SVE_TARGET} -std=c++20 -fsyntax-only -march=armv8-a+${flavour}
                       -msve-vector-bits=${bits} -I${PROJECT_SOURCE_DIR}/include ${TEST_ROOT}/sve/width.cpp)
    endforeach()
  endforeach()
endif()
