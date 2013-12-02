
#ifndef Arduino_h
#define Arduino_h

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "avr/eeprom.h"

extern uint8_t doRestart;

typedef unsigned char boolean;
typedef signed char 	int8_t;
typedef unsigned char 	uint8_t;
//typedef signed int 	int16_t;
//typedef unsigned int 	uint16_t;
//typedef signed long int 	int32_t;
//typedef unsigned long int 	uint32_t;
typedef signed long long int 	int64_t;
typedef unsigned long long int 	uint64_t;

typedef uint8_t byte;

typedef const char * PSTR;
typedef const char * PGM_P;

#define DEC 10
#define HEX 16
#define BIN  2

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define abs(x) ((x)>0?(x):-(x))

#define LOW  0x0
#define HIGH 0x1

#define INPUT  0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

#include "Serial.h"

unsigned long micros(void);
unsigned long millis(void);
void delay(unsigned long ms);


void pinMode(uint8_t, uint8_t);

void digitalWrite(uint8_t pinNo, uint8_t value);
byte digitalRead(uint8_t pinNo);

void analogWrite(uint8_t pinNo, uint8_t value);
int analogRead(uint8_t pinNo);

byte eeprom_read_byte(unsigned char * addr);
void eeprom_write_byte(unsigned char * addr, byte value);

#define pgm_read_byte(address_short) (*(address_short))

void randomSeed(unsigned int);
long random(long);

void attachInterrupt(uint8_t pinNo, void (*func)(void), int mode);
void detachInterrupt(uint8_t interruptNum);

void  interrupts();
void  noInterrupts();


#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bit(b) (1UL << (b))


#endif

