
#ifndef MYEEPROM_h
#define MYEEPROM_h

#include <inttypes.h>
#include <avr/eeprom.h>

#include <stdio.h>


//#undef eEE_ADDR
//#define eEE_ADDR(EETOKEN) ((uint64_t)(void*)&(EETOKEN))

#define ADDR_T uint64_t

/*
 * Structure to store integer values round robin.
 * The MSB bit defines which value is the current one and the remaining bits store the actual value.
 * Using an array of such structures and moving the current flag to the next element upon every
 * change will spread write cycles to the EEProm equally over the whole array.
 * This improves lifetime of the EEProm by N/2 where N is the number of array elements
 * (Not by N, because the former value needs to be invalidated).
 * Static member TYPE  is actually only a hint that allow the the template methods to derive
 * the correct type of the member "value". It should NOT eat up any valuable RAM nor EEProm
 * space since it is static, moreover its value never read.
 */
typedef struct { int8_t  current : 1; int8_t  value :  7; static int8_t  TYPE; } s_rrint7;
typedef struct { int16_t current : 1; int16_t value : 15; static int16_t TYPE; } s_rrint15;
typedef struct { int32_t current : 1; int32_t value : 31; static int32_t TYPE; } s_rrint31;
typedef struct { uint8_t current; uint8_t data; } s_rrstruct;



class eEEPROMClass
{
  public:
    uint8_t read(int a) { return data[a]; };
    void write(int a, uint8_t v) { data[a] = v; };

     uint8_t * getData() 
    { 
      return data; 
    }

uint16_t readWord(int address)
{
	uint8_t hi = read(address  );
	uint8_t lo = read(address+1);
	return (hi<<8) | lo;
}

void writeWord(int address, uint16_t value)
{
	write(address,   value >>   8); // hi
	write(address+1, value & 0xff); // lo
}



void readData(int addr, void * buf, int len)
{
	for (uint8_t * b=(uint8_t*)buf; len>0; len--, addr++, b++)
	{
		*b = read(addr);
	}	
}


void writeData(const char * file, const char * func, int line, int addr, const void * buf, int len)
{
  //printf("%s:%s:%i: ", file, func, line);
  writeData(addr, buf, len);
}

void writeData(int addr, const void * buf, int len)
{
//        printf("writeData: addr=%i, buf=%p (%02x), len=%i\n", addr, buf, *(unsigned char*)buf, len); fflush(stdout);
        
	for (const uint8_t * b=(uint8_t*)buf; len>0; len--, addr++, b++)
	{
		write(addr, *b);
	}	
}

void memFill(int addr, uint8_t data, uint16_t len)
{
	for (; len>0; len--, addr++)
	{
		write(addr, data);
	}
}


    template <class INT> INT numRead(INT * piEE)
    {
      INT    iRam = 0;
      ADDR_T addr = (ADDR_T)piEE;
      readData(addr, (void*)&iRam, sizeof(iRam));
      return iRam;
    }

    // const INT to allow const definition of eeprom struct pointer: 
    //   const s_eeprom_data * EEPROM = 0;
    template <class INT> void numWrite(const INT * piEE, INT iRam)
    {
      printf("numWrite: piEE=%p, iRam=%i\n", piEE, iRam); fflush(stdout);
      ADDR_T addr = (ADDR_T)piEE;
      writeData(addr, (void*)&iRam, sizeof(iRam));
      printf("numWrite: done\n"); fflush(stdout);
    }


    void showPgmString (const char * str)
    {
      printf("%s", str);
    }
    
    
  private:
    uint8_t data[E2END+1];
};

extern eEEPROMClass eEEPROM;


#define eEE_ADDR(EETOKEN) ((ADDR_T)(void*)&(EETOKEN))
  #define eEE_WRITE(SRC, EETOKEN) { eEEPROM.writeData(__FILE__, __func__, __LINE__, eEE_ADDR(EETOKEN), &(SRC),sizeof(EETOKEN)); }
#define eEE_READ(EETOKEN, DEST) { eEEPROM.readData( eEE_ADDR(EETOKEN), &(DEST), sizeof(DEST));    }
#define eEE_ZERO(EETOKEN)       { eEEPROM.memFill(eEE_ADDR(EETOKEN), 0, sizeof(EETOKEN)); }

#define eEE_CHECKSIZE(DATA) struct FailOnEEPromExceess { int c[E2END-sizeof(DATA)]; };

//#define PPRINT(TEXT) eEEPROM.showPgmString(PSTR(TEXT))
#define PPRINT(TEXT) eEEPROM.showPgmString(TEXT)

/*
 * Macros that define round robin integer arrays with user defined size;
 */
#define eEE_rrint7_t(NAME,COUNT)  s_rrint7  NAME[COUNT]
#define eEE_rrint15_t(NAME,COUNT) s_rrint15 NAME[COUNT]
#define eEE_rrint31_t(NAME,COUNT) s_rrint31 NAME[COUNT]

/*
 * Same for arbitrary user structures.
 */
#define eEE_rrstruct(NAME,COUNT,DEF) struct NAME { uint8_t current; DEF data; } NAME[COUNT]

/*
 * Read  write integers (maps to template methods numRead/numWrite)
 */
#define eEE_READN(NAME)        eEEPROM.numRead(&(NAME))
#define eEE_WRITEN(NAME,VALUE) eEEPROM.numWrite(&(NAME), (VALUE))

/*
 * Macros to transparently init/read/write such round robin arrays.
 */
#define eEE_WRITERRI(NAME,VALUE) eEEPROM.rrWrite(NAME, sizeof(NAME)/sizeof(NAME[0]), VALUE)
#define eEE_READRRI(NAME)        eEEPROM.rrRead( NAME, sizeof(NAME)/sizeof(NAME[0]))

#define eEE_WRITERRS(NAME,SOURCE) eEEPROM.rrWriteStruct((s_rrstruct*)&(NAME), sizeof(NAME), sizeof(NAME)/sizeof(NAME[0]), &(SOURCE));
#define eEE_READRRS (NAME,TARGET) eEEPROM.rrReadStruct( (s_rrstruct*)&(NAME), sizeof(NAME), sizeof(NAME)/sizeof(NAME[0]),  &(TARGET));

/* TODO: remove obsolete naming after porting examples
#define eEE_RRWRITE eEE_WRITERRI
#define eEE_RRREAD  eEE_READRRI
#define eEE_RRWRITESTRUCT eEE_WRITERRS 
#define eEE_RRREADSTRUCT  eEE_READRRS
*/

#endif
