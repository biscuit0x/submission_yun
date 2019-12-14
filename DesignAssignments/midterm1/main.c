#define F_CPU 16000000UL
#define BAUDRATE 115200
#define BAUD_PRESCALAR (((F_CPU / (BAUDRATE * 16UL))) - 1)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

void set_timer(void); //timer initialization
void adc_init(void); //adc initialization
void read_adc(void); //read temperature
void AT_init(void); //esplorer set up
void USART_init(void); //USART functions
void USART_sendChar(char ch); 
void USART_sendString(char* str); 


volatile int adc_temp;

int main(void)
{
	USART_init(); // initialize USART
	AT_init(); // initialize AT setttings
	set_timer(); // initialize Timer/Interrupt
	adc_init();  // initialize ADC

	while (1) // Loop forever
	{
	}
}

ISR (TIMER1_OVF_vect) {
	read_adc(); //get temp
	char temp[50]; 
	snprintf(temp,sizeof(temp),"%d\r\n",adc_temp); //read into temp buffer
	USART_sendString("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n"); //connect to thingspeak
	_delay_ms(3000);
	USART_sendString("AT+CIPSEND=50\r\n"); //send 50 characters
	_delay_ms(1000);
	USART_sendString("GET /update?key=W7PJEYOYV6BGU372&field1="); //channel key
	USART_sendString(temp); //send temperature 
	_delay_ms(1000);
	USART_sendString("AT+CIPCLOSE\r\n"); //end
	_delay_ms(1000);
	TCNT1 = 0; //reset
}

void read_adc(void) {
	unsigned char i =4;
	adc_temp = 0; //initialize	
	while (i--) {
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		adc_temp+= ADC;
		_delay_ms(50);
	}
	adc_temp = adc_temp /4; //get average
}

void set_timer(void) {
	TCNT1 = 0; //reset timer1
	TIMSK1 |= (1 << TOIE0); //Enable Timer1 Interrupt
	sei(); //Enable Global Interrupt
	TCCR1B |= (1<<CS12)|(1<<CS10); //prescaler = 1024 
}

void USART_init( void )
{
	UBRR0H = 0; 
	UBRR0L = 8; // BAUD 115200
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
	UCSR0B = _BV(RXEN0) | _BV(TXEN0); /* Enable RX and TX */
}

void USART_sendChar(char ch) {
	//wait until UDR0 is empty
	while (!(UCSR0A & (1<<UDRE0))); 
	UDR0 = ch; ; //transmit ch
}

void USART_sendString(char* str) {
	while ((*str != '\0')) { 
		while (!(UCSR0A & (1<<UDRE0))); //wait until UDR0 is empty
		USART_sendChar(*str); //transmit ch
		str++; 
	}
}

void adc_init(void) {
	ADMUX |= (0<<REFS1)| // Reference Select
	(1<<REFS0)| // Selected AVcc
	(0<<ADLAR)| // Left Adjust Result OFF
	(1<<MUX2) | // Analog Channel Select
	(0<<MUX1) | // 1 0 1
	(1<<MUX0) ; // Channel 5 or PC5
	ADCSRA |= (1<<ADEN)| // Enable ADC
	(0<<ADSC)| // Do not start conversion
	(0<<ADATE)|// Auto Trigger Disabled
	(0<<ADIF)| // Interrupt Flag Cleared
	(0<<ADIE)| // Interrupt Disabled
	(1<<ADPS2)| // ADC Prescaler Select
	(0<<ADPS1)| // Set to
	(1<<ADPS0); // 32
}

void AT_init(void) {
	USART_sendString("AT\r\n"); // Sends AT, expect OK
	_delay_ms(1000);
	USART_sendString("AT+CWMODE=1\r\n"); // Sends mode set to station, expect OK
	_delay_ms(1000);
	USART_sendString("AT+CWLAP\r\n"); // Send command to list Wifi networks, expect list
	_delay_ms(4000);
	USART_sendString("AT+CWJAP=\"Yun\",\"st980204\"\r\n"); // Send command to  join guest wifi, expect OK
	_delay_ms(3000);
}

