#include "lsm303.h"
#include "i2c.h"
#include "bmp180.h"
#include <Servo.h>
int8_t readPressure(uint8_t devAddr, vector3 *out){
	if(readManyRegisters((char *) out, devAddr, 0x28|(1<<7), 6)!=0){
		return -1;
	}
 	return 0;
}

int16_t readTemperatureUncal(uint8_t devAddr){
	int16_t t;
	writeRegister(devAddr, 0xf4, 0x2e);
	readManyRegisters(((char *) &t)+1, devAddr, 0xf6, 2);
	return t;
}

int32_t readPressureUncal(uint8_t devAddr, uint8_t oss){
	int32_t t;
	writeRegister(devAddr, 0xf4, 0x2e|(oss<<6));
	readManyRegisters(((char *)&t)+1, devAddr, 0xf6, 3);

	//sign extend if needed
	if( ((char*)&t)[2]&0x80)
		((char*)&t)[3]=0xff;
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

