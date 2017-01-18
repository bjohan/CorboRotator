#include "lsm303.h"
#include "i2c.h"

uint8_t readAccelerometerData(uint8_t devAddr, vector3 *out){
	if(readManyRegisters((char *) out, devAddr, 0x28|(1<<7), 6)!=0){
		return -1;
	}
 	return 0;
}

uint8_t readMagnetometerData(uint8_t devAddr, vector3 *out){
	if(readManyRegisters((char *) out, devAddr, 0x08|(1<<7), 6)!=0){
		return -1;
	}
 	return 0;
}

void initLsm303(uint8_t devAddr){
	writeRegister(devAddr, 0x21, 0x00);
	writeRegister(devAddr, 0x20, 0x57);
	writeRegister(devAddr, 0x24, 0x64);
	writeRegister(devAddr, 0x25, 0x20);
	writeRegister(devAddr, 0x26, 0x00);
}

