/*
 * kmp_omp.h -- OpenMP definition for kmp_omp_struct_info_t.
 *              This is for information about runtime library structures.
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


/* THIS FILE SHOULD NOT BE MODIFIED IN IDB INTERFACE LIBRARY CODE
 * It should instead be modified in the OpenMP runtime and copied
 * to the interface library code.  This way we can minimize the
 * problems that this is sure to cause having two copies of the
 * same file.
 *
 * files live in libomp and libomp_db/src/include
 */

/* CHANGE THIS WHEN STRUCTURES BELOW CHANGE
 * Before we release this to a customer, please don't change this value.  After it is released and
 * stable, then any new updates to the structures or data structure traversal algorithms need to
 * change this value.
 */
#define KMP_OMP_VERSION 8

typedef struct {
    kmp_int32  offset;
    kmp_int32  size;
} offset_and_size_t;

typedef struct {
    kmp_uint64    addr;
    kmp_int32     size;
    kmp_int32     padding;
} addr_and_size_t;

typedef struct {
    kmp_uint64   flags;         // Flags for future extensions.
    kmp_uint64   file;          // Pointer to name of source file where the parallel region is.
    kmp_uint64   func;          // Pointer to name of routine where the parallel region is.
    kmp_int32    begin;         // Beginning of source line range.
    kmp_int32    end;           // End of source line range.
    kmp_int32    num_threads;   // Specified number of threads.
} kmp_omp_nthr_item_t;

typedef struct {
    kmp_int32     num;          // Number of items in the arrray.
    kmp_uint64    array;        // Address of array of kmp_omp_num_threads_item_t.
} kmp_omp_nthr_info_t;


/* This structure is known to the idb interface library */
typedef struct {

    /* Change this only if you make a fundamental data structure change here */
    kmp_int32          lib_version;

    /* sanity check.  Only should be checked if versions are identical
     * This is also used for backward compatibility to get the runtime
     * structure size if it the runtime is older than the interface */
    kmp_int32          sizeof_this_structure;

    /* OpenMP RTL version info. */
    addr_and_size_t    major;
    addr_and_size_t    minor;
    addr_and_size_t    build;
    addr_and_size_t    banner;

    /* Various globals. */
    addr_and_size_t  threads;            // Pointer to __kmp_threads.
    addr_and_size_t  roots;              // Pointer to __kmp_root.
    addr_and_size_t  capacity;           // Pointer to __kmp_threads_capacity.
    addr_and_size_t  monitor;            // Pointer to __kmp_monitor.
    addr_and_size_t  lock_table;         // Pointer to __kmp_lock_table.
    addr_and_size_t  func_microtask;
    addr_and_size_t  func_fork;
    addr_and_size_t  team_counter;
    addr_and_size_t  task_counter;
    addr_and_size_t  nthr_info;
    kmp_int32        address_width;
    kmp_int32        indexed_locks;
    kmp_int32        last_barrier;
    kmp_int32        deque_size;

    /* thread structure information. */
    kmp_int32          th_sizeof_struct;
    offset_and_size_t  th_info;          // descriptor for thread
    offset_and_size_t  th_team;          // team for this thread
    offset_and_size_t  th_root;          // root for this thread
    offset_and_size_t  th_serial_team;   // serial team under this thread
    offset_and_size_t  th_ident;         // location for this thread (if available)
    offset_and_size_t  th_spin_here;     // is thread waiting for lock (if available)
    offset_and_size_t  th_next_waiting;  // next thread waiting for lock (if available)
    offset_and_size_t  th_task_team;
    offset_and_size_t  th_current_task;
    offset_and_size_t  th_task_state;
    offset_and_size_t  th_bar;
    offset_and_size_t  th_b_worker_arrived;

    /* kmp_desc structure (for info field above) */
    kmp_int32          ds_sizeof_struct;
    offset_and_size_t  ds_tid;           // team thread id
    offset_and_size_t  ds_gtid;          // global thread id
    offset_and_size_t  ds_thread;        // native thread id

    /* team structure information */
    kmp_int32          t_sizeof_struct;
    offset_and_size_t  t_master_tid;     // tid of master in parent team
    offset_and_size_t  t_ident;          // location of parallel region
    offset_and_size_t  t_parent;         // parent team
    offset_and_size_t  t_nproc;          // # team threads
    offset_and_size_t  t_threads;        // array of threads
    offset_and_size_t  t_serialized;     // # levels of serialized teams
    offset_and_size_t  t_id;             // unique team id
    offset_and_size_t  t_pkfn;
    offset_and_size_t  t_task_team;
    offset_and_size_t  t_implicit_task;
    offset_and_size_t  t_bar;
    offset_and_size_t  t_b_master_arrived;
    offset_and_size_t  t_b_team_arrived;

    /* root structure information */
    kmp_int32          r_sizeof_struct;
    offset_and_size_t  r_root_team;      // team at root
    offset_and_size_t  r_hot_team;       // hot team for this root
    offset_and_size_t  r_uber_thread;    // root thread
    offset_and_size_t  r_root_id;        // unique root id (if available)

    /* ident structure information */
    kmp_int32          id_sizeof_struct;
    offset_and_size_t  id_psource;       /* address of string ";file;func;line1;line2;;". */
    offset_and_size_t  id_flags;

    /* lock structure information */
    kmp_int32          lk_sizeof_struct;
    offset_and_size_t  lk_initialized;
    offset_and_size_t  lk_location;
    offset_and_size_t  lk_tail_id;
    offset_and_size_t  lk_head_id;
    offset_and_size_t  lk_next_ticket;
    offset_and_size_t  lk_now_serving;
    offset_and_size_t  lk_owner_id;
    offset_and_size_t  lk_depth_locked;

    /* lock_table_t */
    kmp_int32          lt_size_of_struct;    /* Size and layout of kmp_lock_table_t. */
    offset_and_size_t  lt_used;
    offset_and_size_t  lt_allocated;
    offset_and_size_t  lt_table;

    /* task_team_t */
    kmp_int32          tt_sizeof_struct;
    offset_and_size_t  tt_threads_data;
    offset_and_size_t  tt_found_tasks;
    offset_and_size_t  tt_nproc;
    offset_and_size_t  tt_unfinished_threads;
    offset_and_size_t  tt_active;
    offset_and_size_t  tt_state;

    /* kmp_taskdata_t */
    kmp_int32          td_sizeof_struct;
    offset_and_size_t  td_task_id;
    offset_and_size_t  td_flags;
    offset_and_size_t  td_team;
    offset_and_size_t  td_parent;
    offset_and_size_t  td_ident;
    offset_and_size_t  td_allocated_child_tasks;
    offset_and_size_t  td_incomplete_child_tasks;

    /* kmp_thread_data_t */
    kmp_int32          hd_sizeof_struct;
    offset_and_size_t  hd_deque;
    offset_and_size_t  hd_deque_head;
    offset_and_size_t  hd_deque_tail;
    offset_and_size_t  hd_deque_ntasks;
    offset_and_size_t  hd_deque_last_stolen;

    // The last field of stable version.
    kmp_uint64         last_field;
    // Extensions. 
    // When KMP_OMP_VERSION is bumped, move these fields up to appropriate location,
    // to let last_field be actually last.
    addr_and_size_t    openmp_version;
    offset_and_size_t  td_taskwait_ident;
    offset_and_size_t  td_taskwait_counter;
    offset_and_size_t  td_taskwait_thread;

    offset_and_size_t  lk_lock_flags;

} kmp_omp_struct_info_t;


/* end of file */
