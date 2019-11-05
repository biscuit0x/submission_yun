;DA2A_2.asm
;Kyungseo Yun
;Connect a switch to PORTC.3 (active high - turn on the pull up transistor) to poll 
;for an event to turn on the led at PORTB.2 for 1.333 sec after the event.

.include <m328pdef.inc>

.ORG 0 
CLR R19
LDI R18, 0x04
CBI DDRC, 3	;PC3 as an input
SBI PORTB, 2 
SBI DDRB, 2 ; PB2 as an output
STS TCCR1A, R19
LDI R20, 0X05
STS TCCR1B, R20 ;prescaler=1024

BEGIN:
	SBIS PINC, 3
	RCALL LED_ON
	RJMP BEGIN 

LED_ON:	
	OUT TIFR1, R18	;clear TOV1
	LDI R20, HIGH(-20828)
	STS TCNT1H, R20
	LDI R20, LOW(-20828)
	STS TCNT1L, R20
	CBI PORTB, 2	

COUNT:
	IN R20, TIFR1
	SBRS R20, 2		;check TOV1
	RJMP COUNT 		;if TOV1 = 0, keep counting
	SBI PORTB, 2
	RET
