#
#//===----------------------------------------------------------------------===//
#//
#//                     The LLVM Compiler Infrastructure
#//
#// This file is dual licensed under the MIT and the University of Illinois Open
#// Source Licenses. See LICENSE.txt for details.
#//
#//===----------------------------------------------------------------------===//
#

# This file holds Clang (clang/clang++) specific compiler dependent flags
# The flag types are:
#   1) C/C++ Compiler flags
#   2) Linker flags

#########################################################
# C/C++ Compiler flags
function(append_compiler_specific_c_and_cxx_flags input_c_flags input_cxx_flags)
    set(local_c_flags)
    set(local_cxx_flags)
    append_c_and_cxx_flags("-std=c++0x") # Enables support for many C++11 (formerly known as C++0x) features. The following are the most recently added features:
    append_c_and_cxx_flags("-fno-exceptions") # Exception handling table generation is disabled.
    append_c_and_cxx_flags("-x c++") # Compile C files as C++ files
    if(${IA32})
        append_c_and_cxx_flags("-m32") # Generate 32 bit IA-32 architecture code
        append_c_and_cxx_flags("-msse2") # Allow use of Streaming SIMD Instructions
    elseif(${ARM})
        append_c_and_cxx_flags("-marm") # Target the ARM architecture
    endif()
    append_c_and_cxx_flags("-Wno-unused-value") # Don't warn about unused values
    append_c_and_cxx_flags("-Wno-switch") # Don't warn about switch statements that don't cover entire range of values
    append_c_and_cxx_flags("-Wno-deprecated-register") # Don't warn about using register keyword
    set(${input_c_flags}   ${${input_c_flags}}   "${local_c_flags}" PARENT_SCOPE)
    set(${input_cxx_flags} ${${input_cxx_flags}} "${local_cxx_flags}" PARENT_SCOPE)
endfunction()

#########################################################
# Linker flags
function(append_compiler_specific_linker_flags input_ld_flags input_ld_flags_libs)
    set(local_ld_flags)
    set(local_ld_flags_libs)
    if(${IA32})
        append_linker_flags("-m32")
        append_linker_flags("-msse2")
    endif()
    set(${input_ld_flags}      ${${input_ld_flags}}      "${local_ld_flags}"       PARENT_SCOPE)
    set(${input_ld_flags_libs} ${${input_ld_flags_libs}} "${local_ld_flags_libs}"  PARENT_SCOPE)
endfunction()

