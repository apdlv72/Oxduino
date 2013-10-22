
#ifndef HardwareSerial_h
#define HardwareSerial_h

//#include <Stream.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include "Arduino.h"


struct ring_buffer;

class HardwareSerial //: public Print // Stream
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
  public:
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

	struct termios old_tio, new_tio;

#if 0
    HardwareSerial(ring_buffer *rx_buffer, ring_buffer *tx_buffer,
      volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
      volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
      volatile uint8_t *ucsrc, volatile uint8_t *udr,
      uint8_t rxen, uint8_t txen, uint8_t rxcie, uint8_t udrie, uint8_t u2x);
#endif      
    void begin(unsigned long) {};
    void begin(unsigned long, uint8_t) {};
    void end();

    /* virtual */  
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

    /* virtual */  int peek(void);
    /* virtual */  int read(void) { return getchar(); }
#if 0
    /* virtual */  void flush(void);
    /* virtual */  size_t write(uint8_t);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    //using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool();
#endif
    
        //size_t print(const __FlashStringHelper *);
    //size_t print(const String &);
    size_t print(const char c[]) { printf("%s", c); fflush(stdout); };
    size_t print(char c) { printf("%c", c); fflush(stdout); };
    size_t print(unsigned char c, int base = DEC) { printNumber(c, base); };
    size_t print(int i, int = DEC) { printf("%02i", i); fflush(stdout); };
    size_t print(unsigned int i, int base = DEC) { printNumber(i, base);  };
    size_t print(long i, int = DEC) { printf("%02li", i); fflush(stdout); };
    size_t print(unsigned long i, int base = DEC) { printNumber(i, base);};
    size_t print(double d, int = 2) { printf("%2.2f", d); fflush(stdout); };
    //size_t print(const char * s);
    //size_t print(const Printable&);

    //ize_t println(const __FlashStringHelper *);
    //size_t println(const String &s);
    size_t println(const char c[]) { printf("%s\n", c); fflush(stdout); };
    size_t println(char c) { printf("%c\n", c); fflush(stdout); };
    size_t println(unsigned char c, int base = DEC) { printNumber(c, base); printf("\n"); fflush(stdout); };
    size_t println(int i, int base = DEC) { printNumber(i, base); printf("\n"); fflush(stdout); };
    size_t println(unsigned int i, int base = DEC) { printNumber(i, base); printf("\n"); fflush(stdout); };
    size_t println(long i, int base = DEC) { printNumber(i, base); printf("\n"); fflush(stdout); };
    size_t println(unsigned long i, int base = DEC) { printNumber(i, base); printf("\n"); fflush(stdout); };
    size_t println(double d, int = 2) { printf("%2.2f\n", d); fflush(stdout); };
    //size_t println(const char * s);
    //size_t println(const Printable&);
    size_t println(void) { printf("\n"); fflush(stdout); fflush(stdout); };


  size_t printNumber(unsigned long n, uint8_t base) {
    char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
    char *str = &buf[sizeof(buf) - 1];
    
     *str = '\0';

     if (10==base && n<10) printf("0");
         
     // prevent crash if called with base == 1
     if (base < 2) base = 10;
             
     do {
       unsigned long m = n;
       n /= base;
       char c = m - base * n;
       *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while(n);

              
    int rc = printf("%s", str);
    fflush(stdout);
    return rc;
  }
                                    
                                    
};

extern HardwareSerial Serial;
//HardwareSerial Serial;

extern void serialEventRun(void) __attribute__((weak));

#endif
