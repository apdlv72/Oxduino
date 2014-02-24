
//#warning osx/avr/pgmspace.h included

#ifdef __ATTR_PROGMEM__
  #undef __ATTR_PROGMEM__
#endif

#define   __ATTR_PROGMEM__

//#ifndef __ATTR_PROGMEM__
//#define __ATTR_PROGMEM__ __attribute__((__progmem__)) 
//#endif

//#ifndef __ATTR_PURE__
//#define __ATTR_PURE__ __attribute__((__pure__))
//#endif   

/**
   \ingroup avr_pgmspace
   \def PROGMEM

   Attribute to use in order to declare an object being located in
   flash ROM.
 */
#define PROGMEM __ATTR_PROGMEM__

