
#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <Stream.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include "Oxduino.h"


struct ring_buffer;

void init_port(int *fd, unsigned int baud);


class HardwareSerial : public Stream
{
  private:
    ring_buffer *_rx_buffer;
    ring_buffer *_tx_buffer;
    volatile uint8_t *_ubrrh;
    volatile uint8_t *_ubrrl;
    volatile uint8_t *_ucsra;
    volatile uint8_t *_ucsrb;
    volatile uint8_t *_ucsrc;
    volatile uint8_t *_udr;
    uint8_t _rxen;
    uint8_t _txen;
    uint8_t _rxcie;
    uint8_t _udrie;
    uint8_t _u2x;
    bool transmitting;
    struct termios old_tio, new_tio;
    
  public:
    #if 0
    HardwareSerial()
    {
    	tcgetattr(STDIN_FILENO,&old_tio);
    	new_tio=old_tio;
    	new_tio.c_lflag &=(~ICANON & ~ECHO);
    	tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);
    	
    };

    ~HardwareSerial()
    {
    	/* restore the former settings */
    	tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);
    }
    #endif
    
    HardwareSerial(ring_buffer *rx_buffer, ring_buffer *tx_buffer,
      volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
      volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
      volatile uint8_t *ucsrc, volatile uint8_t *udr,
      uint8_t rxen, uint8_t txen, uint8_t rxcie, uint8_t udrie, uint8_t u2x);
    void begin(unsigned long) {};
    void begin(unsigned long, uint8_t) {};
    void end();

    /*
    int available(void) { 
      struct timeval tv;
      fd_set fds;
      tv.tv_sec = 0;
      tv.tv_usec = 0;
      FD_ZERO(&fds);
      FD_SET(STDIN_FILENO, &fds);
      int rc = select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
      
      //printf("select returned %i\n", rc);
      
      return (FD_ISSET(0, &fds));

    } ;
    */
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool();
    
    //size_t print(const __FlashStringHelper *);
    //size_t print(const String &);
    size_t print(const char c[])                    { int j=printf("%s", c); fflush(stdout); return j; };
    size_t print(char c)                            { int j=printf("%c", c); fflush(stdout); return j; };
    size_t print(unsigned char c, int base = DEC)   { return printNumber(c, base, 0); };
    size_t print(int i, int base = DEC)             { return printNumber(i, base, 0); };
    size_t print(unsigned int i, int base = DEC)    { return printNumber(i, base, 0); };
    size_t print(long i, int base = DEC)            { return printNumber(i, base, 0); };
    size_t print(unsigned long i, int base = DEC)   { return printNumber(i, base, 0); };
    size_t print(double d, int prec = 2)            { return printDouble(d, prec, false); };
    size_t print(float f, int prec = 2)             { return printDouble(f, prec, false);  };
    //size_t print(const char * s);
    //size_t print(const Printable&);

    //size_t println(const __FlashStringHelper *);
    //size_t println(const String &s);
    size_t println(const char c[])                  { int j=printf("%s\n", c); fflush(stdout); return j; };
    size_t println(char c)                          { int j=printf("%c\n", c); fflush(stdout); return j; };
    size_t println(unsigned char c, int base = DEC) { return printNumber(c, base, 1); };
    size_t println(int i, int base = DEC)           { return printNumber(i, base, 1); };
    size_t println(unsigned int i, int base = DEC)  { return printNumber(i, base, 1); };
    size_t println(long i, int base = DEC)          { return printNumber(i, base, 1); };
    size_t println(unsigned long i, int base = DEC) { return printNumber(i, base, 1); };
    size_t println(double d, int prec = 2)          { return printDouble(d, prec, true); };
    size_t println(float f, int prec = 2)           { return printDouble(f, prec, true); };
    //size_t println(const char * s);
    //size_t println(const Printable&);
    size_t println(void) { int j=printf("\n"); fflush(stdout); fflush(stdout); return j; };

    
    size_t printDouble(double d, unsigned int prec, boolean cr)
    {
      char format[16];
      sprintf(format, "%%1.%if", prec);
      
      int rc = printf(format, d);
      if (cr) rc+=printf("\n");
      
      fflush(stdout);
      return rc;
    }

    size_t printNumber(unsigned long n, uint8_t base) {
    	return printNumber(n, base, false);
    }

  size_t printNumber(long n, uint8_t base, boolean cr) {
     if (n<0)
     {
       printf("-");
       n=-n;
    }
    return 1+printNumberPos(n,base,cr); 
  }
  
  size_t printNumberPos(unsigned long n, uint8_t base, boolean cr) {
    char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
    char *str = &buf[sizeof(buf) - 1];

     *str = '\0';

     //if (10==base && n<10) printf("0");
         
     // prevent crash if called with base == 1
     if (base < 2) base = 10;
             
     do {
       unsigned long m = n;
       n /= base;
       char c = m - base * n;
       *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while(n);

              
    int rc = printf("%s", str);
    if (cr) rc+=printf("\n");

    fflush(stdout);
    return rc;
  }
                                    
                                    
};

extern HardwareSerial Serial;
//HardwareSerial Serial;

extern void serialEventRun(void) __attribute__((weak));

#endif
