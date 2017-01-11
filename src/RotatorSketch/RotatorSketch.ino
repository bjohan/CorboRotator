#include <EEPROM.h>
#include <Servo.h>

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
	rotserv.attach(9);
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
 
