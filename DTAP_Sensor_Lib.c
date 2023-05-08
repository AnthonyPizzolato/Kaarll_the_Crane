#include <MKL46Z4.h>
#include "DTAP_Sensor_Lib.h"
const int analog_PIN0 = 20;
const int analog_PIN1 = 18;
const int analog_PIN2 = 16;

void ADC_Intializer(){
	//This function initializes the ADC needed to convert the signal read from the pot
	SIM -> SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE -> PCR[analog_PIN0] |= (000 << 8); //This pin is for the base of the crane
	PORTE -> PCR[analog_PIN1] |= (000 << 8); //This pin is for the joint of the crane
	PORTE -> PCR[analog_PIN2] |= (000 << 8); //This pin is for the claw of the crane
	/*The first four lines enable the clock for the port and pin of the ADC pin*/
	SIM -> SCGC6 |= (1 << 27); /*Enables clock for ADC0*/
	ADC0 -> SC2 |= (0 << 6);  /*This tells the ADC that we will be using software triggers*/
	ADC0 -> SC2 |= (00 << 0); /*This lets Vref(+) and Vref(-) be reference pins*/
	ADC0 -> CFG1 |= (0x2 << 5); /* clock div by 4, */
	ADC0 -> CFG1 |= (0x3 << 2); //This makes the resolution to be 16 bits
	ADC0 -> CFG1 |= (00 << 0); //Makes it use the bus clock
	ADC0 -> CFG1 |= (1 << 4); //long sample time,
}

void PWM_Intializer(){
	//THis function initializes the PWM pins needed for the servos
	SIM -> SCGC6 |= (1 << 24);


}

int potRead(void){
	int result;
	while (!(ADC0-> SC1[0] & ADC_SC1_COCO_MASK)){} /* wait for conversion complete */
	result = ADC0-> R[0];
	return result;
	}

void PWM_Logic(int digital_signal){

}
