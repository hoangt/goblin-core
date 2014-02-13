/* 
 * _EXECUTE_TEST_C_ 
 * 
 * MEMSIM STREAM TEST EXECUTION 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "mem_sim.h"

/* --------------------------------------------------- STATUS MACROS */
#define		TRIAD_START	0x00000
#define		TRIAD_SCALAR	0x00001
#define		TRIAD_LD_C	0x00002
#define		TRIAD_LD_B	0x00003
#define		TRIAD_MUL	0x00004
#define		TRIAD_ADD	0x00005
#define		TRIAD_ST_A	0x00006
#define		TRIAD_FLOW	0x00007

/* --------------------------------------------------- STRUCTURES */
struct tid_t{
	uint32_t ld_scalar;
	uint32_t ld_c;
	uint32_t ld_b;
	uint32_t st_a;

	uint8_t ld_scalar_v;
	uint8_t ld_c_v;
	uint8_t ld_b_v;
	uint8_t st_a_v;
};

/* --------------------------------------------------- EXECUTE_TEST */
/* 
 * EXECUTE TEST 
 * 
 * Executes a Stream Triad : 
 * 
 * a[j] = b[j] + scalar * c[j];
 * where, 0 <= j <= num_req
 * 
 */
extern int execute_test( 	struct memsim_t *msim, 
				uint32_t num_threads, 
				uint64_t *gconst,
				long num_req ) {


	/* vars */
	int ret			= 0;
	uint32_t i		= 0;
	uint32_t done		= 0;
	uint32_t l_tid		= 0;
	uint64_t ui		= 0x00ll;
	uint64_t niter		= 0x00ll;
	uint64_t scalar		= 0x123456789;
	uint64_t *a		= NULL;
	uint64_t *b		= NULL;
	uint64_t *c		= NULL;
	uint64_t *cur		= NULL;
	uint64_t *end		= NULL;	
	uint64_t *status	= NULL;
	struct tid_t *tids	= NULL;
	/* ---- */


	/* 
	 * allocate memory 
	 * 
	 */
	a = malloc( sizeof( uint64_t) * num_req );	
	if( a == NULL ){ 
		printf( "ERROR : COULD NOT ALLOCATE MEMORY FOR a\n" );
		return -1;
	}

	b = malloc( sizeof( uint64_t) * num_req );	
	if( b == NULL ){ 
		printf( "ERROR : COULD NOT ALLOCATE MEMORY FOR b\n" );
		free( a );
		return -1;
	}

	c = malloc( sizeof( uint64_t) * num_req );	
	if( c == NULL ){ 
		printf( "ERROR : COULD NOT ALLOCATE MEMORY FOR c\n" );
		free( a );
		free( b );
		return -1;
	}

	cur	= malloc( sizeof( uint64_t ) * num_threads );
	if( cur == NULL ){ 
		printf( "ERROR : COULD NOT ALLOCATE MEMORY FOR cur\n" );
		free( a );
		free( b );
		free( c );
		return -1;
	}

	end	= malloc( sizeof( uint64_t ) * num_threads );
	if( end == NULL ){ 
		printf( "ERROR : COULD NOT ALLOCATE MEMORY FOR end\n" );
		free( a );
		free( b );
		free( c );
		free( cur );
		return -1;
	}

	status	= malloc( sizeof( uint64_t ) * num_threads );
	if( status == NULL ){ 
		printf( "ERROR : COULD NOT ALLOCATE MEMORY FOR status\n" );
		free( a );
		free( b );
		free( c );
		free( cur );
		free( end );
		return -1;
	}

	tids	= malloc( sizeof( struct tid_t ) * num_threads ); 
	if( tids == NULL ){ 
		printf( "ERROR : COULD NOT ALLOCATE MEMORY FOR status\n" );
		free( a );
		free( b );
		free( c );
		free( cur );
		free( end );
		free( status );
		return -1;
	}

	printf( "ALLOCATED TRIAD DATA\n" );
	
	/* 
	 * init the data 
	 * 
	 */
	niter = (uint64_t)(num_req)/(uint64_t)(num_threads);

	for( ui=0; ui<(uint64_t)(num_req); ui++ ){ 
		a[ui] 	= ui*ui;
		b[ui] 	= ui*ui+1;
		c[ui] 	= ui*ui+2;
	}

	/* -- init the start and end points */
	for( ui=0; ui<(uint64_t)(num_threads); ui++ ){ 

		cur[ui]		= niter*ui;
		status[ui]	= TRIAD_SCALAR;

		if( ui == (uint64_t)(num_threads-1) ){
			end[ui]	= (uint64_t)(num_req-1);
		}else{ 
			end[ui]	= cur[ui] + (niter-1);
		}
	}

	/* -- setup the tids */
	for( i=0; i<num_threads; i++ ){
		tids[i].ld_scalar	= 0x00;
		tids[i].ld_c		= 0x00;
		tids[i].ld_b		= 0x00;
		tids[i].st_a		= 0x00;

		tids[i].ld_scalar_v	= 0;
		tids[i].ld_c_v		= 0;
		tids[i].ld_b_v		= 0;
		tids[i].st_a_v		= 0;
	}

	printf( "INITIALIZED TRIAD DATA\n" );

	printf( "BEGINNING EXECUTION FOR %"PRIu32" THREADS\n", num_threads );

	/* 
	 * begin the test loop
	 *
	 */
	while( done < num_threads ){ 

		/* 
		 * each clock cycle iterate through all
		 * the threads
		 *
 		 * a[j] = b[j] + scalar * c[j];
		 */
		for( i=0; i<num_threads; i++ ){ 
			
			if( cur[i] == end[i]  ){
				/* this thread is done, do nothing */
			}else if( status[i] == TRIAD_START ){ 
				/* starting point */
				/* not used for this benchmark */
			}else if( status[i] == TRIAD_SCALAR ){ 

				/* scalar load */
				ret	= memsim_rqst( 	msim, 
							gconst[i], 
							MEMSIM_RD8,
							(uint64_t)(&scalar), 
							0x00ll, 
							0x00ll, 
							&l_tid );
				if( ret == 0 ){
					status[i]	= TRIAD_LD_C;
					tids[i].ld_scalar= l_tid;
					tids[i].ld_scalar_v = 1;
				}
					
			}else if( status[i] == TRIAD_LD_C ){ 

				/* load c[i] */
				ret	= memsim_rqst( 	msim, 
							gconst[i], 
							MEMSIM_RD8,
							(uint64_t)(&a[cur[i]]), 
							0x00ll, 
							0x00ll, 
							&l_tid );
				if( ret == 0 ){
					status[i]	= TRIAD_LD_B;
					tids[i].ld_c	= l_tid;
					tids[i].ld_c_v	= 1;
				}

			}else if( status[i] == TRIAD_LD_B ){ 

				/* load b[i] */
				ret	= memsim_rqst( 	msim, 
							gconst[i], 
							MEMSIM_RD8,
							(uint64_t)(&b[cur[i]]), 
							0x00ll, 
							0x00ll, 
							&l_tid );
				if( ret == 0 ){
					status[i]	= TRIAD_MUL;
					tids[i].ld_b	= l_tid;
					tids[i].ld_b_v	= 1;
				}
				
			}else if( status[i] == TRIAD_MUL ){ 

				/* mul */

			}else if( status[i] == TRIAD_ADD ){ 
				/* add */
			}else if( status[i] == TRIAD_ST_A ){ 
				/* store a[i] */
			}else if( status[i] == TRIAD_FLOW ){
				/* flow control */
			}
	
		}

		/* 
	 	 * clock the sim 
		 *
		 */
		memsim_clock( msim );

	} /* -- end test loop */

	printf( "SUCCESS : EXECUTION COMPLETE\n" );

	/* 
	 * free the memory
 	 *
	 */
	free( a );
	free( b );
	free( c );
	free( tids );
	free( cur );
	free( end );
	free( status );

	return 0;
}

/* EOF */
