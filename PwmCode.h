

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_tpm.h"

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

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint8_t getCharValue = 0U;
volatile uint8_t updatedDutycycle = 10U;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    tpm_config_t tpmInfo;
    tpm_chnl_pwm_signal_param_t tpmParam[2];

    #ifndef TPM_LED_ON_LEVEL
      #define TPM_LED_ON_LEVEL kTPM_LowTrue
    #endif

    /* Configure tpm params with frequency 24kHZ */
    tpmParam[0].chnlNumber = (tpm_chnl_t)BOARD_FIRST_TPM_CHANNEL;
    tpmParam[0].level = kTPMLowTrue;
    tpmParam[0].dutyCyclePercent = updatedDutycycle;

    tpmParam[1].chnlNumber = (tpm_chnl_t)BOARD_SECOND_TPM_CHANNEL;
    tpmParam[1].level = TPM_LED_ON_LEVEL;
    tpmParam[1].dutyCyclePercent = updatedDutycycle;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Select the clock source for the TPM counter as kCLOCK_PllFllSelClk */
    CLOCK_SetTpmClock(1U);

    /* Print a note to terminal */
    PRINTF("\r\nTPM example to output PWM on 2 channels\r\n");
    PRINTF("\r\nIf an LED is connected to the TPM pin, you will see a change in LED brightness if you enter different values");
    PRINTF("\r\nIf no LED is connected to the TPM pin, then probe the signal using an oscilloscope");

    /*
     * tpmInfo.prescale = kTPM_Prescale_Divide_1;
     * tpmInfo.useGlobalTimeBase = false;
     * tpmInfo.enableDoze = false;
     * tpmInfo.enableDebugMode = false;
     * tpmInfo.enableReloadOnTrigger = false;
     * tpmInfo.enableStopOnOverflow = false;
     * tpmInfo.enableStartOnTrigger = false;
     * tpmInfo.enablePauseOnTrigger = false;
     * tpmInfo.triggerSelect = kTPM_Trigger_Select_0;
     * tpmInfo.triggerSource = kTPM_TriggerSource_External;
     */
    TPM_GetDefaultConfig(&tpmInfo);
    /* Initialize TPM module */
    TPM_Init(BOARD_TPM_BASEADDR, &tpmInfo);

    TPM_SetupPwm(BOARD_TPM_BASEADDR, tpmParam, 2U, kTPM_EdgeAlignedPwm, 24000U, TPM_SOURCE_CLOCK);
    TPM_StartTimer(BOARD_TPM_BASEADDR, kTPM_SystemClock);
    while (1)
    {
        do
        {

            PRINTF("For example: If enter '5', the duty cycle will be set to 50 percent.\r\n");


        } while (getCharValue > 9U);

        updatedDutycycle = getCharValue * 10U;

        //convert to percentage and multiply by 100


        /* Start PWM mode with updated duty cycle */
        TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_FIRST_TPM_CHANNEL, kTPM_EdgeAlignedPwm,
                               updatedDutycycle);
        TPM_UpdatePwmDutycycle(BOARD_TPM_BASEADDR, (tpm_chnl_t)BOARD_SECOND_TPM_CHANNEL, kTPM_EdgeAlignedPwm,
                               updatedDutycycle);

        PRINTF("The duty cycle was successfully updated!\r\n");
    }
}
