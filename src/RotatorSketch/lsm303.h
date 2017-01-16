#include <stdint.h>
typedef struct{
	int16_t x;
	int16_t y;
	int16_t z;
} vector3;

uint8_t readRegister(uint8_t devaddr, uint8_t regaddr);
uint8_t writeRegister(uint8_t devAddr, uint8_t regAddr, uint8_t regData);
uint8_t readManyRegisters(char *buf, uint8_t devAddr, uint8_t startReg, uint8_t n);
uint8_t readAccelerometerData(uint8_t devAddr, vector3 *out);
uint8_t readMagnetometerData(uint8_t devAddr, vector3 *out);
void initLsm303(uint8_t devAddr);

