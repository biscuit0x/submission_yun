/*
 * midterm2.c
 *
 * Created: 12/14/2019 1:48:41 PM
 * Author : jayne
 */ 

#define F_CPU 16000000UL
#define BAUD_PRESCALAR (((F_CPU / (BAUDRATE * 16UL))) - 1)
#define I2C_WRITE 0x00
#define I2C_READ 0x01
#define APDS9960_WRITE_ADR (APDS9960_I2C_ADDR << 1) | I2C_WRITE
#define APDS9960_READ_ADR (APDS9960_I2C_ADDR << 1) | I2C_READ
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "i2c_master.h"
#include "uart.c"
#include "APDS9960_def.h"
#include "adps.c"

char colors[256];
uint16_t ambient, red, green, blue = 0;
void read_adc(void);

int main(void)
{
	i2c_init(); // initialize i2c
	USART_Init(); // initialize uart
	apds_init(); //initialize APDS960 	
	//connects to wifi	
	USART_SendString("AT\r\n"); 
	_delay_ms(1000);
	USART_SendString("AT+CWMODE=1\r\n"); 
	_delay_ms(1000);
	USART_SendString("AT+CWLAP\r\n"); 
	_delay_ms(4000);
	USART_SendString("AT+CWJAP=\"LAST\",\"OVER\"\r\n"); 
	_delay_ms(3000);
	
    while (1) //loop
    {
		read_adc(); //read colors
		USART_SendString("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n"); //thingspeak connection
		_delay_ms(3000);
		USART_SendString("AT+CIPSEND=200\r\n"); //send 200 characters
		_delay_ms(1000);
		snprintf(colors, sizeof(colors),"GET https://api.thingspeak.com/update?api_key=NX0KTDW6RHKFUTFC&field1=%d&field2=%d&field3=%d&field4=%d\r\n", ambient, red, green, blue);
		USART_SendString(colors); //send temperature 
		_delay_ms(1000);
		USART_SendString("AT+CIPCLOSE\r\n"); //end
		_delay_ms(1000);
    }
}

void read_adc(void) {	//read sensor values
	i2c_start(APDS9960_WRITE_ADR); 
	i2c_write(APDS9960_CDATAL); 
	i2c_stop();
	i2c_start(APDS9960_READ_ADR); 
	ambient =((int)i2c_read_ack()|((int)i2c_read_ack()<<8));
	red = ((int)i2c_read_ack()|((int)i2c_read_ack()<<8));
	green = ((int)i2c_read_ack()|((int)i2c_read_ack()<<8));
	blue = ((int)i2c_read_ack()|((int)i2c_read_ack()<<8));
	i2c_stop();
}