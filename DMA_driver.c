/*
 * DMA_driver.c
 *
 *  Created on: Nov 20, 2019
 *      Author: David Ruiz
 */

#include "DMA_driver.h"
#include "MK64F12.h"
#include <stdio.h>


#define NUM_STEPS (1u)
// Valores Originales:
//#define NUM_STEPS (2u)
//#define SYSTEM_CLOCK (21000000u)
#define DELAY (0.01F)

#define ARRAY_SIZE (16u)

#define DMA_CH0 (0x01u)
#define DMA_CH2 (0x04u)
#define DMA_SOURCE_GPIO (51u)
#define DMA_SOURCE_MUX (63u) //** ALWAYS ENABLED */

uint16_t g_data_source[ARRAY_SIZE] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};//defines source data space
uint16_t g_data_desti[4*ARRAY_SIZE]; //defines destination data space


void DMA0_IRQHandler(void)	//** ISR del DMA channel 0 */
{

	uint8_t i;

	DMA0->INT = DMA_CH0;

	for ( i = 0; i < ARRAY_SIZE; ++i)
	{
		printf("%d,",g_data_desti[i]);
	}
	printf("\n");
}

void DMA2_IRQHandler(void)	//** ISR del DMA channel 2 */
{

	uint8_t i;

	DMA0->INT = DMA_CH2;

	for ( i = 0; i < ARRAY_SIZE; ++i)
	{
		printf("%d,",g_data_desti[i]);
	}
	printf("\n");
}

void DMA_clock_gating(void)
{
	SIM->SCGC7 |= SIM_SCGC7_DMA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
}


void DMA_init(void)
{

	DMAMUX->CHCFG[0] = 0;
	DMAMUX->CHCFG[0] = DMAMUX_CHCFG_ENBL_MASK | /*enables DMA MUX channel*/
					   DMAMUX_CHCFG_SOURCE(DMA_SOURCE_GPIO);/*source is FTM0 channel 0*/

	DMA0->ERQ = 0x01;//enables DMA0 request

	DMA0->TCD[0].SADDR = (uint32_t)(&g_data_source[0]);/*defines source data address*/
	DMA0->TCD[0].SOFF = 1;/*Source address signed offset;it is expressed in number of bytes*/
	DMA0->TCD[0].DADDR = (uint32_t)(&g_data_desti[0]);/*defines destination data address*/
	DMA0->TCD[0].DOFF = 1;/*destination address signed offset;it is expressed in number of bytes*/
	/*CITER represents the number of minor loops that compose a major loop, every time a minor loop is completed CITTER is decremented by one.
	 * Once CITTER is 0 the DMA copy BITTER into CITTER and adjust SADDR and DADDR with the values specified in SLAST and DLAST_SGA respectively
	 *  SADDR  = SADDR + SLAST and DADDR + DLAST_SGA*/
	DMA0->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(NUM_STEPS);
	/* Once a major loop is completed, BITTER is copy to CITTER*/
	DMA0->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(NUM_STEPS);
	DMA0->TCD[0].NBYTES_MLNO = 8;/*byte number*/

	DMA0->TCD[0].ATTR = 0;/*8 bit transfer size, in order to transfer see Kinetis user manual*/
	DMA0->TCD[0].SLAST = -6;//restores the source address to the initial value, which is expressed in the amount of bytes to restore*/
	DMA0->TCD[0].DLAST_SGA = 0;/*restores the destination address to the initial value, which is expressed in the amount of bytes to restore*/
	DMA0->TCD[0].CSR = DMA_CSR_INTMAJOR_MASK;/*The end-of-major loop interrupt is enabled*/
}


void DMA_init_P1_1(void)
{

	DMAMUX->CHCFG[0] = 0;
	DMAMUX->CHCFG[0] = DMAMUX_CHCFG_ENBL_MASK | /*enables DMA MUX channel*/
					   DMAMUX_CHCFG_SOURCE(DMA_SOURCE_GPIO);/*source is FTM0 channel 0*/

	DMA0->ERQ = 0x01;//enables DMA0 request

	DMA0->TCD[0].SADDR = (uint32_t)(&g_data_source[0]);/*defines source data address*/
	DMA0->TCD[0].SOFF = 1;/*Source address signed offset;it is expressed in number of bytes*/
	DMA0->TCD[0].DADDR = (uint32_t)(&g_data_desti[0]);/*defines destination data address*/
	DMA0->TCD[0].DOFF = 1;/*destination address signed offset;it is expressed in number of bytes*/
	/*CITER represents the number of minor loops that compose a major loop, every time a minor loop is completed CITTER is decremented by one.
	 * Once CITTER is 0 the DMA copy BITTER into CITTER and adjust SADDR and DADDR with the values specified in SLAST and DLAST_SGA respectively
	 *  SADDR  = SADDR + SLAST and DADDR + DLAST_SGA*/
	DMA0->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(NUM_STEPS);
	/* Once a major loop is completed, BITTER is copy to CITTER*/
	DMA0->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(NUM_STEPS);
	DMA0->TCD[0].NBYTES_MLNO = 32;/*byte number*/

	DMA0->TCD[0].ATTR = 0;/*8 bit transfer size, in order to transfer see Kinetis user manual*/
	DMA0->TCD[0].SLAST = -6;//restores the source address to the initial value, which is expressed in the amount of bytes to restore*/
	DMA0->TCD[0].DLAST_SGA = 0;/*restores the destination address to the initial value, which is expressed in the amount of bytes to restore*/
	DMA0->TCD[0].CSR = DMA_CSR_INTMAJOR_MASK;/*The end-of-major loop interrupt is enabled*/
}

void DMA_init_P1_2(void)
{

	DMAMUX->CHCFG[0] = 0;
	DMAMUX->CHCFG[0] = DMAMUX_CHCFG_ENBL_MASK | /*enables DMA MUX channel*/
					   DMAMUX_CHCFG_SOURCE(DMA_SOURCE_GPIO);/*source is FTM0 channel 0*/

	DMA0->ERQ = 0x01;//enables DMA0 request

	DMA0->TCD[0].SADDR = (uint32_t)(&g_data_source[0]);/*defines source data address*/
	DMA0->TCD[0].SOFF = 1;/*Source address signed offset;it is expressed in number of bytes*/
	DMA0->TCD[0].DADDR = (uint32_t)(&g_data_desti[0]);/*defines destination data address*/
	DMA0->TCD[0].DOFF = 1;/*destination address signed offset;it is expressed in number of bytes*/
	/*CITER represents the number of minor loops that compose a major loop, every time a minor loop is completed CITTER is decremented by one.
	 * Once CITTER is 0 the DMA copy BITTER into CITTER and adjust SADDR and DADDR with the values specified in SLAST and DLAST_SGA respectively
	 *  SADDR  = SADDR + SLAST and DADDR + DLAST_SGA*/
	DMA0->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(NUM_STEPS);
	/* Once a major loop is completed, BITTER is copy to CITTER*/
	DMA0->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(NUM_STEPS);
	DMA0->TCD[0].NBYTES_MLNO = 16;/*byte number*/

	DMA0->TCD[0].ATTR = 0;/*8 bit transfer size, in order to transfer see Kinetis user manual*/
	DMA0->TCD[0].SLAST = -6;//restores the source address to the initial value, which is expressed in the amount of bytes to restore*/
	DMA0->TCD[0].DLAST_SGA = 0;/*restores the destination address to the initial value, which is expressed in the amount of bytes to restore*/
	DMA0->TCD[0].CSR = DMA_CSR_INTMAJOR_MASK;/*The end-of-major loop interrupt is enabled*/
}

void DMA_init_P1_3(void)
{

	DMAMUX->CHCFG[0] = 0;
	DMAMUX->CHCFG[0] = DMAMUX_CHCFG_ENBL_MASK | /*enables DMA MUX channel*/
					   DMAMUX_CHCFG_SOURCE(DMA_SOURCE_GPIO);/*source is FTM0 channel 0*/

	DMA0->ERQ = 0x01;//enables DMA0 request

	DMA0->TCD[0].SADDR = (uint32_t)(&g_data_source[4]);/*defines source data address*/
	DMA0->TCD[0].SOFF = 4;/*Source address signed offset;it is expressed in number of bytes*/
	DMA0->TCD[0].DADDR = (uint32_t)(&g_data_desti[0]);/*defines destination data address*/
	DMA0->TCD[0].DOFF = 1;/*destination address signed offset;it is expressed in number of bytes*/
	/*CITER represents the number of minor loops that compose a major loop, every time a minor loop is completed CITTER is decremented by one.
	 * Once CITTER is 0 the DMA copy BITTER into CITTER and adjust SADDR and DADDR with the values specified in SLAST and DLAST_SGA respectively
	 *  SADDR  = SADDR + SLAST and DADDR + DLAST_SGA*/
	DMA0->TCD[0].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(NUM_STEPS);
	/* Once a major loop is completed, BITTER is copy to CITTER*/
	DMA0->TCD[0].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(NUM_STEPS);
	DMA0->TCD[0].NBYTES_MLNO = 6;/*byte number*/

	DMA0->TCD[0].ATTR = 0x100;/*8 bit transfer size, in order to transfer see Kinetis user manual*/
	DMA0->TCD[0].SLAST = -20;//restores the source address to the initial value, which is expressed in the amount of bytes to restore*/
	DMA0->TCD[0].DLAST_SGA = 0;/*restores the destination address to the initial value, which is expressed in the amount of bytes to restore*/
	DMA0->TCD[0].CSR = DMA_CSR_INTMAJOR_MASK;/*The end-of-major loop interrupt is enabled*/
}

extern const uint16_t DAC_SquareWaveForm[100];
extern const uint16_t DAC_SineWaveForm[100];
extern const uint16_t DAC_TriangleForm[100];

void DMA_init_P2_SEN(void)
{

	DMAMUX->CHCFG[2] = 0;
	DMAMUX->CHCFG[2] = DMAMUX_CHCFG_ENBL_MASK | /*enables DMA MUX channel*/
					   DMAMUX_CHCFG_TRIG_MASK |
					   DMAMUX_CHCFG_SOURCE(DMA_SOURCE_MUX);/*source is FTM0 channel 0*/

	DMA0->ERQ = 0x01;//enables DMA0 request


	DMA0->TCD[2].SADDR = (uint32_t)(&DAC_SquareWaveForm[0]);/*defines source data address*/
	DMA0->TCD[2].SOFF = 1;/*Source address signed offset;it is expressed in number of bytes*/
	DMA0->TCD[2].DADDR = (uint32_t)(&g_data_desti[0]);/*defines destination data address*/
	DMA0->TCD[2].DOFF = 0;/*destination address signed offset;it is expressed in number of bytes*/
	/*CITER represents the number of minor loops that compose a major loop, every time a minor loop is completed CITTER is decremented by one.
	 * Once CITTER is 0 the DMA copy BITTER into CITTER and adjust SADDR and DADDR with the values specified in SLAST and DLAST_SGA respectively
	 *  SADDR  = SADDR + SLAST and DADDR + DLAST_SGA*/
	DMA0->TCD[2].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(NUM_STEPS);
	/* Once a major loop is completed, BITTER is copy to CITTER*/
	DMA0->TCD[2].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(NUM_STEPS);
	DMA0->TCD[2].NBYTES_MLNO = 2;/*byte number*/

	DMA0->TCD[2].ATTR = 0x101;/*8 bit transfer size, in order to transfer see Kinetis user manual*/
	DMA0->TCD[2].SLAST = 0;//restores the source address to the initial value, which is expressed in the amount of bytes to restore*/
	DMA0->TCD[2].DLAST_SGA = 0;/*restores the destination address to the initial value, which is expressed in the amount of bytes to restore*/
	DMA0->TCD[2].CSR = DMA_CSR_INTMAJOR_MASK;/*The end-of-major loop interrupt is enabled*/
}

