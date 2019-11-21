/*
 * DMA_driver.h
 *
 *  Created on: Nov 20, 2019
 *      Author: David Ruiz
 */

#ifndef DMA_DRIVER_H_
#define DMA_DRIVER_H_

#define SYSTEM_CLOCK (21000000u)

void DMA0_IRQHandler(void);

void DMA_clock_gating(void);

void DMA_init(void);


#endif /* DMA_DRIVER_H_ */
