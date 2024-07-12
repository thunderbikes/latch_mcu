// This is stolen from stm8-sedcc-examples/blink

/* The "Hello world!" of microcontrollers. Blink LED on/off */
#include <stdint.h>
#include "stm8.h"


#define BMS_OUT_PORT    PD
#define BMS_OUT_PIN     PIN4
#define IMD_OUT_PORT	PD
#define IMD_OUT_PIN     PIN6


/* Simple busy loop delay */
void delay(unsigned long count) {
    while (count--)
        nop();
}

int main(void)
{
   	/* Set clock to full speed (16 Mhz) */
    CLK_CKDIVR = 0;
    /* GPIO setup */
    // Set pin data direction as output
    PORT(BMS_OUT_PORT, DDR)  |= BMS_OUT_PIN; // i.e. PB_DDR |= (1 << 5);
    // Set pin as "Push-pull"
	PORT(BMS_OUT_PORT, CR1)  |= BMS_OUT_PIN; // i.e. PB_CR1 |= (1 << 5);

    // now the same for IMD_OUT
    PORT(IMD_OUT_PORT, DDR) |= IMD_OUT_PIN;
    PORT(IMD_OUT_PORT, CR1) |= IMD_OUT_PIN;

	while(1) {
        PORT(BMS_OUT_PORT, ODR) |= BMS_OUT_PIN; // turn on
        PORT(IMD_OUT_PORT, ODR) &= IMD_OUT_PIN; //turn off

        delay(300000L); // i don't know if we can know how long this delay is without more info

        PORT(BMS_OUT_PORT, ODR) &= ~BMS_OUT_PIN; // PB_ODR &= ~(1 << 5);
        PORT(IMD_OUT_PORT, ODR) |= IMD_OUT_PIN;

        delay(300000L);
    }
}
