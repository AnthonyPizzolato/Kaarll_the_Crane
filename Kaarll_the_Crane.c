#include <MKL46Z4.h>
#include "DTAP_Sensor_Lib.h"

int main(void){
	ADC_Intializer();
	//PWM_Intializer();
	while(1){
		ADC0 -> SC1[0] = 0; /* start conversion on channel 0 */
		int result = potRead();
		//PWM_Logic(result);

	}

}
