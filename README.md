# latch_mcu

## About

This repo contains the code to be run on the STM8S003F3 that serves as the latch for the safety loop.

### ```blink.c```

This code is a test of the MCU. With the latched output selected, the output pins and by extension the LEDs connected to the IMD and BMS status should blink, always inverted from eachother.

### ```input_test.c```

This code is a test of my approach to reading the value of input pins. The current code will have the MCU act fully transparent, such that anything on the input pins is seen on the output pins, and the ```LATCH_RST``` pin is ignored.

```asm
00108$:
;	input_test.c: 52: if (PORT(BMS_IN_PORT, IDR) & BMS_IN_PIN) // this is bit masking the BMS pin with the input value array (True if high?)
	ld	a, 0x5010
	ld	xl, a
;	input_test.c: 54: PORT(BMS_OUT_PORT, ODR) |= BMS_OUT_PIN; // turn on
	ld	a, 0x500f
;	input_test.c: 52: if (PORT(BMS_IN_PORT, IDR) & BMS_IN_PIN) // this is bit masking the BMS pin with the input value array (True if high?)
	push	a
	ld	a, xl
	bcp	a, #0x20
	pop	a
	jreq	00102$
;	input_test.c: 54: PORT(BMS_OUT_PORT, ODR) |= BMS_OUT_PIN; // turn on
	or	a, #0x10
	ld	0x500f, a
	jra	00103$
00102$:
;	input_test.c: 58: PORT(BMS_OUT_PORT, ODR) &= BMS_OUT_PIN; //turn off
	and	a, #0x10
	ld	0x500f, a
00103$:
```

Based on this snippet of the while loop, I think it's doing what I'd expect.

### ```latch.c```

This code provides the goal functionality of the original latch circuitry. There will be a delay upon startup, to allow for the IMD and BMS signals to reach there initial position. After that the device will read the state of the ```IMD_IN``` (PA2) and ```BMS_IN``` (PD5) and pass that on to their respective output pins ```IMD_OUT``` (PD6) and ```BMS_OUT``` (PD4). The MCU will always allow the device to go from OK to FAULT (HIGH TO LOW on the output pins) with a change in the input pins, but will not allow the device to clear a fault (LOW TO HIGH) unless the ```LATCH_RST``` (PA1) is HIGH, in addition to the change in the IMD or BMS pin. 

