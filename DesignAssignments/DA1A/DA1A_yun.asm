;
; assignment_1A.asm
;
; Created: 9/14/2019 6:36:18 PM
; Author : Kyungseo Yun
;

.include  <m328pdef.inc>
.def ZERO = R2
.ORG 00
	LDI R22, 0X11
	LDI R23, 0X22
	LDI R24, 0XBB
	LDI R25, 0XAA
	CPI R24, 0
	BREQ L2

L1 : 
	 ADD R17, R22
	 ADC R18, R23
	 ADC R19, ZERO
	 DEC R24
	 BRNE L1

L2 : CPI R25, 0
	 BREQ end 

L3 : 
	 ADD R18, R22
	 ADC R19, R23
	 ADC R20, ZERO
	 DEC R25
	 BRNE L3

end : jmp end
	
