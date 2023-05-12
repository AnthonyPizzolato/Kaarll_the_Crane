#include <MKL46Z4.h>


/*
   Main program: entry point
 */

int timeLong = 0x3333; //Set interrupt times
int timeShort = 0x28F;
int timeSA=0;
int timeSB=0;
int timeSC=0;
int whichOn=1; //track which servo we are at



int main (void)
{

	NVIC_EnableIRQ(PIT_IRQn); //Enable IRQ for PIT
	NVIC_ClearPendingIRQ(PIT_IRQn); //initially clear IRQ
	SIM -> SCGC5 |= SIM_SCGC5_PORTD_MASK;
	PORTD -> PCR[5] |= PORT_PCR_MUX(001);
	PTD -> PDDR |= (1<< 5);
	PORTD -> PCR[4] |= PORT_PCR_MUX(001);
	PTD -> PDDR |= (1<< 4);
	PORTD -> PCR[2] |= PORT_PCR_MUX(001);
	PTD -> PDDR |= (1<< 2);



	SIM->SCGC6 = SIM_SCGC6_PIT_MASK; //Send clock to PIT
	PIT->MCR = ~PIT_MCR_MDIS_MASK; //Enable PIT

	PIT->CHANNEL[0].LDVAL = timeLong; //Load short time into pit timer
	PIT->CHANNEL[0].TCTRL|= PIT_TCTRL_TEN_MASK; //Enable timer
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK; //Enable timer interrupt

	PIT->CHANNEL[1].LDVAL = timeShort; //Load short time into pit timer
	PIT->CHANNEL[1].TCTRL|= PIT_TCTRL_TEN_MASK; //Enable timer
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK; //Enable timer interrupt

	while(1){
	}
}



/*
   PIT Interrupt Handler
 */
void PIT_IRQHandler(void)
{
	/* code goes here */
	if(PIT -> CHANNEL[1].TFLG == 1){
		int adcValueA=4095;
		int adcValueB=3000;
		int adcValueC=1000;
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
