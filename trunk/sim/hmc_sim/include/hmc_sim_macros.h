/* 
 * _HMC_SIM_MACROS_H_
 * 
 * HYBRID MEMORY CUBE SIMULATION LIBRARY
 * 
 * MACROS HEADER FILE 
 * 
 */

#ifndef _HMC_SIM_MACROS_H_
#define _HMC_SIM_MACROS_H_


#ifdef __cplusplus
extern "C" {
#endif


/* -------------------------------------------- RETURN CODES */
#define		HMC_ERROR		-1
#define		HMC_OK			0
#define		HMC_STALL		2

/* -------------------------------------------- MACROS */
#define		HMC_MAX_DEVS		8
#define		HMC_MAX_LINKS		8
#define		HMC_MIN_LINKS		4
#define		HMC_MAX_CAPACITY	8
#define		HMC_MIN_CAPACITY	2
#define		HMC_MAX_VAULTS		16
#define		HMC_MIN_VAULTS		32
#define		HMC_MAX_BANKS		8
#define		HMC_MIN_BANKS		16
#define		HMC_MIN_DRAMS		20
#define		HMC_MAX_DRAMS		20
#define		HMC_MIN_QUEUE_DEPTH	2
#define		HMC_MAX_QUEUE_DEPTH	65536
#define 	HMC_MAX_UQ_PACKET	18

#define		HMC_RQST_VALID		1
#define		HMC_RQST_INVALID	0

#define		HMC_1GB		1073741824

#define		HMC_NUM_REGS		26

#define		HMC_REG_EDR0		0x2B0000
#define		HMC_REG_EDR1		0x2B0001
#define		HMC_REG_EDR2		0x2B0002
#define		HMC_REG_EDR3		0x2B0003
#define		HMC_REG_ERR		0x2B0004
#define		HMC_REG_GC		0x280000
#define		HMC_REG_LC0		0x240000
#define		HMC_REG_LC1		0x250000
#define		HMC_REG_LC2		0x260000
#define		HMC_REG_LC3		0x270000
#define		HMC_REG_LRLL0		0x240003
#define		HMC_REG_LRLL1		0x250003
#define		HMC_REG_LRLL2		0x260003
#define		HMC_REG_LRLL3		0x270003
#define		HMC_REG_LR0		0x0C0000
#define		HMC_REG_LR1		0x0D0000
#define		HMC_REG_LR2		0x0E0000
#define		HMC_REG_LR3		0x0F0000
#define		HMC_REG_IBTC0		0x040000
#define		HMC_REG_IBTC1		0x050000
#define		HMC_REG_IBTC2		0x060000
#define		HMC_REG_IBTC3		0x070000
#define		HMC_REG_AC		0x2C0000
#define		HMC_REG_VCR		0x108000
#define		HMC_REG_FEAT		0x2C0003
#define		HMC_REG_RVID		0x2C0004

#define		HMC_REG_EDR0_IDX	0x000000
#define		HMC_REG_EDR1_IDX	0x000001
#define		HMC_REG_EDR2_IDX	0x000002
#define		HMC_REG_EDR3_IDX	0x000003
#define		HMC_REG_ERR_IDX		0x000004
#define		HMC_REG_GC_IDX		0x000005
#define		HMC_REG_LC0_IDX		0x000006
#define		HMC_REG_LC1_IDX		0x000007
#define		HMC_REG_LC2_IDX		0x000008
#define		HMC_REG_LC3_IDX		0x000009
#define		HMC_REG_LRLL0_IDX	0x00000A
#define		HMC_REG_LRLL1_IDX	0x00000B
#define		HMC_REG_LRLL2_IDX	0x00000C
#define		HMC_REG_LRLL3_IDX	0x00000D
#define		HMC_REG_LR0_IDX		0x00000E
#define		HMC_REG_LR1_IDX		0x00000F
#define		HMC_REG_LR2_IDX		0x000010
#define		HMC_REG_LR3_IDX		0x000011
#define		HMC_REG_IBTC0_IDX	0x000012
#define		HMC_REG_IBTC1_IDX	0x000013
#define		HMC_REG_IBTC2_IDX	0x000014
#define		HMC_REG_IBTC3_IDX	0x000015
#define		HMC_REG_AC_IDX		0x000016
#define		HMC_REG_VCR_IDX		0x000017
#define		HMC_REG_FEAT_IDX	0x000018
#define		HMC_REG_RVID_IDX	0x000019


#ifdef __cplusplus
} /* extern C */
#endif

#endif	/* _HMC_SIM_MACROS_H_ */

/* EOF */