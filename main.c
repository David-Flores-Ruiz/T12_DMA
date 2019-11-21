

#include "MK64F12.h"
#include "NVIC.h"
#include "GPIO.h"
#include "Bits.h"
#include <stdio.h>
#include "generador.h"
#include "RGB.h"
#include "PIT.h"
#include "DMA_driver.h"



int main(void)
{
	RGB_init();
	SW_init();
	PIT_init();
	DAC0_clock_gating();
	DAC0_config();

	//NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_10);
	//NVIC_enable_interrupt_and_priotity(PIT_CH1_IRQ, PRIORITY_10);
	NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ, PRIORITY_10);
	NVIC_enable_interrupt_and_priotity(PORTC_IRQ, PRIORITY_10);	// sw2
	NVIC_enable_interrupt_and_priotity(PORTA_IRQ, PRIORITY_10);	// sw3


	gpio_pin_control_register_t sw2 = GPIO_MUX1 | GPIO_PE | GPIO_PS | DMA_FALLING_EDGE; /* GPIO configured to trigger the DMA*/
	GPIO_clock_gating(GPIO_C);
	GPIO_pin_control_register(GPIO_C, bit_6, &sw2);
	DMA_clock_gating();
	DMA_init(); /* Configure the T*/
	NVIC_enable_interrupt_and_priotity(DMA_CH0_IRQ, PRIORITY_5);
	NVIC_global_enable_interrupts;


	PIT_delayFloat(PIT_2, SYSTEM_CLOCK, Delay_2ms);	// Corre el PIT de 2ms

	for (;;) {

		DAC_FSM_signals(); // Genera señal Triangular, Cuadrada y Senoidal

	}

	/* Never leave main */
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
