#include <EEPROM.h>
//#include <Servo.h>
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

typedef struct{
	int16_t value;
	int16_t a;
} iir_state_int16_t;

uint8_t servoPower = 0;
uint8_t servoEnabled = 0;
uint8_t lastServoEnabled = 1;
uint16_t azimuth = 0;
uint8_t stepperP = 5;
uint8_t stepperDir = 2;

extern "C" {

void setMotorSpeed(int32_t d){
	int32_t value;
	if(d < 0){
		digitalWrite(stepperDir, LOW);
		d = -d;
	} else {
		digitalWrite(stepperDir, HIGH);
	}
	value = 250000/d;
	OCR1A = value;	
}


void setServoEnabled(uint8_t d){servoEnabled=d;}
void setAzimuth(int32_t a){azimuth=a;};

ANA_OUT("MotorSpeed", "%", "-100", "100", -1024*2, 1024*2, setMotorSpeed,setMotorSpeedWidget);
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
	&setMotorSpeedWidget,
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



//Servo rotserv;
calibrationCoefficient_t cal;
calibrationCoefficient_t cal2;



// Interrupt is called once a millisecond, 
//SIGNAL(TIMER0_COMPA_vect) 
SIGNAL(TIMER1_COMPA_vect)
{
	static int t=0;
	if((t&0x1)==0)
		digitalWrite(stepperP, HIGH);
		digitalWrite(stepperP, LOW);
	t++;
}

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

void filter(iir_state_int16_t *fil, int16_t value){
	int32_t n = int32_t(fil->value)*(0x7fff-int32_t(fil->a))+int32_t(value)*int32_t(fil->a);
	fil->value = n >> 15;
}


void stepperPower(uint8_t state){
	if(state == 0){
		digitalWrite(8, HIGH);
		digitalWrite(12, LOW);
	} else {
		digitalWrite(8, LOW);
		digitalWrite(12, HIGH);
	}
}

void setup()
{
  pinMode(stepperP, OUTPUT);
  pinMode(stepperDir, OUTPUT);
  digitalWrite(stepperDir, LOW);
  pinMode(12, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(12, LOW);
  Wire.begin();
  Serial.begin(9600);
  //rotserv.attach(9);
  initLsm303(0x1D);
  initL3gd20(0x6B);
  initBmp180(0x77, &cal);

  //Set timer1 to 4us resolution
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 512/6;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS10) | (1<<CS11);
  TIMSK1 |= (1 <<OCIE1A);
//  OCR0A = 0xAF;
//  TIMSK0 |= _BV(OCIE0A);

	//setup 
}

void loop()
{
	static uint16_t tolerance = 0x0200;
	static uint8_t txnum = 0; 
	vector3 mag, gyr, acc;
	int16_t temp;
	float magAzf;
	int16_t magAzi;
	int16_t aziDelta;
	int16_t vel;
	static bool servoEnabledLast = false;
	static iir_state_int16_t fil{0, 0x0fff};


	//digitalWrite(12, HIGH);

	if(readAccelerometerData(0x1d, &acc) == 0){
	}

	if(readMagnetometerData(0x1d, &mag) == 0){
		magAzf = atan2(mag.x, mag.z);
		magAzi = int(32767.0*magAzf/3.141529);
		aziDelta = magAzi-azimuth;
		if(abs(aziDelta) > abs(azimuth-magAzi))
			aziDelta = azimuth-magAzi;
		filter(&fil, aziDelta);
		aziDelta = fil.value;
	}

	if(readGyroData(0x6b, &gyr) == 0){
	}
	
	if(servoEnabled && (abs(aziDelta) > tolerance/*0x0100*/)){
		tolerance = 0x0100;
		digitalWrite(LED_BUILTIN, HIGH);

		vel = -aziDelta>>0;
		//if(abs(vel) < 768){
		//	if(vel < 0)
		//		vel = -768;
		//	else
		//		vel = 768;
		//}
			
		if(vel > 1500)
			vel = 1500;
		if(vel < -1500)
			vel = -1500;

		//vel=abs(aziDelta/870)+1;

		if(aziDelta<0){
			setMotorSpeed(vel);
			transmitDigitalIn(&servoDirectionWidget,1);
		}else{
			setMotorSpeed(vel);
			transmitDigitalIn(&servoDirectionWidget,0);
		}
		stepperPower(true);
		//digitalWrite(12, HIGH);
		servoEnabledLast = true;
	} else {
		tolerance = 0x0200;
		//if(servoEnabledLast)
			//rotserv.detach();
		//digitalWrite(12, LOW);
		stepperPower(false);
		servoEnabledLast = false;
	}
		if(txBufferBytes()==0){
		switch(txnum++){
			case 0:	transmitAnalogIn(&accelerometerX, acc.x); break;
			case 1: transmitAnalogIn(&accelerometerY, acc.y); break;
			case 2: transmitAnalogIn(&accelerometerZ, acc.z); break;
			case 3: transmitAnalogIn(&magnetometerX, mag.x); break;
			case 4: transmitAnalogIn(&magnetometerY, mag.y); break;
			case 5: transmitAnalogIn(&magnetometerZ, mag.z); break;
			case 6: transmitAnalogIn(&magneticAzimuth, magAzi); break;
			case 7: transmitAnalogIn(&azimuthDelta, aziDelta); break;
			case 8: transmitAnalogIn(&gyroX, gyr.x); break;
			case 9: transmitAnalogIn(&gyroY, gyr.y); break;
			case 10: transmitAnalogIn(&gyroZ, gyr.z); break;
			case 11: transmitAnalogIn(&temperature, compensateTemperature(readTemperatureUncal(0x77), &cal)); txnum = 0;break;
			default:
				txnum = 0;
			break;
		}
	}
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
 
