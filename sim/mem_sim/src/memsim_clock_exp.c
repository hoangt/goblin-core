/* 
 * _MEMSIM_CLOCK_EXP_C_ 
 * 
 * MEMORY INTERFACE SIMULATION LIBRARY
 * 
 * 
 * MEMSIM CLOCK EXP BACKEND
 * 
 * IMPLEMENTS THE LEIDEL ALGORITHM FOR MEMORY COALESCING ON 
 * HMC DEVICES 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "mem_sim.h"


/* ========== EXPERIMENTAL BACKEND ================
 * 
 * The experimental backend uses basic task group
 * queue inputs and transforms them into
 * trees based upon the target memory 
 * destination. 
 * 
 * The trees are constructed where the LHS
 * are read operations and the RHS are write
 * operations.  There is a single root 
 * node for each of {local,global,amo} trees
 * that does not contain an entry.
 * 
 *                     [ROOT]
 * 			 |
 * 			/ \
 *                     /   \
 *                READS     WRITES
 *                 |          |
 *                / \        / \
 *               /   \      /   \
 *              /     \    /     \ 
 *            -addr +addr -addr  +addr
 * 
 */





/* ------------------------------------------------ FUNCTION PROTOTYPES */
extern int memsim_cycle_tid( struct memsim_t *msim );
extern int memsim_tid_push( struct memsim_t *msim, uint32_t tid );
extern int memsim_tick_count( struct memsim_entry_t *ent, uint64_t *ticks );
extern int memsim_trace_memop( struct memsim_t *msim, struct memsim_entry_t *ent );
extern int memsim_trace_membus( struct memsim_t *msim, struct memsim_entry_t *ent );
extern int memsim_clear_entry( struct memsim_entry_t *ent );
extern int memsim_bubble_slot( struct memsim_slot_t *slot );
extern int memsim_find_slot( struct memsim_slot_t *slot, uint32_t *rtn );
extern int memsim_cp_entry( struct memsim_entry_t *src, struct memsim_entry_t *dest );
extern int memsim_rotate_tree( struct memsim_tree_t *tree );
extern int memsim_rqst_type( memsim_rqst_t rqst );
extern int memsim_insert_tree_entry( 	struct memsim_tree_t *tree, 
					struct memsim_entry_t *entry, 
					int type );


/* ------------------------------------------------ MEMSIM_CLOCK_EXP_PROCESS_SOCKET */
static int memsim_clock_exp_process_socket( struct memsim_t *msim ){ 

	/* vars */
        uint64_t count  = 0x00ll;
        uint64_t ticks  = 0x00ll;
        int done        = 0;
        int i           = 0;
        uint32_t cur    = 0;
        /* ---- */

        /*
         * process the slot entries within this clock cycle
         *
         */
        while( (done != 1) && (count<msim->hw.payps) ){

                /*
                 * check the 'cur' element
                 *
                 */
                ticks = 0x00ll;

                if( msim->socket->entry[cur].valid == 1 ){

                        /*
                         * cur element is valid, process it
                         *
                         */

                        /*
                         * First, determine the number of ticks
                         * required to process this entry
                         * If we don't have enough ticks,
                         * we're done.
                         *
                         * Each 64-bit element requires a tick
                         *
                         */
                        if( memsim_tick_count( &(msim->socket->entry[cur]), &ticks ) != 0 ){
                                return MEMSIM_ERROR;
                        }

                        if( (count+ticks) <= msim->hw.payps ){

                                /*
                                 * we're within range.
                                 * push the request
                                 *
                                 */

                                /* print the trace */
                                if( (msim->tracelevel & MEMSIM_TRACE_MEMOP ) > 0 ){
                                        memsim_trace_memop( msim,
                                                        &(msim->socket->entry[cur]) );
                                }

                                /* clear the valid tids */
                                for( i=0; i<msim->socket->entry[cur].num_tids; i++ ){
                                        memsim_tid_push( msim, msim->socket->entry[cur].tid[i] );
                                }

                                /* clear the entry */
                                memsim_clear_entry( &(msim->socket->entry[cur]) );

                                /* bump the count */
                                count += ticks;

                        }else{
				/*
                                 * out of range, time to stall
                                 *
                                 */
                                done = 1;
                        }

                }

                if( (cur+1) >= msim->socket->num_slots ){
                        /*
                         * Last element
                         *
                         */
                        done = 1;
                }else{
                        cur += 1;
                }

        }

        /*
         * bubble the slot entries
         *
         */
        if( memsim_bubble_slot( msim->socket ) != 0 ){
                return MEMSIM_ERROR;
        }

	return MEMSIM_OK;
}

/* ------------------------------------------------ MEMSIM_CLOCK_EXP_PROCESS_AMO */
static int memsim_clock_exp_process_amo( struct memsim_t *msim ){ 

	/* vars */
        int done        = 0;
        int rtn         = 0;
        uint32_t cur    = 0;
        uint32_t target = 0;
        /* ---- */


        /*
         * process the slot entries within this clock cycle
         *
         */
        while( (done != 1) ){

                /*
                 * check the 'cur' element
                 *
                 */
                if( msim->amo->entry[cur].valid == 1 ){

                        /*
                         * current element is valid, process it
                         *
                         * find a candidate slot, if no slot we're done
                         *
                         */
                        rtn = memsim_find_slot( msim->socket, &target );

                        if( rtn == MEMSIM_OK ){
                                /*
                                 * found an open slot, transfer data
                                 *
                                 */

                                /* print the trace */
                                if( (msim->tracelevel & MEMSIM_TRACE_MEMBUS ) > 0 ){
                                        memsim_trace_membus( msim,
                                                        &(msim->amo->entry[cur]) );
                                }

                                /* copy the entry */
                                if( memsim_cp_entry( &(msim->amo->entry[cur]),
                                                        &(msim->socket->entry[target])) != MEMSIM_OK ){
                                        return MEMSIM_ERROR;
                                }

                                /* clear the entry */
                                memsim_clear_entry( &(msim->amo->entry[cur]) );

                        }else{
                                /*
                                 * no more slots at the socket level
                                 *
                                 */
                                done = 1;
                        }
                }


                /*
                 * make sure we're not at the end of our valid slots
                 *
                 */
		if( (cur+1) >= msim->amo->num_slots ){
                        /*
                         * Last element
                         *
                         */
                        done = 1;
                }else{
                        cur += 1;
                }


        }

        /*
         * bubble the slot entries
         *
         */
        if( memsim_bubble_slot( msim->amo ) != 0 ){
                return MEMSIM_ERROR;
        }

	return MEMSIM_OK;
}

/* ------------------------------------------------ MEMSIM_CLOCK_EXP_PROCESS_GLOBAL */
static int memsim_clock_exp_process_global( struct memsim_t *msim ){
	
	/* vars */
        uint64_t count  = 0x00ll;
        uint64_t ticks  = 0x00ll;
        int done        = 0;
        uint32_t cur    = 0;
        /* ---- */

        /*
         * process the slot entries within this clock cycle
         *
         */
        while( (done != 1) && (count<msim->hw.payps) ){

                /*
                 * check the 'cur' element
                 *
                 */
                ticks = 0x00ll;

                if( msim->global->entry[cur].valid == 1 ){

                        /*
                         * cur element is valid, process it
                         *
                         */

                        /*
                         * First, determine the number of ticks
                         * required to process this entry
                         * If we don't have enough ticks,
                         * we're done.
                         *
                         * Each 64-bit element requires a tick
                         *
                         */
                        if( memsim_tick_count( &(msim->global->entry[cur]), &ticks ) != 0 ){
                                return MEMSIM_ERROR;
                        }

                        if( (count+ticks) <= msim->hw.payps ){

                                /*
                                 * we're within range.
                                 * push the request
                                 *
                                 */

                                /* print the trace */
                                if( (msim->tracelevel & MEMSIM_TRACE_MEMBUS ) > 0 ){
                                        memsim_trace_membus( msim,
                                                        &(msim->global->entry[cur]) );
                                }

                                /* clear the entry */
                                memsim_clear_entry( &(msim->global->entry[cur]) );

                                /* bump the count */
                                count += ticks;

                        }else{
                                /*
                                 * out of range, time to stall
                                 *
                                 */
                                done = 1;
                        }

                }

		if( (cur+1) >= msim->global->num_slots ){
                        /*
                         * Last element
                         *
                         */
                        done = 1;
                }else{
                        cur += 1;
                }

        }

        /*
         * bubble the slot entries
         *
         */
        if( memsim_bubble_slot( msim->global ) != 0 ){
                return MEMSIM_ERROR;
        }

        return MEMSIM_OK;
}

/* ------------------------------------------------ MEMSIM_CLOCK_EXP_PROCESS_GLOBAL_TREE */
/* 
 * Process the global tree and determine whether we have an appropriate 
 * request to flush or an appropriate tree to flush 
 * 
 */
static int memsim_clock_exp_process_global_tree( struct memsim_t *msim ) {

	/* 
	 * final step is rotate the tree
	 * 
	 */
	if( memsim_rotate_tree( msim->t_global ) != MEMSIM_OK ){ 
		return MEMSIM_ERROR;
	}

	return MEMSIM_OK;
}

/* ------------------------------------------------ MEMSIM_CLOCK_EXP_PROCESS_AMO_TREE */
/* 
 * Process the amo tree and determine whether we have an appropriate 
 * request to flush or an appropriate tree to flush 
 * 
 */
static int memsim_clock_exp_process_amo_tree( struct memsim_t *msim ) {

	/* 
	 * final step is rotate the tree
	 * 
	 */
	if( memsim_rotate_tree( msim->t_amo ) != MEMSIM_OK ){ 
		return MEMSIM_ERROR;
	}

	return MEMSIM_OK;
}

/* ------------------------------------------------ MEMSIM_CLOCK_EXP_PROCESS_LOCAL_TREE */
/* 
 * Process the local tree and determine whether we have an appropriate 
 * request to flush or an appropriate tree to flush 
 * 
 */
static int memsim_clock_exp_process_local_tree( struct memsim_t *msim ) {

	/* 
	 * final step is rotate the tree
	 * 
	 */
	if( memsim_rotate_tree( msim->t_local ) != MEMSIM_OK ){ 
		return MEMSIM_ERROR;
	}

	return MEMSIM_OK;
}

/* ------------------------------------------------ MEMSIM_CLOCK_EXP_PROCESS_TASKGROUP */
/* 
 * Process each group's request queue. 
 * For each request, attempt to enter it into the appropriate tree instance
 * 
 */
static int memsim_clock_exp_process_taskgroup( struct memsim_t *msim, uint32_t gr ){ 

	/* vars */
	int done			= 0;
	int type			= 0;
	int rtn				= 0;
	uint32_t cur			= 0;
	struct memsim_tree_t *tree	= NULL;
	/* ---- */

	/* 
	 * process the slot entries within this clock cycle 
	 * 
 	 */	
	while( (done != 1 ) ){ 
		
		/* 
	 	 * check the 'cur' element
	 	 * 
		 */
		if( msim->group[gr].entry[cur].valid == 1 ){ 
			
			/* 
			 * current element is valid, process it
			 * 
			 * select the approrpiate tree destination
			 * and determine whether we can push the 
	 		 * request downstream
			 * 
			 */
			
			/* 
			 * first, decide where to insert it 
			 *
			 */
			type	= memsim_rqst_type( msim->group[gr].entry[cur].rqst );

			switch( type )
			{
				case 0:
					/* read operations */
					tree = msim->t_local;
					break;
				case 1: 
					/* write operations */
					tree = msim->t_local;
					break;
				case 2:
					/* amo operations */
					tree = msim->t_amo;
					break;
				case 3:
					/* force a flush */
					tree = msim->t_local;
					break;
				default:
					return MEMSIM_ERROR;
					break;
			}

			rtn = memsim_insert_tree_entry( tree, &(msim->group[gr].entry[cur]), type);

			if( rtn == MEMSIM_ERROR ){ 
				return MEMSIM_ERROR;
			}else if( rtn == MEMSIM_STALL ){ 
				/* 
				 * insertion returned a stall, no more tree
				 * entries in the respective tree 
				 * todo : record a log entry 
				 * 
				 * we can't however dump out of this routine
				 * the subsequent requests may not go to the same 
				 * tree
				 * 
				 */
			}
			
		} /* end check the current element */

		/* 
	 	 * make sure we're not at the end of our valid slots
	 	 * 
	 	 */
		if( (cur+1) >= msim->group[gr].num_slots ){ 
			/* 
	 	 	 * Last element 
		 	 * 
		 	 */
			done = 1;
		}else{
			cur += 1;
		}
	}

	/* 
	 * bubble the slot entries
	 * 
	 */
	if( memsim_bubble_slot( &(msim->group[gr]) ) != 0 ){ 
		return MEMSIM_ERROR;
	}

	return rtn;
}

/* ------------------------------------------------ MEMSIM_CLOCK_EXP */
/* 
 * MEMSIM_CLOCK_EXP
 * 
 * This backend implementation implements a exp coherency
 * mechanism using a 4-way set associated exp model 
 * 
 * Each Task Group has a private D-EXP of some size
 * This is analogous to an L2 data exp.  There is no 
 * L3 data exp
 * 
 */
extern int memsim_clock_exp( struct memsim_t *msim )
{
	/* vars */
	uint32_t i	= 0;
	int rtn		= 0;
	/* ---- */

	/* 
	 * process the queues 
 	 * 
	 */
	/* -- socket queue */
	if( memsim_clock_exp_process_socket( msim ) != 0 ){ 
		return MEMSIM_ERROR;
	}

	/* -- amo queue */
	if( (msim->opt & MEMSIM_AMO) > 0 ){ 
		if( memsim_clock_exp_process_amo( msim ) != 0 ){ 
			return MEMSIM_ERROR;
		}
	} 

	/* -- global queue */
	if( (msim->opt & MEMSIM_GA) > 0 ){ 
		if( memsim_clock_exp_process_global( msim ) != 0 ){ 
			return MEMSIM_ERROR;
		}
	}

	/* -- local tree processing */
	if( memsim_clock_exp_process_local_tree( msim ) != 0 ){ 
		return MEMSIM_ERROR;
	} 

	/* -- amo tree processing */
	if( memsim_clock_exp_process_amo_tree( msim ) != 0 ){ 
		return MEMSIM_ERROR;
	} 

	/* -- global tree processing */
	if( memsim_clock_exp_process_global_tree( msim ) != 0 ){ 
		return MEMSIM_ERROR;
	} 

	/* 
	 * process each of the local task group queues
	 * 
 	 */
	for( i=0; i<msim->task_groups; i++ ){ 
		rtn = memsim_clock_exp_process_taskgroup( msim, i );
		if( rtn == MEMSIM_ERROR ) { 
			return rtn;
		}/* else, we have an ok or stall state */
	}

	/* 
	 * cycle the tids 
	 * 
	 */
	if( memsim_cycle_tid( msim ) != 0 ){ 
		return MEMSIM_ERROR;
	}

	return MEMSIM_OK;
}

/* EOF */
