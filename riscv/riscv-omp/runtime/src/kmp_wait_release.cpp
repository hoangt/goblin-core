/*
 * kmp_wait_release.cpp -- Wait/Release implementation
 * $Revision: 43417 $
 * $Date: 2014-08-26 14:06:38 -0500 (Tue, 26 Aug 2014) $
 */


//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//

#include "kmp_wait_release.h"

void __kmp_wait_32(kmp_info_t *this_thr, kmp_flag_32 *flag, int final_spin
                   USE_ITT_BUILD_ARG(void * itt_sync_obj) )
{
    __kmp_wait_template(this_thr, flag, final_spin
                        USE_ITT_BUILD_ARG(itt_sync_obj) );
}

void __kmp_wait_64(kmp_info_t *this_thr, kmp_flag_64 *flag, int final_spin
                   USE_ITT_BUILD_ARG(void * itt_sync_obj) )
{
    __kmp_wait_template(this_thr, flag, final_spin
                        USE_ITT_BUILD_ARG(itt_sync_obj) );
}

void __kmp_wait_oncore(kmp_info_t *this_thr, kmp_flag_oncore *flag, int final_spin
                       USE_ITT_BUILD_ARG(void * itt_sync_obj) )
{
    __kmp_wait_template(this_thr, flag, final_spin
                        USE_ITT_BUILD_ARG(itt_sync_obj) );
}



void __kmp_release_32(kmp_flag_32 *flag) {
    __kmp_release_template(flag);
}

void __kmp_release_64(kmp_flag_64 *flag) {
    __kmp_release_template(flag);
}

void __kmp_release_oncore(kmp_flag_oncore *flag) {
    __kmp_release_template(flag);
}
