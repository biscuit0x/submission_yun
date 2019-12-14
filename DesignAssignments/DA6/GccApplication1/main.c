/*
 * GccApplication1.c
 * Author : jayne
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h> 
#include <stdio.h> 
#include "inc\MPU6050_def.h" 
#include "inc\i2c_master.h" 
#include "inc\uart.h" 

float Acc_x, Acc_y, Acc_z, Gy_x, Gy_y, Gy_z;
void read(void);
void MPU6050_init(void);

int main(void){
	char buffer[20], float_[10];
	float x_a, Y_a, Z_a, X_g, Y_g, Z_g;
	
	MPU6050_init();
	USART_Init(9600);
	i2c_init();

	while(1){
	read();
	
	x_a = Acc_x/16384.0; /*Divide raw value by sensitivity scale factor to get real values */
	Y_a = Acc_y/16384.0;
	Z_a = Acc_x/16384.0;
	X_g = Gy_x/16.4;
	Y_g = Gy_y/16.4;
	Z_g = Gy_z/16.4;

	dtostrf( x_a, 3, 2, float_ ); /* Take values in buffer to send all parameters over USART */
	sprintf(buffer,"Ax: %s, ",float_);
	USART_SendString(buffer);
	
	dtostrf( Y_a, 3, 2, float_ );
	sprintf(buffer,"Ay: %s, ",float_);
	USART_SendString(buffer);

	dtostrf( Z_a, 3, 2, float_ );
	sprintf(buffer,"Az: %s, \n",float_);
	USART_SendString(buffer);
	
	dtostrf( X_g, 3, 2, float_ );
	sprintf(buffer,"Gx: %s, ",float_);
	USART_SendString(buffer);
	
	dtostrf( Y_g, 3, 2, float_ );
	sprintf(buffer,"Gy: %s, ",float_);
	USART_SendString(buffer);
	
	dtostrf( Z_g, 3, 2, float_ );	
	sprintf(buffer,"Gz: %s, \n\n",float_);
	USART_SendString(buffer);
	
	_delay_ms(1000);
}
return 0;
}

void read(void){ //read gyro sensor value
	i2c_start(0xD0);
	i2c_write(ACCEL_XOUT_H);
	i2c_stop();	
	i2c_start(0xD1);
	Acc_x = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	Acc_y = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	Acc_z = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());

	i2c_stop();
	i2c_start(0xD0);
	i2c_write(GYRO_XOUT_H); 
	i2c_stop();
	i2c_start(0xD1);
	Gy_x = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	Gy_y = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	Gy_z = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	i2c_stop();
}

void MPU6050_init(void){ //gyro sensor initialization
	_delay_ms(150);
	i2c_start(0xD0); //write addr
	i2c_write(SMPLRT_DIV); //1KHz sample rate
	i2c_write(0x07); 
	i2c_stop();
	
	i2c_start(0xD0); //power mgnt reg
	i2c_write(PWR_MGMT_1);
	i2c_write(0x01); 
	i2c_stop();
	
	i2c_start(0xD0); //configuration reg
	i2c_write(CONFIG); 
	i2c_write(0x00); //8Khz
	i2c_stop();
	
	i2c_start(0xD0);
	i2c_write(GYRO_CONFIG);  //range +/2000
	i2c_write(0x18); 
	i2c_stop();
	
	i2c_start(0xD0);
	i2c_write(INT_ENABLE); //enable interrupt
	i2c_write(0x01);
	i2c_stop();
}