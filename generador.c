/*
 * @file:			generador.c
 * @company:		ITESO
 * @Engineer Team:	D.F.R. / R.G.P.
 */

#include "generador.h"
#include "MK64F12.h"
#include "PIT.h"
#include "RGB.h"
#include "GPIO.h"


#define SQUARE   &FSM_MooreWave[0]
#define SENOIDAL &FSM_MooreWave[1]
#define TRIANGLE &FSM_MooreWave[2]


const uint16_t DAC_SquareWaveForm [MAX] =
{
		0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,
		0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,
		0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,
		0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,
		0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,0x0c80,
		0x0,   0x0,	  0x0,   0x0,   0x0,   0x0,	  0x0,   0x0,	0x0,   0x0,
		0x0,   0x0,	  0x0,   0x0,   0x0,   0x0,	  0x0,   0x0,	0x0,   0x0,
		0x0,   0x0,	  0x0,   0x0,   0x0,   0x0,	  0x0,   0x0,	0x0,   0x0,
		0x0,   0x0,	  0x0,   0x0,   0x0,   0x0,	  0x0,   0x0,	0x0,   0x0,
		0x0,   0x0,	  0x0,   0x0,   0x0,   0x0,	  0x0,   0x0,	0x0,   0x0
};

const uint16_t DAC_SineWaveForm[MAX] =
{
		0x03e8,0x0427,0x0465,0x04a3,0x04e1,0x051d,0x0558,0x0592,0x05ca,0x0600,
		0x0634,0x0665,0x0695,0x06c1,0x06eb,0x0711,0x0734,0x0754,0x0771,0x078a,
		0x079f,0x07b1,0x07be,0x07c8,0x07ce,0x07d0,0x07ce,0x07c8,0x07be,0x07b1,
		0x079f,0x078a,0x0771,0x0754,0x0734,0x0711,0x06eb,0x06c1,0x0695,0x0665,
		0x0634,0x0600,0x05ca,0x0592,0x0558,0x051d,0x04e1,0x04a3,0x0465,0x0427,
		0x03e8,0x03a9,0x036b,0x032d,0x02ef,0x02b3,0x0278,0x023e,0x0206,0x01d0,
		0x019c,0x016b,0x013b,0x010f,0x00e5,0x00bf,0x009c,0x007c,0x005f,0x0046,
		0x0031,0x001f,0x0012,0x0008,0x0002,0x0,   0x0002,0x0008,0x0012,0x001f,
		0x0031,0x0046,0x005f,0x007c,0x009c,0x00bf,0x00e5,0x010f,0x013b,0x016b,
		0x019c,0x01d0,0x0206,0x023e,0x0278,0x02b3,0x02ef,0x032d,0x036b,0x03a9
};

const uint16_t DAC_TriangleForm [MAX] =
{
		0x0028,0x0050,0x0078,0x00a0,0x00c8,0x00f0,0x0118,0x0140,0x0168,0x0190,
		0x01b8,0x01e0,0x0208,0x0230,0x0258,0x0280,0x02a8,0x02d0,0x02f8,0x0320,
		0x0348,0x0370,0x0398,0x03c0,0x03e8,0x0410,0x0438,0x0460,0x0488,0x04b0,
		0x04d8,0x0500,0x0528,0x0550,0x0578,0x05a0,0x05c8,0x05f0,0x0618,0x0640,
		0x0668,0x0690,0x06b8,0x06e0,0x0708,0x0730,0x0758,0x0780,0x07a8,0x07d0,
		0x07a8,0x0780,0x0758,0x0730,0x0708,0x06e0,0x06b8,0x0690,0x0668,0x0640,
		0x0618,0x05f0,0x05c8,0x05a0,0x0578,0x0550,0x0528,0x0500,0x04d8,0x04b0,
		0x0488,0x0460,0x0438,0x0410,0x03e8,0x03c0,0x0398,0x0370,0x0348,0x0320,
		0x02f8,0x02d0,0x02a8,0x0280,0x0258,0x0230,0x0208,0x01e0,0x01b8,0x0190,
		0x0168,0x0140,0x0118,0x00f0,0x00c8,0x00a0,0x0078,0x0050,0x0028,0x0
};


typedef struct
{
	uint32_t out;
	uint16_t* wave;
	const struct StateWave* next[3];
}StateWave_t;

const StateWave_t FSM_MooreWave[3] = {
	{ BLUE_ON,  DAC_SquareWaveForm, { SQUARE, SENOIDAL, TRIANGLE } }, /* blue  */
	{ RED_ON,   DAC_SineWaveForm,	{ SQUARE, SENOIDAL, TRIANGLE } }, /* red   */
	{ GREEN_ON, DAC_TriangleForm,	{ SQUARE, SENOIDAL, TRIANGLE } }  /* green */
};

void DAC0_clock_gating( ){
	/** DAC: Configurar el DAC-12 bits para que inicie sin buffer 		  */
	SIM->SCGC2 |= SIM_SCGC2_DAC0_MASK;	//SIM_SCGC6_DAC0_MASK;	/** Enable clock DAC0: Bit 9 of SIM_SCGC5 is  set   */
}

void DAC0_config( ){
	DAC0->C0 |= DAC_C0_DACEN_MASK; /**  Habilita canal 0 del DAC 		  */
	DAC0->C0 |= DAC_C0_DACRFS_MASK;/** Voltaje de referencia interno de la k64 */
	DAC0->C0 |= DAC_C0_LPEN_MASK;  /** Low-Power Mode is selected		  */
}

void Wait_2ms() {
//	uint8_t estado = FALSE;
//	PIT_delayFloat(PIT_2, SYSTEM_CLOCK, Delay_2ms);	// Corre el PIT
//	estado = PIT_get_irq_flag_status(PIT_2);
//	if (estado == TRUE) {
//		PIT_clear_irq_flag_status(PIT_2);	// Limpiamos bandera de Software
//		PIT_stop(PIT_2);					// Paramos el PIT
//	}
}

void DAC_FSM_signals(void) {
	static StateWave_t* current_state = SQUARE;
	color_ON output = 0;
	uint16_t waveElement = 0;
	uint8_t statusINT_sw3 = 0;
	static int8_t sw3_veces = -1;
	static uint16_t indexElemento = 0;

	statusINT_sw3 = GPIO_get_irq_status(GPIO_A);
	if (statusINT_sw3) {
		sw3_veces++;
		GPIO_clear_irq_status(GPIO_A);
		indexElemento = CERO_N;
	}

	if (sw3_veces == 3) {
		sw3_veces = CERO_N;
	}

	if (indexElemento == MAX){
		indexElemento = CERO_N;
	}

	if (sw3_veces != -1) {
		output = current_state->out;
		apagar_LED(output);
		current_state = current_state->next[sw3_veces];
		output = current_state->out;
		encender_LED(output);

		uint8_t estadoPIT_2 = FALSE;
		estadoPIT_2 = PIT_get_irq_flag_status(PIT_2);
		if (estadoPIT_2 == TRUE) 	//** Delay para tiempo de muestreo */
		{
			if ((indexElemento >= 0) && (indexElemento < MAX)) {
				waveElement = current_state->wave[indexElemento];
				DAC_plot(waveElement);
			}
			PIT_clear_irq_flag_status(PIT_2);	// Limpiamos bandera de Software
			indexElemento++;//** Inyectar siguiente elemento del arreglo de forma correspondiente */
		}

//		for(indexElemento = 0; indexElemento<MAX; indexElemento++){
//			waveElement = current_state->wave[indexElemento];
//			Wait_2ms();	//** Delay para tiempo de muestreo */
//			DAC_plot(waveElement);
//		}
//		indexElemento++;//** Inyectar siguiente elemento del arreglo de forma correspondiente */

	} // end if: si no se presiono sw3
}

void DAC_plot(uint16_t elemento)
{
	uint16_t data_Low  = (elemento) & (0x00FF);
	uint16_t data_High = (elemento >> 8) & (0x000F);

	DAC0->DAT[0].DATL = data_Low;
	DAC0->DAT[0].DATH = data_High;
}
