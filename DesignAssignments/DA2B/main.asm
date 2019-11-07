.ORG 0		;location of reset
	JMP MAIN
.ORG 0x02	;location of external interrupt INT0
	JMP LED_ON

MAIN:
	LDI R20, HIGH(RAMEND)	;stack initialization
	OUT SPH, R20
	LDI R20, LOW(RAMEND)
	OUT SPL, R20 
	CLR R19
	STS TCCR1A, R19
	STS TCCR1B, R19
	LDI R20, 0X02	;set INT0 falling edge triggered
	STS EICRA, R20 
	SBI PORTB, 2	;set PB2 (led off)	
	SBI DDRB, 2		;PB2 as output 

	SBI PORTD, 2	;PD2 pull-up activated
	LDI R20, 1<<INT0
	OUT EIMSK, R20	;enable INT0
	SEI	;enable interrupts
HERE : JMP HERE	;wait for INT0

LED_ON:	
	LDI R20, 0X05
	STS TCCR1B, R20 ;prescaler=1024	
	CLR R20
	STS TCNT1H, R20
	STS TCNT1L, R20
	CBI PORTB, 2
	RCALL DELAY1
	RETI

DELAY1:
	LDS	R19, TCNT1H	 ;load upper timer1
	LDS	R18, TCNT1L  ;load lower timer1
	CPI R19, 0X51    ;compare upper timer1 with 0x51
	BRLO DELAY1		 ;if lower than 0x51, wait more

DELAY2:
	LDS R18, TCNT1L	;load lower timer1
	CPI	R18, 0X5C	;compare lower timer1 with 0x5c
	BRLO DELAY2		;if lower than 0x5C, wait more
	SBI	PORTB, 2	;turn off led
	RET				