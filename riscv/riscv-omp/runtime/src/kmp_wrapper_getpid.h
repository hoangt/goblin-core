/*
 * kmp_wrapper_getpid.h -- getpid() declaration.
 * $Revision: 42951 $
 * $Date: 2014-01-21 14:41:41 -0600 (Tue, 21 Jan 2014) $
 */


//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//


#ifndef KMP_WRAPPER_GETPID_H
#define KMP_WRAPPER_GETPID_H

#if KMP_OS_UNIX

    // On Unix-like systems (Linux* OS and OS X*) getpid() is declared in standard headers.
    #include <sys/types.h>
    #include <unistd.h>

#elif KMP_OS_WINDOWS

    // On Windows* OS _getpid() returns int (not pid_t) and is declared in "process.h".
    #include <process.h>
    // Let us simulate Unix.
    typedef int pid_t;
    #define getpid _getpid

#else

    #error Unknown or unsupported OS.

#endif

/*
    TODO: All the libomp source code uses pid_t type for storing the result of getpid(), it is good.
    But often it printed as "%d", that is not good, because it ignores pid_t definition (may pid_t
    be longer that int?). It seems all pid prints should be rewritten as

        printf( "%" KMP_UINT64_SPEC, (kmp_uint64) pid );

    or (at least) as

        printf( "%" KMP_UINT32_SPEC, (kmp_uint32) pid );

    (kmp_uint32, kmp_uint64, KMP_UINT64_SPEC, and KMP_UNIT32_SPEC are defined in "kmp_os.h".)

*/

#endif // KMP_WRAPPER_GETPID_H

// end of file //
