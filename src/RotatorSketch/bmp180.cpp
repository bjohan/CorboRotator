#include "lsm303.h"
#include "i2c.h"
#include "bmp180.h"
#include <Arduino.h>
int8_t readPressure(uint8_t devAddr, vector3 *out){
	if(readManyRegisters((char *) out, devAddr, 0x28|(1<<7), 6)!=0){
		return -1;
	}
 	return 0;
}

int16_t readTemperatureUncal(uint8_t devAddr){
	int16_t t;
	writeRegister(devAddr, 0xf4, 0x2e);
	delay(5);
	readManyRegisters(((char *) &t), devAddr, 0xf6, 2);
	return t;
}


int32_t compensateTemperature(int16_t temperature, calibrationCoefficient_t *cal){
	Serial.print("ac6 "); Serial.println(cal->ac6);
	Serial.print("ac5 "); Serial.println(cal->ac5);
	Serial.print("mc "); Serial.println(cal->mc);
	Serial.print("md "); Serial.println(cal->md);
	Serial.print("ut "); Serial.println(temperature);
	int32_t x1 = ((temperature-cal->ac6)*cal->ac5) >> 15;
	int32_t x2 = ((cal->mc)<<11)/(x1+cal->md);
	int32_t b5 = x1+x2;
	return (b5+8);
}

int32_t readPressureUncal(uint8_t devAddr, uint8_t oss){
	int32_t t;
	writeRegister(devAddr, 0xf4, 0x2e|(oss<<6));
	delay(5);
	readManyRegisters(((char *)&t), devAddr, 0xf6, 3);

	//sign extend if needed
	if( ((char*)&t)[3]&0x80)
		((char*)&t)[4]=0xff;
	return t;
}


int16_t initBmp180(uint8_t devAddr, calibrationCoefficient_t *cal){
	int16_t temp;
	uint32_t pres;
	uint8_t oss=3;
	//read calibration coefficients
	if(readManyRegisters((char *) cal, devAddr, 0xAA, 22)!=0)
		return -1;

	//read uncompensated temperature
	writeRegister(devAddr, 0xf7, 0x2e);

	if(readManyRegisters((char *) &temp, devAddr, 0xf4, 2)!=0)
		return -2;

	writeRegister(devAddr, 0x34|(oss<<6), 0xf4);
	if(readManyRegisters((char *) &pres, devAddr, 0xf6, 3)!=0)
		return -3;
	return (int)cal;	
		

}

