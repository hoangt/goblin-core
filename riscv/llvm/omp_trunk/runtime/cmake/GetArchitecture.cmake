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

# Determine the architecture from predefined compiler macros
# The architecture name can only contain alphanumeric characters and underscores (i.e., C identifier)

# void get_architecture(string* return_arch)
# - Returns the architecture in return_arch
function(get_architecture return_arch)
    set(detect_arch_src_txt "
    #if defined(__KNC__)
        #error ARCHITECTURE=mic
    #elif defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
        #error ARCHITECTURE=32e
    #elif defined(__i386) || defined(__i386__) || defined(__IA32__) || defined(_M_I86) || defined(_M_IX86) || defined(__X86__) || defined(_X86_)
        #error ARCHITECTURE=32
    #elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7R__) ||  defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7M__)  || defined(__ARM_ARCH_7S__)
        #error ARCHITECTURE=arm
    #elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__)  || defined(__ARM_ARCH_6Z__)  || defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_6ZK__)
        #error ARCHITECTURE=arm
    #elif defined(__ARM_ARCH_5__) || defined(__ARM_ARCH_5T__) || defined(__ARM_ARCH_5E__)  || defined(__ARM_ARCH_5TE__) || defined(__ARM_ARCH_5TEJ__)
        #error ARCHITECTURE=arm
    #elif defined(__ARM_ARCH_4__) || defined(__ARM_ARCH_4T__) 
        #error ARCHITECTURE=arm
    #elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
        #error ARCHITECTURE=arm
    #elif defined(__ARM_ARCH_2__)
        #error ARCHITECTURE=arm
    #elif defined(__arm__) || defined(_M_ARM) || defined(_ARM)
        #error ARCHITECTURE=arm
    #elif defined(__aarch64__)
        #error ARCHITECTURE=aarch64
    #elif defined(__powerpc64__) && defined(__LITTLE_ENDIAN__)
        #error ARCHITECTURE=ppc64le
    #elif defined(__powerpc64__)
        #error ARCHITECTURE=ppc64
    #else
        #error ARCHITECTURE=UnknownArchitecture
    #endif
    "
    )
    # Write out ${detect_arch_src_txt} to a file within the cmake/ subdirectory
    file(WRITE "${LIBOMP_WORK}/cmake/detect_arch.c" ${detect_arch_src_txt}) 

    # Try to compile using the C Compiler.  It will always error out with an #error directive, so store error output to ${local_architecture}
    try_run(run_dummy compile_dummy "${CMAKE_CURRENT_BINARY_DIR}" "${LIBOMP_WORK}/cmake/detect_arch.c" COMPILE_OUTPUT_VARIABLE local_architecture)

    # Match the important architecture line and store only that matching string in ${local_architecture}
    string(REGEX MATCH "ARCHITECTURE=([a-zA-Z0-9_]+)" local_architecture "${local_architecture}")

    # Get rid of the ARCHITECTURE= part of the string
    string(REPLACE "ARCHITECTURE=" "" local_architecture "${local_architecture}")

    # set the return value to the architecture detected (e.g., 32e, 32, arm, ppc64, etc.)
    set(${return_arch} "${local_architecture}" PARENT_SCOPE)

    # Remove ${detect_arch_src_txt} from cmake/ subdirectory
    file(REMOVE "${LIBOMP_WORK}/cmake/detect_arch.c")
endfunction()
