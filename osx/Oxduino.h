
#ifndef Oxduino_h
#define Oxduino_h

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include "avr/eeprom.h"

typedef unsigned char   boolean;
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

#define MAX(A,B) (((A)>(B)) ? (A) : (B))
#define MIN(A,B) (((A)<(B)) ? (A) : (B))

#define CHANGE 1
#define FALLING 2
#define RISING 3

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

extern char *itoa(int __val, char *__s, int __radix);
extern char *ltoa(long int __val, char *__s, int __radix);
extern char *utoa(unsigned int __val, char *__s, int __radix);
extern char *ultoa(unsigned long int __val, char *__s, int __radix);

extern long atol(const char *__s);
extern int atoi(const char *__s);
extern double atof(const char *__nptr);

#endif
