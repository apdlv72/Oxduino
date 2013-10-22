
#ifndef Arduino_h
#define Arduino_h

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "avr/eeprom.h"

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

typedef const char* PSTR;
typedef const char * PGM_P;


#define DEC 10
#define HEX 16
#define BIN 2

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define abs(x) ((x)>0?(x):-(x))

#define LOW  0x0
#define HIGH 0x1

#define INPUT  0x0
#define OUTPUT 0x1


void digitalWrite(uint8_t, uint8_t);

unsigned long millis(void);
void delay(unsigned long ms); 

#define pgm_read_byte(address_short) (*(address_short)) 

void pinMode(uint8_t, uint8_t);

#endif
