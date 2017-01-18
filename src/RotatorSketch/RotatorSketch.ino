#include <EEPROM.h>
#include <Servo.h>
#include <Wire.h>
#include "lsm303.h"
#include "l3gd20.h"
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

void printValue(char *name, float value)
{
  Serial.print(name);
  if (value > 0)
   Serial.print("+");
  Serial.print(value);
}

void setup()
{
  uint8_t whoami;
  Wire.begin();
  Serial.begin(115200);
  rotserv.attach(9);
  initLsm303(0x1D);
  initL3gd20(0x6B);
  Serial.print("whoamireg has value ");
  Serial.print(whoami, HEX);
  Serial.println("");


}

void print3(vector3 d){
	Serial.print("X: ") ; Serial.print(d.x);
	Serial.print(" Y: ") ; Serial.print(d.y);
	Serial.print(" Z: ") ; Serial.print(d.z);
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
		//Serial.println("");
	}
	if(readGyroData(0x6b, &d) == 0){
		Serial.print("gyro "); print3(d);
		Serial.println("");
	}
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
 
