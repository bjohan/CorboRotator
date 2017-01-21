#include <stdint.h>
#include "vector.h"

uint8_t readAccelerometerData(uint8_t devAddr, vector3 *out);
uint8_t readMagnetometerData(uint8_t devAddr, vector3 *out);
uint8_t readTemperatureLsm303(uint8_t devAddr, int16_t *data);
void initLsm303(uint8_t devAddr);

