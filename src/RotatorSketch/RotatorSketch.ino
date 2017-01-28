#include <EEPROM.h>
#include <Servo.h>
#include <Wire.h>
#include "lsm303.h"
#include "l3gd20.h"
#include "bmp180.h"
#include "i2c.h"
#include "math.h"

extern "C"
{
#include "uart_command_lib.h"
#include "corbomite.h"
}



uint8_t servoPower = 0;
uint8_t servoEnabled = 0;
uint8_t lastServoEnabled = 0;
uint16_t azimuth = 0;


extern "C" {

void setServoPower(int32_t d){ servoPower=d;}
void setServoEnabled(uint8_t d){servoEnabled=d;}
void setAzimuth(int32_t a){azimuth=a;};

ANA_OUT("Servo_power", "%", "0", "100", 0, 255, setServoPower,setServoPowerWidget);
ANA_OUT("Azimuth", "deg", "-180", "180", -32768, 32767, setAzimuth, setAzimuthWidget);
ANA_IN("Azi_delta", "deg", "-180", "180", -32768, 32767, azimuthDelta);
DIG_OUT("Servo_enabled", setServoEnabled, servoEnabledWidget);
DIG_IN("Servo_direction", servoDirectionWidget);
ANA_IN("Accelerometer_X", "g", "-2", "2", -32768, 32767, accelerometerX);
ANA_IN("Accelerometer_Y", "g", "-2", "2", -32768, 32767, accelerometerY);
ANA_IN("Accelerometer_Z", "g", "-2", "2", -32768, 32767, accelerometerZ);

ANA_IN("Magnetometer_X", "gauss", "-1.3", "1.3", -32768, 32767, magnetometerX);
ANA_IN("Magnetometer_Y", "gauss", "-1.3", "1.3", -32768, 32767, magnetometerY);
ANA_IN("Magnetometer_Z", "gauss", "-1.3", "1.3", -32768, 32767, magnetometerZ);
ANA_IN("Magnetic_azimuth", "deg", "-180", "180", -32768, 32767, magneticAzimuth);
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
	&setAzimuthWidget,
	&azimuthDelta,
	&servoEnabledWidget,
	&servoDirectionWidget,
	&accelerometerX,
	&accelerometerY,
	&accelerometerZ,
	&magnetometerX,
	&magnetometerY,
	&magnetometerZ,
	&magneticAzimuth,
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
  pinMode(12, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
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
	float magAzf;
	int16_t magAzi;
	int16_t aziDelta;


	if(readAccelerometerData(0x1d, &d) == 0){
		transmitAnalogIn(&accelerometerX, d.x);
		transmitAnalogIn(&accelerometerY, d.y);
		transmitAnalogIn(&accelerometerZ, d.z);
	}

	if(readMagnetometerData(0x1d, &d) == 0){
		transmitAnalogIn(&magnetometerX, d.x);
		transmitAnalogIn(&magnetometerY, d.y);
		transmitAnalogIn(&magnetometerZ, d.z);
		magAzf = atan2(d.x, d.z);
		magAzi = int(32767.0*magAzf/3.141529);
		transmitAnalogIn(&magneticAzimuth, magAzi);
		aziDelta = magAzi-azimuth;
		transmitAnalogIn(&azimuthDelta, aziDelta);
	}

	if(readGyroData(0x6b, &d) == 0){
		transmitAnalogIn(&gyroX, d.x);
		transmitAnalogIn(&gyroY, d.y);
		transmitAnalogIn(&gyroZ, d.z);
	}
	
	if(servoEnabled && (abs(aziDelta) > 0x0200)){
		digitalWrite(LED_BUILTIN, HIGH);
		//rotserv.write(servoPower);
		if(aziDelta<0){
			rotserv.write(90);
			transmitDigitalIn(&servoDirectionWidget,1);
		}else{
			rotserv.write(86);
			transmitDigitalIn(&servoDirectionWidget,0);
		}
		digitalWrite(12, HIGH);
	} else {
		digitalWrite(LED_BUILTIN, LOW);
		digitalWrite(12, LOW);
	}

	transmitAnalogIn(&temperature, compensateTemperature(readTemperatureUncal(0x77), &cal));
	commandLine();
}

void platformSerialWrite(const char *buf, uint16_t len)
{
    Serial.write((uint8_t *)buf, len);
}

void serialEvent()
{
    while(Serial.available()){
        addCharToBuffer(Serial.read());
    }
}
 
