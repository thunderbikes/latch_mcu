;--------------------------------------------------------
; File Created by SDCC : free open source ISO C Compiler 
; Version 4.4.0 #14620 (Linux)
;--------------------------------------------------------
	.module blink
	.optsdcc -mstm8
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
	.globl _delay
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area DATA
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area INITIALIZED
;--------------------------------------------------------
; Stack segment in internal ram
;--------------------------------------------------------
	.area SSEG
__start__stack:
	.ds	1

;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area DABS (ABS)

; default segment ordering for linker
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area CONST
	.area INITIALIZER
	.area CODE

;--------------------------------------------------------
; interrupt vector
;--------------------------------------------------------
	.area HOME
__interrupt_vect:
	int s_GSINIT ; reset
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area GSINIT
	call	___sdcc_external_startup
	tnz	a
	jreq	__sdcc_init_data
	jp	__sdcc_program_startup
__sdcc_init_data:
; stm8_genXINIT() start
	ldw x, #l_DATA
	jreq	00002$
00001$:
	clr (s_DATA - 1, x)
	decw x
	jrne	00001$
00002$:
	ldw	x, #l_INITIALIZER
	jreq	00004$
00003$:
	ld	a, (s_INITIALIZER - 1, x)
	ld	(s_INITIALIZED - 1, x), a
	decw	x
	jrne	00003$
00004$:
; stm8_genXINIT() end
	.area GSFINAL
	jp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME
	.area HOME
__sdcc_program_startup:
	jp	_main
;	return from main will return to caller
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE
;	blink.c: 15: void delay(unsigned long count) {
;	-----------------------------------------
;	 function delay
;	-----------------------------------------
_delay:
	sub	sp, #8
;	blink.c: 16: while (count--)
	ldw	y, (0x0d, sp)
	ldw	(0x07, sp), y
	ldw	x, (0x0b, sp)
00101$:
	ldw	(0x01, sp), x
	ld	a, (0x07, sp)
	ld	(0x03, sp), a
	ld	a, (0x08, sp)
	ldw	y, (0x07, sp)
	subw	y, #0x0001
	ldw	(0x07, sp), y
	jrnc	00121$
	decw	x
00121$:
	tnz	a
	jrne	00122$
	ldw	y, (0x02, sp)
	jrne	00122$
	tnz	(0x01, sp)
	jreq	00104$
00122$:
;	blink.c: 17: nop();
	nop
	jra	00101$
00104$:
;	blink.c: 18: }
	ldw	x, (9, sp)
	addw	sp, #14
	jp	(x)
;	blink.c: 20: int main(void)
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	blink.c: 23: CLK_CKDIVR = 0;
	mov	0x50c6+0, #0x00
;	blink.c: 26: PORT(BMS_OUT_PORT, DDR)  |= BMS_OUT_PIN; // i.e. PB_DDR |= (1 << 5);
	bset	0x5011, #4
;	blink.c: 28: PORT(BMS_OUT_PORT, CR1)  |= BMS_OUT_PIN; // i.e. PB_CR1 |= (1 << 5);
	bset	0x5012, #4
;	blink.c: 31: PORT(IMD_OUT_PORT, DDR) |= IMD_OUT_PIN;
	bset	0x5011, #6
;	blink.c: 32: PORT(IMD_OUT_PORT, CR1) |= IMD_OUT_PIN;
	bset	0x5012, #6
;	blink.c: 34: while(1) {
00102$:
;	blink.c: 35: PORT(BMS_OUT_PORT, ODR) |= BMS_OUT_PIN; // turn on
	bset	0x500f, #4
;	blink.c: 36: PORT(IMD_OUT_PORT, ODR) &= IMD_OUT_PIN; //turn off
	ld	a, 0x500f
	and	a, #0x40
	ld	0x500f, a
;	blink.c: 38: delay(300000L); // i don't know if we can know how long this delay is without more info
	push	#0xe0
	push	#0x93
	push	#0x04
	push	#0x00
	call	_delay
;	blink.c: 40: PORT(BMS_OUT_PORT, ODR) &= ~BMS_OUT_PIN; // PB_ODR &= ~(1 << 5);
	bres	0x500f, #4
;	blink.c: 41: PORT(IMD_OUT_PORT, ODR) |= IMD_OUT_PIN;
	bset	0x500f, #6
;	blink.c: 43: delay(300000L);
	push	#0xe0
	push	#0x93
	push	#0x04
	push	#0x00
	call	_delay
	jra	00102$
;	blink.c: 45: }
	ret
	.area CODE
	.area CONST
	.area INITIALIZER
	.area CABS (ABS)
