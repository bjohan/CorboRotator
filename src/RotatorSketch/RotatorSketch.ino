#include <EEPROM.h>
#include <Servo.h>
#include <Wire.h>

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
void setup()
{
  Serial.begin(9600);
	rotserv.attach(9);
}


uint8_t checkAddress(uint8_t addr){
	Wire.beginTransmission(addr);
	return Wire.endTransmission() == 0;
}

uint8_t readRegister(uint8_t devaddr, uint8_t regaddr){
	uint8_t data;
	Wire.beginTransmission(devaddr);
	Wire.write(regaddr);
	
	Wire.requestFrom((int)devaddr, (int)1);
	data = Wire.read();
	Wire.endTransmission();
	return data;
}

void printValue(char *name, float value)
{
  Serial.print(name);
  if (value > 0)
   Serial.print("+");
  Serial.print(value);
}


void loop()
{ 
	//LSM303D 0x1D Accelerometer and magnetometer
	//BMP180 0x77 Pressure sensor
	//L3GD20 0x6B Gyro
	//0x1D, 0x6B 0x77
	/*if(checkAddress(0x1D))
		Serial.println("Device present");
	else
		Serial.println("no device");
		
        for(int i = 0 ; i <255 ; i++){
		printValue("addr", i);
		printValue("Acc", readRegister(0x77, i));
		Serial.println("");
	}*/
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
 
