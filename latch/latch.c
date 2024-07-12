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
void delay(unsigned long count)
{
    while (count--)
        nop();
}

int main(void)
{
   	/*  Set clock to full speed (16 Mhz)  */
    CLK_CKDIVR = 0;
    /*  setup latching vars for BMS and IMD, when they are non-zero, it will latch the value  */
    unsigned int unlatched_bms;
    unsigned int unlatched_imd;
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
    for (int i = 0; i < INIT_DELAY_COUNT; i++)
    {
        delay(300000L);
    }

    if (PORT(BMS_IN_PORT, IDR) & BMS_IN_PIN) // this is bit masking the BMS pin with the input value array (True if high?)
    {
        PORT(BMS_OUT_PORT, ODR) |= BMS_OUT_PIN; // turn on
        unlatched_bms = 1; // bms is not lachted
    } 
    else
    {
        PORT(BMS_OUT_PORT, ODR) &= BMS_OUT_PIN; //turn off
        unlatched_bms = 0; // bms is lachted
    }

    if (PORT(IMD_OUT_PORT, IDR) & IMD_OUT_PIN) // this is bit masking the IMD pin with the input value array (True if high?)
    {
        PORT(IMD_OUT_PORT, ODR) |= IMD_OUT_PIN; // turn on
        unlatched_imd = 1;
    }
    else
    {
        PORT(IMD_OUT_PORT, ODR) &= IMD_OUT_PIN; //turn off
        unlatched_imd = 0;
    }

	while(1)
    {
        if (unlatched_bms || (PORT(LATCH_RST_PORT, IDR) & LATCH_RST_PIN))
        {
            if (PORT(BMS_IN_PORT, IDR) & BMS_IN_PIN) // this is bit masking the BMS pin with the input value array (True if high?)
            {
                PORT(BMS_OUT_PORT, ODR) |= BMS_OUT_PIN; // turn on
                unlatched_bms = 1;
            } 
            else
            {
                PORT(BMS_OUT_PORT, ODR) &= BMS_OUT_PIN; //turn off
                unlatched_bms = 0;
            }
        }
        if (unlatched_imd || (PORT(LATCH_RST_PORT, IDR) & LATCH_RST_PIN))
        {
            if (PORT(IMD_OUT_PORT, IDR) & IMD_OUT_PIN) // this is bit masking the IMD pin with the input value array (True if high?)
            {
                PORT(IMD_OUT_PORT, ODR) |= IMD_OUT_PIN; // turn on
                unlatched_imd = 1;
            }
            else
            {
                PORT(IMD_OUT_PORT, ODR) &= IMD_OUT_PIN; //turn off
                unlatched_imd = 0;
            }
        }
    }
}
