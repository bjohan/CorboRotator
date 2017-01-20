#include "lsm303.h"
#include "i2c.h"

typedef struct{
	int16_t ac1;
	int16_t ac2;
	int16_t ac3;
	uint16_t ac4;
	uint16_t ac5;
	uint16_t ac6;
	int16_t b1;
	int16_t b2;
	int16_t ma;
	int16_t mb;
	int16_t mc;
} calibrationCoefficient_t;

int8_t readPressure(uint8_t devAddr, vector3 *out);
int16_t initBmp180(uint8_t devAddr, calibrationCoefficient_t *cal);
int16_t readTemperatureUncal(uint8_t devAddr);
int32_t readPressureUncal(uint8_t devAddr, uint8_t oss);

