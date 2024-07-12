                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ISO C Compiler 
                                      3 ; Version 4.4.0 #14620 (Linux)
                                      4 ;--------------------------------------------------------
                                      5 	.module blink
                                      6 	.optsdcc -mstm8
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _main
                                     12 	.globl _delay
                                     13 ;--------------------------------------------------------
                                     14 ; ram data
                                     15 ;--------------------------------------------------------
                                     16 	.area DATA
                                     17 ;--------------------------------------------------------
                                     18 ; ram data
                                     19 ;--------------------------------------------------------
                                     20 	.area INITIALIZED
                                     21 ;--------------------------------------------------------
                                     22 ; Stack segment in internal ram
                                     23 ;--------------------------------------------------------
                                     24 	.area SSEG
      000001                         25 __start__stack:
      000001                         26 	.ds	1
                                     27 
                                     28 ;--------------------------------------------------------
                                     29 ; absolute external ram data
                                     30 ;--------------------------------------------------------
                                     31 	.area DABS (ABS)
                                     32 
                                     33 ; default segment ordering for linker
                                     34 	.area HOME
                                     35 	.area GSINIT
                                     36 	.area GSFINAL
                                     37 	.area CONST
                                     38 	.area INITIALIZER
                                     39 	.area CODE
                                     40 
                                     41 ;--------------------------------------------------------
                                     42 ; interrupt vector
                                     43 ;--------------------------------------------------------
                                     44 	.area HOME
      008000                         45 __interrupt_vect:
      008000 82 00 80 07             46 	int s_GSINIT ; reset
                                     47 ;--------------------------------------------------------
                                     48 ; global & static initialisations
                                     49 ;--------------------------------------------------------
                                     50 	.area HOME
                                     51 	.area GSINIT
                                     52 	.area GSFINAL
                                     53 	.area GSINIT
      008007 CD 80 9C         [ 4]   54 	call	___sdcc_external_startup
      00800A 4D               [ 1]   55 	tnz	a
      00800B 27 03            [ 1]   56 	jreq	__sdcc_init_data
      00800D CC 80 04         [ 2]   57 	jp	__sdcc_program_startup
      008010                         58 __sdcc_init_data:
                                     59 ; stm8_genXINIT() start
      008010 AE 00 00         [ 2]   60 	ldw x, #l_DATA
      008013 27 07            [ 1]   61 	jreq	00002$
      008015                         62 00001$:
      008015 72 4F 00 00      [ 1]   63 	clr (s_DATA - 1, x)
      008019 5A               [ 2]   64 	decw x
      00801A 26 F9            [ 1]   65 	jrne	00001$
      00801C                         66 00002$:
      00801C AE 00 00         [ 2]   67 	ldw	x, #l_INITIALIZER
      00801F 27 09            [ 1]   68 	jreq	00004$
      008021                         69 00003$:
      008021 D6 80 2C         [ 1]   70 	ld	a, (s_INITIALIZER - 1, x)
      008024 D7 00 00         [ 1]   71 	ld	(s_INITIALIZED - 1, x), a
      008027 5A               [ 2]   72 	decw	x
      008028 26 F7            [ 1]   73 	jrne	00003$
      00802A                         74 00004$:
                                     75 ; stm8_genXINIT() end
                                     76 	.area GSFINAL
      00802A CC 80 04         [ 2]   77 	jp	__sdcc_program_startup
                                     78 ;--------------------------------------------------------
                                     79 ; Home
                                     80 ;--------------------------------------------------------
                                     81 	.area HOME
                                     82 	.area HOME
      008004                         83 __sdcc_program_startup:
      008004 CC 80 5B         [ 2]   84 	jp	_main
                                     85 ;	return from main will return to caller
                                     86 ;--------------------------------------------------------
                                     87 ; code
                                     88 ;--------------------------------------------------------
                                     89 	.area CODE
                                     90 ;	blink.c: 15: void delay(unsigned long count) {
                                     91 ;	-----------------------------------------
                                     92 ;	 function delay
                                     93 ;	-----------------------------------------
      00802D                         94 _delay:
      00802D 52 08            [ 2]   95 	sub	sp, #8
                                     96 ;	blink.c: 16: while (count--)
      00802F 16 0D            [ 2]   97 	ldw	y, (0x0d, sp)
      008031 17 07            [ 2]   98 	ldw	(0x07, sp), y
      008033 1E 0B            [ 2]   99 	ldw	x, (0x0b, sp)
      008035                        100 00101$:
      008035 1F 01            [ 2]  101 	ldw	(0x01, sp), x
      008037 7B 07            [ 1]  102 	ld	a, (0x07, sp)
      008039 6B 03            [ 1]  103 	ld	(0x03, sp), a
      00803B 7B 08            [ 1]  104 	ld	a, (0x08, sp)
      00803D 16 07            [ 2]  105 	ldw	y, (0x07, sp)
      00803F 72 A2 00 01      [ 2]  106 	subw	y, #0x0001
      008043 17 07            [ 2]  107 	ldw	(0x07, sp), y
      008045 24 01            [ 1]  108 	jrnc	00121$
      008047 5A               [ 2]  109 	decw	x
      008048                        110 00121$:
      008048 4D               [ 1]  111 	tnz	a
      008049 26 08            [ 1]  112 	jrne	00122$
      00804B 16 02            [ 2]  113 	ldw	y, (0x02, sp)
      00804D 26 04            [ 1]  114 	jrne	00122$
      00804F 0D 01            [ 1]  115 	tnz	(0x01, sp)
      008051 27 03            [ 1]  116 	jreq	00104$
      008053                        117 00122$:
                                    118 ;	blink.c: 17: nop();
      008053 9D               [ 1]  119 	nop
      008054 20 DF            [ 2]  120 	jra	00101$
      008056                        121 00104$:
                                    122 ;	blink.c: 18: }
      008056 1E 09            [ 2]  123 	ldw	x, (9, sp)
      008058 5B 0E            [ 2]  124 	addw	sp, #14
      00805A FC               [ 2]  125 	jp	(x)
                                    126 ;	blink.c: 20: int main(void)
                                    127 ;	-----------------------------------------
                                    128 ;	 function main
                                    129 ;	-----------------------------------------
      00805B                        130 _main:
                                    131 ;	blink.c: 23: CLK_CKDIVR = 0;
      00805B 35 00 50 C6      [ 1]  132 	mov	0x50c6+0, #0x00
                                    133 ;	blink.c: 26: PORT(BMS_OUT_PORT, DDR)  |= BMS_OUT_PIN; // i.e. PB_DDR |= (1 << 5);
      00805F 72 18 50 11      [ 1]  134 	bset	0x5011, #4
                                    135 ;	blink.c: 28: PORT(BMS_OUT_PORT, CR1)  |= BMS_OUT_PIN; // i.e. PB_CR1 |= (1 << 5);
      008063 72 18 50 12      [ 1]  136 	bset	0x5012, #4
                                    137 ;	blink.c: 31: PORT(IMD_OUT_PORT, DDR) |= IMD_OUT_PIN;
      008067 72 1C 50 11      [ 1]  138 	bset	0x5011, #6
                                    139 ;	blink.c: 32: PORT(IMD_OUT_PORT, CR1) |= IMD_OUT_PIN;
      00806B 72 1C 50 12      [ 1]  140 	bset	0x5012, #6
                                    141 ;	blink.c: 34: while(1) {
      00806F                        142 00102$:
                                    143 ;	blink.c: 35: PORT(BMS_OUT_PORT, ODR) |= BMS_OUT_PIN; // turn on
      00806F 72 18 50 0F      [ 1]  144 	bset	0x500f, #4
                                    145 ;	blink.c: 36: PORT(IMD_OUT_PORT, ODR) &= IMD_OUT_PIN; //turn off
      008073 C6 50 0F         [ 1]  146 	ld	a, 0x500f
      008076 A4 40            [ 1]  147 	and	a, #0x40
      008078 C7 50 0F         [ 1]  148 	ld	0x500f, a
                                    149 ;	blink.c: 38: delay(300000L); // i don't know if we can know how long this delay is without more info
      00807B 4B E0            [ 1]  150 	push	#0xe0
      00807D 4B 93            [ 1]  151 	push	#0x93
      00807F 4B 04            [ 1]  152 	push	#0x04
      008081 4B 00            [ 1]  153 	push	#0x00
      008083 CD 80 2D         [ 4]  154 	call	_delay
                                    155 ;	blink.c: 40: PORT(BMS_OUT_PORT, ODR) &= ~BMS_OUT_PIN; // PB_ODR &= ~(1 << 5);
      008086 72 19 50 0F      [ 1]  156 	bres	0x500f, #4
                                    157 ;	blink.c: 41: PORT(IMD_OUT_PORT, ODR) |= IMD_OUT_PIN;
      00808A 72 1C 50 0F      [ 1]  158 	bset	0x500f, #6
                                    159 ;	blink.c: 43: delay(300000L);
      00808E 4B E0            [ 1]  160 	push	#0xe0
      008090 4B 93            [ 1]  161 	push	#0x93
      008092 4B 04            [ 1]  162 	push	#0x04
      008094 4B 00            [ 1]  163 	push	#0x00
      008096 CD 80 2D         [ 4]  164 	call	_delay
      008099 20 D4            [ 2]  165 	jra	00102$
                                    166 ;	blink.c: 45: }
      00809B 81               [ 4]  167 	ret
                                    168 	.area CODE
                                    169 	.area CONST
                                    170 	.area INITIALIZER
                                    171 	.area CABS (ABS)
