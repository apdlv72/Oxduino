
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




HardwareSerial Serial;


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





