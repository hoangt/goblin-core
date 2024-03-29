/*
 * kmp_stub.c -- stub versions of user-callable OpenMP RT functions.
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


#include <stdlib.h>
#include <limits.h>
#include <errno.h>

#include "omp.h"                // Function renamings.
#include "kmp.h"                // KMP_DEFAULT_STKSIZE
#include "kmp_stub.h"

#if KMP_OS_WINDOWS
    #include <windows.h>
#else
    #include <sys/time.h>
#endif

// Moved from omp.h
#define omp_set_max_active_levels    ompc_set_max_active_levels
#define omp_set_schedule             ompc_set_schedule
#define omp_get_ancestor_thread_num  ompc_get_ancestor_thread_num
#define omp_get_team_size            ompc_get_team_size

#define omp_set_num_threads          ompc_set_num_threads
#define omp_set_dynamic              ompc_set_dynamic
#define omp_set_nested               ompc_set_nested
#define kmp_set_stacksize            kmpc_set_stacksize
#define kmp_set_stacksize_s          kmpc_set_stacksize_s
#define kmp_set_blocktime            kmpc_set_blocktime
#define kmp_set_library              kmpc_set_library
#define kmp_set_defaults             kmpc_set_defaults
#define kmp_malloc                   kmpc_malloc
#define kmp_calloc                   kmpc_calloc
#define kmp_realloc                  kmpc_realloc
#define kmp_free                     kmpc_free

static double frequency = 0.0;

// Helper functions.
static size_t __kmps_init() {
    static int    initialized = 0;
    static size_t dummy = 0;
    if ( ! initialized ) {

        // TODO: Analyze KMP_VERSION environment variable, print __kmp_version_copyright and
        // __kmp_version_build_time.
        // WARNING: Do not use "fprintf( stderr, ... )" because it will cause unresolved "__iob"
        // symbol (see C70080). We need to extract __kmp_printf() stuff from kmp_runtime.c and use
        // it.

        // Trick with dummy variable forces linker to keep __kmp_version_copyright and
        // __kmp_version_build_time strings in executable file (in case of static linkage).
        // When KMP_VERSION analyze is implemented, dummy variable should be deleted, function
        // should return void.
        dummy = __kmp_version_copyright - __kmp_version_build_time;

        #if KMP_OS_WINDOWS
            LARGE_INTEGER freq;
            BOOL status = QueryPerformanceFrequency( & freq );
            if ( status ) {
                frequency = double( freq.QuadPart );
            }; // if
        #endif

        initialized = 1;
    }; // if
    return dummy;
}; // __kmps_init

#define i __kmps_init();

/* set API functions */
void omp_set_num_threads( omp_int_t num_threads ) { i; }
void omp_set_dynamic( omp_int_t dynamic )         { i; __kmps_set_dynamic( dynamic ); }
void omp_set_nested( omp_int_t nested )           { i; __kmps_set_nested( nested );   }
void omp_set_max_active_levels( omp_int_t max_active_levels ) { i; }
void omp_set_schedule( omp_sched_t kind, omp_int_t modifier ) { i; __kmps_set_schedule( (kmp_sched_t)kind, modifier ); }
int omp_get_ancestor_thread_num( omp_int_t level ) { i; return ( level ) ? ( -1 ) : ( 0 ); }
int omp_get_team_size( omp_int_t level ) { i; return ( level ) ? ( -1 ) : ( 1 ); }
int kmpc_set_affinity_mask_proc( int proc, void **mask ) { i; return -1; }
int kmpc_unset_affinity_mask_proc( int proc, void **mask ) { i; return -1; }
int kmpc_get_affinity_mask_proc( int proc, void **mask ) { i; return -1; }

/* kmp API functions */
void kmp_set_stacksize( omp_int_t arg )   { i; __kmps_set_stacksize( arg ); }
void kmp_set_stacksize_s( size_t arg )    { i; __kmps_set_stacksize( arg ); }
void kmp_set_blocktime( omp_int_t arg )   { i; __kmps_set_blocktime( arg ); }
void kmp_set_library( omp_int_t arg )     { i; __kmps_set_library( arg ); }
void kmp_set_defaults( char const * str ) { i; }

/* KMP memory management functions. */
void * kmp_malloc( size_t size )                 { i; return malloc( size ); }
void * kmp_calloc( size_t nelem, size_t elsize ) { i; return calloc( nelem, elsize ); }
void * kmp_realloc( void *ptr, size_t size )     { i; return realloc( ptr, size ); }
void   kmp_free( void * ptr )                    { i; free( ptr ); }

static int __kmps_blocktime = INT_MAX;

void __kmps_set_blocktime( int arg ) {
    i;
    __kmps_blocktime = arg;
} // __kmps_set_blocktime

int __kmps_get_blocktime( void ) {
    i;
    return __kmps_blocktime;
} // __kmps_get_blocktime

static int __kmps_dynamic = 0;

void __kmps_set_dynamic( int arg ) {
    i;
    __kmps_dynamic = arg;
} // __kmps_set_dynamic

int __kmps_get_dynamic( void ) {
    i;
    return __kmps_dynamic;
} // __kmps_get_dynamic

static int __kmps_library = 1000;

void __kmps_set_library( int arg ) {
    i;
    __kmps_library = arg;
} // __kmps_set_library

int __kmps_get_library( void ) {
    i;
    return __kmps_library;
} // __kmps_get_library

static int __kmps_nested = 0;

void __kmps_set_nested( int arg ) {
    i;
    __kmps_nested = arg;
} // __kmps_set_nested

int __kmps_get_nested( void ) {
    i;
    return __kmps_nested;
} // __kmps_get_nested

static size_t __kmps_stacksize = KMP_DEFAULT_STKSIZE;

void __kmps_set_stacksize( int arg ) {
    i;
    __kmps_stacksize = arg;
} // __kmps_set_stacksize

int __kmps_get_stacksize( void ) {
    i;
    return __kmps_stacksize;
} // __kmps_get_stacksize

static kmp_sched_t __kmps_sched_kind     = kmp_sched_default;
static int         __kmps_sched_modifier = 0;

    void __kmps_set_schedule( kmp_sched_t kind, int modifier ) {
        i;
        __kmps_sched_kind     = kind;
        __kmps_sched_modifier = modifier;
    } // __kmps_set_schedule

    void __kmps_get_schedule( kmp_sched_t *kind, int *modifier ) {
        i;
        *kind     = __kmps_sched_kind;
        *modifier = __kmps_sched_modifier;
    } // __kmps_get_schedule

#if OMP_40_ENABLED

static kmp_proc_bind_t __kmps_proc_bind = proc_bind_false;

void __kmps_set_proc_bind( kmp_proc_bind_t arg ) {
    i;
    __kmps_proc_bind = arg;
} // __kmps_set_proc_bind

kmp_proc_bind_t __kmps_get_proc_bind( void ) {
    i;
    return __kmps_proc_bind;
} // __kmps_get_proc_bind

#endif /* OMP_40_ENABLED */

double __kmps_get_wtime( void ) {
    // Elapsed wall clock time (in second) from "sometime in the past".
    double wtime = 0.0;
    i;
    #if KMP_OS_WINDOWS
        if ( frequency > 0.0 ) {
            LARGE_INTEGER now;
            BOOL status = QueryPerformanceCounter( & now );
            if ( status ) {
                wtime = double( now.QuadPart ) / frequency;
            }; // if
        }; // if
    #else
        // gettimeofday() returns seconds and microseconds sinse the Epoch.
        struct timeval  tval;
        int             rc;
        rc = gettimeofday( & tval, NULL );
        if ( rc == 0 ) {
            wtime = (double)( tval.tv_sec ) + 1.0E-06 * (double)( tval.tv_usec );
        } else {
            // TODO: Assert or abort here.
        }; // if
    #endif
    return wtime;
}; // __kmps_get_wtime

double __kmps_get_wtick( void ) {
    // Number of seconds between successive clock ticks.
    double wtick = 0.0;
    i;
    #if KMP_OS_WINDOWS
        {
            DWORD increment;
            DWORD adjustment;
            BOOL  disabled;
            BOOL  rc;
            rc = GetSystemTimeAdjustment( & adjustment, & increment, & disabled );
            if ( rc ) {
                wtick = 1.0E-07 * (double)( disabled ? increment : adjustment );
            } else {
                // TODO: Assert or abort here.
                wtick = 1.0E-03;
            }; // if
        }
    #else
        // TODO: gettimeofday() returns in microseconds, but what the precision?
        wtick = 1.0E-06;
    #endif
    return wtick;
}; // __kmps_get_wtick

// end of file //

