#include <MKL46Z4.h>
#include "DTAP_Sensor_Lib.h"

int timeSA=0;
int timeSB=0;
int timeSC=0;
const int timeLong = 0x3333; //Set interrupt times
const int timeShort = 0x28F;
int whichOn=1;
int pot_On;

int adcValueA;
int adcValueB;
int adcValueC;

int main(void){
	//short int result;
	PWM_Intializer();
	ADC_Intializer();

	while(1){
		//Maybe disable the Pit timer when the conversion starts then enable it again?
		//NVIC_DisableIRQ(PIT_IRQn);
		if (pot_On == 0){
			ADC0 -> SC1[0] = 0; /* start conversion on PTE20 pin */
			short int result0 = potRead();
			adcValueA = result0; //Places the digital signal into Load value for servo A
			pot_On++;
		}
		else if (pot_On == 1){
			ADC0 -> SC1[0] = 1; /* start conversion on PTE16 pin */
			short int result1 = potRead(); //Outputs a digital signal
			adcValueB = result1; //Places the digital signal into Load value for servo B
			pot_On++;
		}
		else{
			ADC0 -> SC1[0] = 2; /* start conversion on PTE18 pin */
			short int result2 = potRead();
			adcValueC = result2; //Places the digital signal into Load value for servo C
			pot_On = 0;
		}
//		ADC0 -> SC1[0] = 0;
//		short int result = potRead();
//		adcValueA = result;
		//NVIC_EnableIRQ(PIT_IRQn);

	}

}

void PIT_IRQHandler(void)
{
	/* code goes here */
	if(PIT -> CHANNEL[1].TFLG == 1){

		timeSA=(int)((adcValueA/4095.0) *timeShort)+timeShort*1.5;
		timeSB=(int)((adcValueB/4095.0) *timeShort)+timeShort*1.5;
		timeSC=(int)((adcValueC/4095.0) *timeShort)+timeShort*1.5;
		if(whichOn==1){
			//turn servo A off
			PTD -> PCOR |= GPIO_PCOR_PTCO(1 << 5);
			//turn servo B on
			PTD -> PSOR |= GPIO_PCOR_PTCO(1 << 4);
			PIT->CHANNEL[1].LDVAL = timeSB;
			whichOn++;
		}
		else if(whichOn==2){
			//turn servo B off
			PTD -> PCOR |= GPIO_PCOR_PTCO(1 << 4);

			//turn servo /c on
			PTD -> PSOR |= GPIO_PCOR_PTCO(1 << 2);
			PIT->CHANNEL[1].LDVAL = timeSC;
			whichOn++;
		}
		else{
			//turn off servo C
			PTD -> PCOR |= GPIO_PCOR_PTCO(1 << 2);
			whichOn++;

		}

	}
	if(PIT -> CHANNEL[0].TFLG == 1){
		PIT->CHANNEL[0].LDVAL = timeLong;
		PIT->CHANNEL[1].LDVAL = timeSA;
		//set servo A on
		PTD -> PSOR |= GPIO_PCOR_PTCO(1 << 5);
		whichOn=1;


	}
	NVIC_ClearPendingIRQ(PIT_IRQn);
	PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
	PIT->CHANNEL[1].TFLG = PIT_TFLG_TIF_MASK;

}
