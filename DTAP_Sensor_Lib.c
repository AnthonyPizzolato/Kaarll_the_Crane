#include <MKL46Z4.h>
#include "DTAP_Sensor_Lib.h"
#include "fsl_debug_console.h"
#include "board.h"
//#include "fsl_tpm.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* The Flextimer instance/channel used for board */
#define BOARD_TPM_BASEADDR TPM0
#define BOARD_FIRST_TPM_CHANNEL 2U
#define BOARD_SECOND_TPM_CHANNEL 5U

/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_PllFllSelClk)

const int analog_PIN0 = 20;
const int analog_PIN1 = 18;
const int analog_PIN2 = 16;


//const int TimeLong = 0x3333; //Set interrupt times
//const int TimeShort = 0x28F;
//int timeSA=0;
//int timeSB=0;
//int timeSC=0;

void ADC_Intializer(){
	//This function initializes the ADC needed to convert the signal read from the pot
//	SIM -> SCGC5 |= SIM_SCGC5_PORTE_MASK;
//	PORTE -> PCR[analog_PIN0] |= (000 << 8); //This pin is for the base of the crane
//	PORTE -> PCR[analog_PIN1] |= (000 << 8); //This pin is for the joint of the crane
//	PORTE -> PCR[analog_PIN2] |= (000 << 8); //This pin is for the claw of the crane
//	/*The first four lines enable the clock for the port and pin of the ADC pin*/
//	SIM -> SCGC6 |= (1 << 27); /*Enables clock for ADC0*/
//	ADC0 -> SC2 |= (0 << 6);  /*This tells the ADC that we will be using software triggers*/
//	ADC0 -> SC2 |= (00 << 0); /*This lets Vref(+) and Vref(-) be reference pins*/
//	ADC0 -> CFG1 |= (0x2 << 5); /* clock div by 4, */
//	ADC0 -> CFG1 |= (0x3 << 2); //This makes the resolution to be 16 bits
//	ADC0 -> CFG1 |= (00 << 0); //Makes it use the bus clock
//	ADC0 -> CFG1 |= (1 << 4); //long sample time,
	SIM-> SCGC5 |= 0x2000; /* clock to PORTE */
	PORTE-> PCR[16] = 0; /* PTE16 analog input */
	PORTE-> PCR[20] = 0; /* PTE20 analog input */
	SIM-> SCGC6 |= 0x8000000; /* clock to ADC0 */
	ADC0-> SC2 &= ~ 0x40; /* software triggers */
	/* clock div by 4, long sample time, single ended 12 bits, bus clock */
	ADC0-> CFG1 = 0x40 | 0x10 | 0x04 | 0x00 ;
}

void PWM_Intializer(void){
	//THis function initializes the PWM pins needed for the servos
	SIM -> SCGC5 |= SIM_SCGC5_PORTD_MASK;
	PORTD -> PCR[5] |= PORT_PCR_MUX(001);
	PTD -> PDDR |= (1<< 5);
	PORTD -> PCR[4] |= PORT_PCR_MUX(001);
	PTD -> PDDR |= (1<< 4);
	PORTD -> PCR[2] |= PORT_PCR_MUX(001);
	PTD -> PDDR |= (1<< 2);

	NVIC_EnableIRQ(PIT_IRQn); //Enable IRQ for PIT
	NVIC_ClearPendingIRQ(PIT_IRQn); //initially clear IRQ
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; //Send clock to PIT
	PIT->MCR = ~PIT_MCR_MDIS_MASK; //Enable PIT
	const int TimeLong = 0x3333; //Set interrupt times
	const int TimeShort = 0x28F;

	PIT->CHANNEL[0].LDVAL = TimeLong; //Load short time into pit timer
	PIT->CHANNEL[0].TCTRL|= PIT_TCTRL_TEN_MASK; //Enable timer
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK; //Enable timer interrupt

	PIT->CHANNEL[1].LDVAL = TimeShort; //Load short time into pit timer
	PIT->CHANNEL[1].TCTRL|= PIT_TCTRL_TEN_MASK; //Enable timer
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK; //Enable timer interrupt


}

int potRead(void){
	while (!(ADC0-> SC1[0] & ADC_SC1_COCO_MASK)){} /* wait for conversion complete */
	short int result = ADC0-> R[0];
	return result;
	}

void PWM_Logic(int digital_signal){

}

//void PIT_IRQHandler(void)
//{
//	/* code goes here */
//	if(PIT -> CHANNEL[1].TFLG == 1){
//		int adcValueA=4095;
//		int adcValueB=3000;
//		int adcValueC=1000;
//		timeSA=(int)((adcValueA/4095.0) *timeShort)+timeShort*1.5;
//		timeSB=(int)((adcValueB/4095.0) *timeShort)+timeShort*1.5;
//		timeSC=(int)((adcValueC/4095.0) *timeShort)+timeShort*1.5;
//		if(whichOn==1){
//			//turn servo A off
//			PTD -> PCOR |= GPIO_PCOR_PTCO(1 << 5);
//			//turn servo B on
//			PTD -> PSOR |= GPIO_PCOR_PTCO(1 << 4);
//			PIT->CHANNEL[1].LDVAL = timeSB;
//			whichOn++;
//		}
//		else if(whichOn==2){
//			//turn servo B off
//			PTD -> PCOR |= GPIO_PCOR_PTCO(1 << 4);
//
//			//turn servo /c on
//			PTD -> PSOR |= GPIO_PCOR_PTCO(1 << 2);
//			PIT->CHANNEL[1].LDVAL = timeSC;
//			whichOn++;
//		}
//		else{
//			//turn off servo C
//			PTD -> PCOR |= GPIO_PCOR_PTCO(1 << 2);
//			whichOn++;
//
//		}
//
//	}
//	if(PIT -> CHANNEL[0].TFLG == 1){
//		PIT->CHANNEL[0].LDVAL = timeLong;
//		PIT->CHANNEL[1].LDVAL = timeSA;
//		//set servo A on
//		PTD -> PSOR |= GPIO_PCOR_PTCO(1 << 5);
//		whichOn=1;
//
//
//	}
//	NVIC_ClearPendingIRQ(PIT_IRQn);
//	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
//	PIT->CHANNEL[1].TFLG = PIT_TFLG_TIF_MASK;
//
//}




