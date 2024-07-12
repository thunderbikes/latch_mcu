# latch_mcu

## About

This repo contains the code to be run on the STM8 that serves as the latch for the safety loop.

### ```blink.c```

This code is a test of the MCU. With the latched output selected, the output pins and by extension the LEDs connected to the IMD and BMS status should blink, always inverted from eachother.

### ```latch.c```

This code provides the goal functionality of the original latch circuitry. There will be a delay upon startup, to allow for the IMD and BMS signals to reach there initial position. After that the device will read the state of the ```IMD_OK``` (PA2) and ```BMS_OUT1``` (PD5) and pass that on to their respective output pins ```IMD_OUT``` (PD6) and ```BMS_OUT``` (PD4). The MCU will always allow the device to go from OK to FAULT (HIGH TO LOW on the output pins) with a change in the input pins, but will not allow the device to clear a fault (LOW TO HIGH) unless the ```LATCH_RST``` (PA1) is HIGH, in addition to the change in the IMD or BMS pin. 

