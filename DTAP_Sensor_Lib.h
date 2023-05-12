#ifndef __DTAP_SENSOR_LIB_H__
#define __DTAP_SENSOR_LIB_H__

//NEED a Function to read in values from a POT
int potRead(void);
//NEED a function that converts Values read from pot into angles/PWM signals for servo
void PWM_Logic(int);
//NEED a function that initialize all of the pins that will be used by the pot and servo
void ADC_Intializer(void);
void PWM_Intializer(void);
//Only use this function if Anthony wants to use it
void PIT_IRQHandler(void);

#endif
