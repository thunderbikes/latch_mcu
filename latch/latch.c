#include <stdint.h>
#include "stm8.h"

#define INIT_DELAY_COUNT    5

/*  INPUTS  */
#define BMS_IN_PORT         PD
#define BMS_IN_PIN          PIN5
#define IMD_IN_PORT         PA
#define IMD_IN_PIN          PIN2

#define LATCH_RST_PORT      PA
#define LATCH_RST_PIN       PIN1


/*  OUTPUTS  */
#define BMS_OUT_PORT        PD
#define BMS_OUT_PIN         PIN4
#define IMD_OUT_PORT	    PD
#define IMD_OUT_PIN         PIN6


/* Simple busy loop delay */
void delay(unsigned long count) {
    while (count--)
        nop();
}

int main(void)
{
   	/*  Set clock to full speed (16 Mhz)  */
    CLK_CKDIVR = 0;
    /*  Setup OUTPUTS */
    /* GPIO setup */
    // Set pin data direction as output
    PORT(BMS_OUT_PORT, DDR)  |= BMS_OUT_PIN; // i.e. PB_DDR |= (1 << 5);
    PORT(IMD_OUT_PORT, DDR) |= IMD_OUT_PIN;

    // Set pin as "Push-pull"
	PORT(BMS_OUT_PORT, CR1)  |= BMS_OUT_PIN; // i.e. PB_CR1 |= (1 << 5);
    PORT(IMD_OUT_PORT, CR1) |= IMD_OUT_PIN;


    /*  Setup INPUTS  */
    // Set pin data direction as input
    PORT(BMS_IN_PORT, IDR) |= BMS_IN_PIN;
    PORT(IMD_IN_PORT, IDR) |= IMD_IN_PIN;
    PORT(LATCH_RST_PORT, IDR) |= LATCH_RST_PIN;

    /*
     Based on the STM8S Series Reference Manual it seems like the default mode (CR1 and CR2 set to 0) is Floating without interupt, which is what we want.
    */

    /*  Initial delay for letting the BMS and IMD reach steady state */
    for (int i = 0; i < INIT_DELAY_COUNT; i++){
        delay(300000L);
    }

	while(1) {
        PORT(BMS_OUT_PORT, ODR) |= BMS_OUT_PIN; // turn on
        PORT(IMD_OUT_PORT, ODR) &= IMD_OUT_PIN; //turn off

        delay(300000L); // i don't know if we can know how long this delay is without more info

        PORT(BMS_OUT_PORT, ODR) &= ~BMS_OUT_PIN; // PB_ODR &= ~(1 << 5);
        PORT(IMD_OUT_PORT, ODR) |= IMD_OUT_PIN;

        delay(300000L);
    }
}
