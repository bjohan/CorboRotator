#include <EEPROM.h>
#include <Servo.h>
#include <Wire.h>

typedef struct{
	int16_t x;
	int16_t y;
	int16_t z;
} vector3;


extern "C"
{
#include "uart_command_lib.h"
#include "corbomite.h"
}



uint8_t servoPower = 0;

int32_t readTemperature = 0;
extern "C" {

void setServoPower(int32_t d){ servoPower=d; }

ANA_OUT("Servo_power", "%", "0", "100", 0, 255, setServoPower ,setServoPowerWidget);


const CorbomiteEntry last PROGMEM = {LASTTYPE, "", 0};
const EventData initEvent PROGMEM = {registeredEntries};

const CorbomiteEntry initcmd PROGMEM = 
	{EVENT_OUT, internalId, (CorbomiteData*)&initEvent};

const CorbomiteEntry * const entries[] PROGMEM = {
	&setServoPowerWidget,
	&initcmd, &last
};

} //extern "C"
Servo rotserv;

uint8_t checkAddress(uint8_t addr){
	Wire.beginTransmission(addr);
	return Wire.endTransmission() == 0;
}

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
	unsigned t0 = millis();
	while(Wire.available() < n){
		if(millis() > t0+10000)
			return -1;
	}
	
	for(int i = 0 ; i < n ; i++){
		buf[i] = Wire.read();
	}
	return 0;
}

uint8_t readAccelerometerData(uint8_t devAddr, vector3 *out){
	if(readManyRegisters((char *) out, devAddr, 0x28|(1<<7), 6)!=0){
		Serial.println("Failed to read from sensor");
		return -1;
	}
 	return 0;
}

uint8_t readMagnetometerData(uint8_t devAddr, vector3 *out){
	if(readManyRegisters((char *) out, devAddr, 0x08|(1<<7), 6)!=0){
		Serial.println("Failed to read from sensor");
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


void printValue(char *name, float value)
{
  Serial.print(name);
  if (value > 0)
   Serial.print("+");
  Serial.print(value);
}


void print3(vector3 d){
	Serial.print("X: ") ; Serial.print(d.x);
	Serial.print(" Y: ") ; Serial.print(d.y);
	Serial.print(" Z: ") ; Serial.print(d.z);
}

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  rotserv.attach(9);
  initLsm303(0x1D);
}

void loop()
{ 
	vector3 d;
	
	if(readMagnetometerData(0x1d, &d) == 0){
		Serial.print("Magnetometer "); print3(d);
		//Serial.println("");
	}
	if(readAccelerometerData(0x1d, &d) == 0){
		Serial.print("Accelerometer "); print3(d);
		Serial.println("");
	}
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
 
