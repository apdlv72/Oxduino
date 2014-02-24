
#include "Serial.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>

#include <stdlib.h>
#include <syslog.h>
#include <signal.h>

#include <CoreFoundation/CoreFoundation.h>
#include <SystemConfiguration/SystemConfiguration.h>

#include <IOKit/IOKitLib.h>
#include <IOKit/pwr_mgt/IOPMLib.h>
#include <IOKit/ps/IOPSKeys.h>
#include <IOKit/ps/IOPowerSources.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/hid/IOHIDKeys.h>
#include <IOKit/hid/IOHIDLib.h>

#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/IOBSD.h>

#include <unistd.h>

HardwareSerial Serial(0,0,0,0,0,0,0,0,0,0,0,0,0);


HardwareSerial::HardwareSerial(ring_buffer *rx_buffer, ring_buffer *tx_buffer,
  volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
  volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
  volatile uint8_t *ucsrc, volatile uint8_t *udr,
  uint8_t rxen, uint8_t txen, uint8_t rxcie, uint8_t udrie, uint8_t u2x)
{
}


int HardwareSerial::available(void) { 
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


int HardwareSerial::peek()
{
  return 0; //stdin.peek();
}

int HardwareSerial::read(void)
{ 
  return getchar(); 
}
    
void HardwareSerial::flush(void)
{
  fflush(stdout);
}
    
size_t HardwareSerial::write(uint8_t i)
{
  return printf("%c", i);
  //return write(1, &i, 1);
}

/*
size_t HardwareSerial::write(const uint8_t *buffer, size_t size)
{
  return -1; //write(1, buffer, size);
  //return write(1, &i, 1);
}
*/

void init_port(int *fd, unsigned int baud)
{
    struct termios options;
    tcgetattr(*fd,&options);
    switch(baud)
    {
       case  9600: cfsetispeed(&options, B9600); cfsetospeed(&options, B9600);  break;
       case 19200: cfsetispeed(&options,B19200); cfsetospeed(&options,B19200); break;
       case 38400: cfsetispeed(&options,B38400); cfsetospeed(&options,B38400); break;
      default:     cfsetispeed(&options, B9600); cfsetospeed(&options, B9600); break;
    }
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    tcsetattr(*fd,TCSANOW,&options);
}






