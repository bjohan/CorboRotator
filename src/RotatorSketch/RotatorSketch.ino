#include <EEPROM.h>
#include <Servo.h>
#include <Wire.h>
#include "lsm303.h"
#include "l3gd20.h"
#include "bmp180.h"
#include "i2c.h"


extern "C"
{
#include "uart_command_lib.h"
#include "corbomite.h"
}



uint8_t servoPower = 0;

extern "C" {

void setServoPower(int32_t d){ servoPower=d; }

ANA_OUT("Servo_power", "%", "0", "100", 0, 255, setServoPower ,setServoPowerWidget);

ANA_IN("Accelerometer_X", "g", "-2", "2", -32768, 32767, accelerometerX);
ANA_IN("Accelerometer_Y", "g", "-2", "2", -32768, 32767, accelerometerY);
ANA_IN("Accelerometer_Z", "g", "-2", "2", -32768, 32767, accelerometerZ);

ANA_IN("Magnetometer_X", "gauss", "-1.3", "1.3", -32768, 32767, magnetometerX);
ANA_IN("Magnetometer_Y", "gauss", "-1.3", "1.3", -32768, 32767, magnetometerY);
ANA_IN("Magnetometer_Z", "gauss", "-1.3", "1.3", -32768, 32767, magnetometerZ);

ANA_IN("Gyro_X", "deg/s", "-125", "125", -32768, 32767, gyroX);
ANA_IN("Gyro_Y", "deg/s", "-125", "125", -32768, 32767, gyroY);
ANA_IN("Gyro_Z", "deg/s", "-125", "125", -32768, 32767, gyroZ);

ANA_IN("Temperature", "C", "-128", "127", -128, 127, temperature);
ANA_IN("Pressure", "pa", "-128", "127", -128, 127, pressure);

const CorbomiteEntry last PROGMEM = {LASTTYPE, "", 0};
const EventData initEvent PROGMEM = {registeredEntries};

const CorbomiteEntry initcmd PROGMEM = 
	{EVENT_OUT, internalId, (CorbomiteData*)&initEvent};

const CorbomiteEntry * const entries[] PROGMEM = {
	&setServoPowerWidget,
	&accelerometerX,
	&accelerometerY,
	&accelerometerZ,
	&magnetometerX,
	&magnetometerY,
	&magnetometerZ,
	&gyroX,
	&gyroY,
	&gyroZ,
	&temperature,
	&pressure,
	&initcmd, &last
};

} //extern "C"
Servo rotserv;
calibrationCoefficient_t cal;
calibrationCoefficient_t cal2;
void printValue(char *name, float value)
{
  Serial.print(name);
  if (value > 0)
   Serial.print("+");
  Serial.print(value);
}


void printHex(char *buf, uint16_t n){
	uint8_t b=0;
	while(n--){
		b++;
		Serial.print((uint8_t)*buf++, HEX);
		if(b%4 == 0)
			Serial.print(" ");
		if(b == 8){
			b = 0;
			Serial.println("");
		}
	}
}

void print3(vector3 d){
	Serial.print("X: ") ; Serial.print(d.x);
	Serial.print(" Y: ") ; Serial.print(d.y);
	Serial.print(" Z: ") ; Serial.print(d.z);
}

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  rotserv.attach(9);
  initLsm303(0x1D);
  initL3gd20(0x6B);
  initBmp180(0x77, &cal);
}

void loop()
{ 
	vector3 d;
	int16_t temp;

	if(readAccelerometerData(0x1d, &d) == 0){
		transmitAnalogIn(&accelerometerX, d.x);
		transmitAnalogIn(&accelerometerY, d.y);
		transmitAnalogIn(&accelerometerZ, d.z);
	}

	if(readMagnetometerData(0x1d, &d) == 0){
		transmitAnalogIn(&magnetometerX, d.x);
		transmitAnalogIn(&magnetometerY, d.y);
		transmitAnalogIn(&magnetometerZ, d.z);
	}

	if(readGyroData(0x6b, &d) == 0){
		transmitAnalogIn(&gyroX, d.x);
		transmitAnalogIn(&gyroY, d.y);
		transmitAnalogIn(&gyroZ, d.z);
	}

	transmitAnalogIn(&temperature, compensateTemperature(readTemperatureUncal(0x77), &cal));
	transmitAnalogIn(&pressure, readPressureUncal(0x77,0));

	/*if(readMagnetometerData(0x1d, &d) == 0){
		Serial.print("Magnetometer "); print3(d);
		//Serial.println("");
	}
	if(readAccelerometerData(0x1d, &d) == 0){
		Serial.print("Accelerometer "); print3(d);
		//Serial.println("");
	}
	if(readGyroData(0x6b, &d) == 0){
		Serial.print("gyro "); print3(d);
		Serial.println("");
	}*/
	/*Serial.print("status register 0x");
	Serial.print(readRegister(0x6b, 0x27), HEX);
	Serial.println("");
	Serial.print("temp register 0x");
	Serial.print(readRegister(0x6b, 0x26), HEX);
	Serial.println("");
	Serial.print("ctl1 register 0x");
	Serial.print(readRegister(0x6b, 0x20), HEX);
	Serial.println("");*/
	//delay(200);
	//LSM303D 0x1D Accelerometer and magnetometer
	//BMP180 0x77 Pressure sensor
	//L3GD20 0x6B Gyro
	//0x1D, 0x6B 0x77
	commandLine();
}

void platformSerialWrite(const char *buf, uint16_t len)
{
    rotserv.write(servoPower);
    Serial.write((uint8_t *)buf, len);
}

void serialEvent()
{
    while(Serial.available()){
        addCharToBuffer(Serial.read());
    }
}
 
