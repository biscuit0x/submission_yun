#include "i2c_master.c"

void apds_init(){
		uint8_t data;
		i2c_readReg(APDS9960_WRITE_ADR | I2C_WRITE, APDS9960_ID, &data, 1);
		if (data != APDS9960_ID_1) while(1); 
		data = APDS9960_PON | APDS9960_AEN;
		i2c_writeReg(APDS9960_WRITE_ADR, APDS9960_ENABLE, &data, 1);
		data = DEFAULT_AGAIN; 
		i2c_writeReg(APDS9960_WRITE_ADR, APDS9960_CONTROL, &data, 1);
		data = DEFAULT_ATIME; 
		i2c_writeReg(APDS9960_WRITE_ADR, APDS9960_ATIME, &data, 1);
}


