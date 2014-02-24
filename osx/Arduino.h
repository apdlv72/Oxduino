
#ifndef Arduino_h
#define Arduino_h

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "avr/eeprom.h"

// for int8_t etc.:
#include "Oxduino.h"

#include "Serial.h"

extern uint8_t doRestart;


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

// clear/set interrupts
void cli();
void sei();

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bit(b) (1UL << (b))


extern char *itoa(int __val, char *__s, int __radix);
extern char *ltoa(long int __val, char *__s, int __radix);
extern char *utoa(unsigned int __val, char *__s, int __radix);
extern char *ultoa(unsigned long int __val, char *__s, int __radix);

extern long atol(const char *__s);
extern int atoi(const char *__s);
extern double atof(const char *__nptr);

#endif


