#include "l3gd20.h"
#include "i2c.h"

void swapBytesInWord(char *ptr){
	char t= ptr[0];
	ptr[0]=ptr[1];
	ptr[1]=t;
}

void byteOrder(char *ptr, uint16_t n){
	for(uint16_t i = 0; i < 2*n; i+=2)
		swapBytesInWord(ptr+i);
}

uint8_t readGyroData(uint8_t devAddr, vector3 *out){
	if(readManyRegisters((char *) out, devAddr, 0x28|(1<<7), 6)!=0){
		return -1;
	}
 	return 0;
}

uint8_t readWhoAmI(uint8_t devAddr){
	return readRegister(devAddr, 0x0f);
}

uint8_t readStatus(uint8_t devAddr){
	return readRegister(devAddr, 0x27);
}

uint8_t readTemperature(uint8_t devAddr){
	return readRegister(devAddr, 0x26);
}

void initL3gd20(uint8_t devAddr){
	writeRegister(devAddr, 0x23, 0x00); //CTRL_REG4
	writeRegister(devAddr, 0x20, 0x6f); //CTRL_REG1
}

