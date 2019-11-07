
;DA2A.asm
;Kyungseo Yun

.include <m328pdef.inc>

.ORG 0
CBI PORTB, 5
CLR R19
STS TCCR1A, R19
LDI R18, 0X04
STS TCCR1B, R18 ;prescalar = 256
SBI DDRB, 3		;PB3 as an output

BEGIN: 
	SBI PORTB, 3	;PB3 = 1
	RCALL DELAY_40DC ;40% DC
	CBI PORTB, 3 	;PB3 = 0
	RCALL DELAY_60DC ;60% DC
	RJMP BEGIN
	
DELAY_40DC:	;set TCNT1 register for 40DC
	LDI R20, HIGH(49911)
	STS TCNT1H, R20
	LDI R20, LOW(49911)
	STS TCNT1L, R20
	RJMP COUNT

DELAY_60DC:	;set TCNT1 register for 60DC
	LDI R20, HIGH(42108)
	STS TCNT1H, R20
	LDI R20, LOW(42108)
	STS TCNT1L,R20


COUNT:
	IN R20, TIFR1
	SBRS R20, 2		;check TOV1
	RJMP COUNT 		;if TOV1 = 0, keep counting
	OUT TIFR1, R18	;clear TOV1
	RET