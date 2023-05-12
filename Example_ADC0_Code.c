/* This program converts the analog input from channel 0 (PTE20)
* using software trigger continuously.
* Bits 10-8 are used to control the tri-color LEDs. LED code is copied from p2_7.
* Connect a potentiometer between 3.3V and
* ground. The wiper of the potentiometer is connected to PTE20.
* When the potentiometer is turned, the LEDs should change color.
*/

#include <MKL46Z4.h>
#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
void ADC0_init (void);

int main(void){
short int result;

ADC0_init() ; /* Configure ADC0 */
while(1){
ADC0 -> SC1[0] = 0; /* start conversion on channel 0 */
 while(!( ADC0-> SC1[0] & 0x80 )){}/* wait for conversion complete */
result = ADC0-> R[0]; /* read conversion result and clear COCO flag */

}
}
void ADC0_init ( void )
{
SIM-> SCGC5 |= 0x2000; /* clock to PORTE */
PORTE-> PCR[16] = 0; /* PTE16 analog input */
PORTE-> PCR[20] = 0; /* PTE20 analog input */
SIM-> SCGC6 |= 0x8000000; /* clock to ADC0 */
ADC0-> SC2 &= ~ 0x40; /* software triggers */
/* clock div by 4, long sample time, single ended 12 bits, bus clock */
ADC0-> CFG1 = 0x40 | 0x10 | 0x04 | 0x00 ;
}
