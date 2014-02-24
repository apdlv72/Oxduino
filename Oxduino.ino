
#include <Arduino.h>

void serialEvent() {}
void setup() 
{
	pinMode(13, OUTPUT);
	Serial.begin(56000);
	Serial.println("Hello World");
	Serial.println(4711, DEC);
	Serial.println(65535, HEX);
}

#define ABS(A) ( ((A)<0) ? (-(A)) : (A) )

void loop() 
{
	int value = millis()%3000;
	value = value<511 ? 255-ABS(value-255) : value<1023 ? 255-ABS((value-511)-255) : 0;
	analogWrite(13, value);
	delay(10);
}
