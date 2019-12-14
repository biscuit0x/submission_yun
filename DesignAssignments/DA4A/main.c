#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>



volatile int val, power = 0; //val value and power on/off state
void init_pwm(void) {
	DDRD |= (1<<PORTD1); 
	DDRC |= (1<<PORTC1); 
	TCCR0A |=
	(1<<WGM01)| 
	(1<<WGM00)| 
	(1<<COM0A1);
	PCMSK1 |= (1<<PCINT9); 
	PCICR |= (1<<PCIE1); 
	TCCR0B |=
	(1<<CS02)| // Prescaler = 1024
	(1<<CS00); 
	
}

void init_adc(void) {
	DDRC &= (0<<PORTC0);  //adc initialization
	PORTC |= (1<<PORTC1); 
	ADMUX |= (1<<REFS0); 
	ADCSRA |= 
	(1<<ADEN)| 
	(1<<ADPS2)| 
	(1<<ADPS1)|
	(1<<ADPS0); // prescaler = 128
	sei();
}

int main(void)
{
    init_adc(); // initialization
	init_pwm(); 
    while (1) 
    {
		if (power == 1) {
			ADCSRA |= (1<<ADSC); 
			while((ADCSRA&(1<<ADIF))==0); //wait for conversion
			ADCSRA |= (1<<ADIF); //reset
			val = ADC; // store ADC to val for debugging
			OCR0A = (224/1024)*val; //max speed = 95% of PMW
		}
		else
			OCR0A = 0;
    }
}

//PC1 = power button 
ISR(PCINT1_vect) { 
	if(!(PINC & (1 << PINC1))) { 
		_delay_ms(100); 
		while(!(PINC&(1<<PINC1))); 
		power ^= 1; //toggle power
	}
}

