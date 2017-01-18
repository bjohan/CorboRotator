#include "vector.h"

void swapBytesInWord(char *ptr);
void byteOrder(char *ptr, uint16_t n);
uint8_t readGyroData(uint8_t devAddr, vector3 *out);
uint8_t readWhoAmI(uint8_t devAddr);
uint8_t readStatus(uint8_t devAddr);
uint8_t readTemperature(uint8_t devAddr);
void initL3gd20(uint8_t devAddr);

