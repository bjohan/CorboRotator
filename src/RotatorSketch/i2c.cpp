#include "i2c.h"
#include <Wire.h>

uint8_t readRegister(uint8_t devaddr, uint8_t regaddr){
	uint8_t data;
	Wire.beginTransmission(devaddr);
	Wire.write(regaddr);
	Wire.endTransmission();
	
	Wire.requestFrom((int)devaddr, (int)1);
	data = Wire.read();
	Wire.endTransmission();
	return data;
}

uint8_t writeRegister(uint8_t devAddr, uint8_t regAddr, uint8_t regData){
	Wire.beginTransmission(devAddr);
	Wire.write(regAddr);
	Wire.write(regData);
	return Wire.endTransmission();
}

uint8_t readManyRegisters(char *buf, uint8_t devAddr, uint8_t startReg, uint8_t n){
	Wire.beginTransmission(devAddr);
	Wire.write(startReg);
	Wire.endTransmission();

	Wire.requestFrom(devAddr, n);
	while(Wire.available() < n){
	}
	
	for(int i = 0 ; i < n ; i++){
		buf[i] = Wire.read();
	}
	return 0;
}

