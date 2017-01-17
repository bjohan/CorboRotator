#include <Wire.h>

uint8_t readRegister(uint8_t devaddr, uint8_t regaddr);
uint8_t writeRegister(uint8_t devAddr, uint8_t regAddr, uint8_t regData);
uint8_t readManyRegisters(char *buf, uint8_t devAddr, uint8_t startReg, uint8_t n);

