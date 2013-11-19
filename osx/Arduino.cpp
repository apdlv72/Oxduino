
#include <Arduino.h>
#include <unistd.h>
#include <sys/time.h>


//TCP SERVER
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <arpa/inet.h>
//#include <fcntl.h>
#include <unistd.h>

#include <pthread.h>    /* POSIX Threads */
#include <sys/types.h>
#include <sys/socket.h>

extern void setup();
extern void loop();
extern void serialEvent();

#define NUM_DPINS 20
#define NUM_APINS  8

#include "eEEPROM.h"

// digital io pin
typedef struct
{
	volatile int mode;
	volatile int oLevel, iLevel;
	volatile boolean pwm;
	volatile byte pLevel;
}
s_dpin;

// analog input pin
typedef struct
{
	int level : 10;
} 
s_apin;

typedef struct
{
	int mode;
	void (*func)(void);
}
s_intr;

// should be shared somehow. now via socket (slow).
// consider something like http://psy-lob-saw.blogspot.de/2013/04/lock-free-ipc-queue.html
typedef struct 
{
	s_dpin dPins[NUM_DPINS];
	s_apin aPins[NUM_APINS];
	s_intr inter[NUM_DPINS];
	struct
	{
		unsigned long sequence;
		boolean pinMode   : 1;
		boolean pinLevel  : 1;
		boolean eeprom    : 1;
		boolean analogRef : 1;
		boolean intrFunc  : 1;
		boolean intrEnabl : 1;
	}
	changed;

	uint8_t eeprom_data[E2END+1];
	int     analogRef;
	boolean interrupts;
}
s_state;

s_state currState;
boolean oDumpEEProm = false;
boolean doRestart   = true;
unsigned long __micros_first = 0;


unsigned long micros(void)
{
	timeval time;
	gettimeofday(&time, NULL);
	long m = (time.tv_sec * 1000000) + time.tv_usec;
	if (!__micros_first) __micros_first=m;
	m-=__micros_first;
	//	printf("micros: %li\n", m);
	return m;
}

unsigned long millis(void) 
{
	unsigned long ms = micros()/1000;
	//printf("millis: %li\n", ms);
	return ms;
};


void delay(unsigned long ms)
{
	usleep(ms * 1000);
};


void pinMode(uint8_t pinNo, uint8_t mode)
{
	//if (13!=pinNo)
	//printf("pinMode(%i, %i)\n", pinNo, mode);
	s_dpin &pin = currState.dPins[pinNo];
	if ((currState.changed.pinMode = (pin.mode != mode)))
	{
		currState.changed.sequence = micros();
	}
	pin.mode = mode;
}


void digitalWrite(uint8_t pinNo, uint8_t level)
{
	//if (13!=pinNo)
		//printf("digitalWrite(%i, %i)\n", pinNo, level);
	s_dpin &pin = currState.dPins[pinNo];
	if ((currState.changed.pinLevel = (pin.oLevel != level) || pin.pwm!=false))
	{
		currState.changed.sequence = micros();
	}
	pin.oLevel = level;
	pin.pwm    = false;
}


byte digitalRead(uint8_t pinNo)
{
	s_dpin &pin = currState.dPins[pinNo];
	return pin.iLevel;
}


void analogWrite(uint8_t pinNo, uint8_t level)
{
	//if (13!=pinNo) printf("analogWrite(%i, %i)\n", pinNo, level);
	s_dpin &pin = currState.dPins[pinNo];
	if (((currState.changed.pinMode  = (pin.pwm != true))) || ((currState.changed.pinLevel = (pin.pLevel != level))))
	{
		currState.changed.sequence = micros();
	}
	pin.pLevel = level;
	pin.pwm    = true;
}


int analogRead(uint8_t pinNo)
{
	s_apin &pin = currState.aPins[pinNo];
	return pin.level;
}


void analogReference(int type)
{
	if ((currState.changed.analogRef = (currState.analogRef != type)))
	{
		currState.changed.sequence = micros();
	}
	currState.analogRef = type;
}


void randomSeed(unsigned int seed)
{
	srandom(seed);
}

long random(long limit)
{
	long max = ((unsigned long)-1L)/2-1;
	long val = (long)(max*random());
	return (0==limit) ? val : val%limit;
}


void attachInterrupt(uint8_t pinNo, void (*func)(void), int mode)
{
	s_intr &i = currState.inter[pinNo];
	i.mode = mode;
	i.func = func;
	currState.changed.intrFunc = true;
	currState.changed.sequence = micros();
}

void detachInterrupt(uint8_t pinNo)
{
	s_intr &i = currState.inter[pinNo];
	if ((currState.changed.intrFunc = NULL!=i.func))
	{
		currState.changed.sequence = micros();
	}
	i.func = NULL;
}

void interrupts()
{
	if ((currState.changed.intrEnabl = (currState.interrupts!=false)))
	{
		currState.changed.sequence = micros();
	}
	currState.interrupts = false;
}

void noInterrupts()
{
	if ((currState.changed.intrEnabl = (currState.interrupts!=true)))
	{
		currState.changed.sequence = micros();
	}
	currState.changed.intrEnabl = true;
}


void handleclient(int sock)
{
	char buf[1024];

	s_state lastState;

	for (;;)
	{
		memcpy(&lastState, &currState, sizeof(lastState));

		// clear console:
		send(sock, "\x1b\x5b\x48\x1b\x5b\x32\x4a", 7, 0);

		for (int i=1; i<=13; i++)
		{
			s_dpin &pin = currState.dPins[i];

			if (pin.mode==OUTPUT)
			{
				if (pin.pwm)
				{
					snprintf(buf, 1024, "%02i:  ->  %03i ", i, pin.pLevel);
					int j, n=strlen(buf), l=pin.pLevel/8;
					for (j=n; j<n+l; j++) buf[j] = '=';
					buf[j]   = '\n';
					buf[j+1] = 0;
				}
				else
				{
					snprintf(buf, 1024, "%02i:  ->  %s\n", i, pin.oLevel==HIGH ? "*" : ".");
				}
			}
			else
			{
				snprintf(buf, 1024, "%02i: %s %s\n",
						i,
						pin.mode==INPUT  ? "<-  "   : "<=  ",
						pin.iLevel==HIGH ? "*" : ".");
			}

			//printf("pin %02i: p=%p  level=%i\n", i, &pin, pin.oLevel);
			send(sock, buf, strlen(buf), 0); //MSG_NOSIGNAL);
			/*
        k = recv(temp_sock_desc,buf,100,0);     
        if(strcmp(buf,"exit")==0)        
            break;     
        if(k>0)         
            printf("%s",buf); 
			 */
		}

		if (oDumpEEProm)
		{
		int s=32;
		for (int i=0; i<=256; i+=16)
		{
			for (int n=0; n<s; n++)
			{
				if (0==n%16) send(sock, " ", 1, 0);
				uint8_t b = lastState.eeprom_data[i+n];
				snprintf(buf, 1024, "%02x ", b);
				send(sock, buf, strlen(buf), 0);
			}
			send(sock, "  ", 2, 0);

			for (int n=0; n<s; n++)
			{
				if (0==n%16) send(sock, " ", 1, 0);
				uint8_t b = lastState.eeprom_data[i+n];
				if (b<' ' || b>127)
					snprintf(buf, 1024, ".");
				else
					snprintf(buf, 1024, "%c", b);

				send(sock, buf, strlen(buf), 0);
			}
			send(sock, "\n", 1, 0);
		}
		}

		while (
				0==memcmp(&lastState, &currState, sizeof(lastState))
				//&& (!oDumpEEProm || 0==memcpy(&lastData, eEEPROM.getData(), sizeof(lastData)))
		)
		{
			usleep(20*1000);
		}
		usleep(20*1000);
	}
}


byte eeprom_read_byte(unsigned char * addr)
{
	return currState.eeprom_data[(long)addr];
}


void eeprom_write_byte(unsigned char * addr, byte value)
{
	currState.eeprom_data[(long)addr] = value;
}


void * server(void * data)
{ 
	char buf[100];
	socklen_t len;
	int k,sock_desc,temp_sock_desc;
	struct sockaddr_in client,server;
	memset(&client,0,sizeof(client));
	memset(&server,0,sizeof(server));
	sock_desc = socket(AF_INET,SOCK_STREAM,0);
	server.sin_family = AF_INET; server.sin_addr.s_addr = inet_addr("127.0.0.1");

	server.sin_port = ntohs(7777);


	int optval;
	optval=1;
	setsockopt(sock_desc,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));

	k = bind(sock_desc,(struct sockaddr*)&server,sizeof(server));
	perror("OX: bind");

	printf("OX: listening on port 7777...\n");
	k = listen(sock_desc,20);
	perror("OX: listen");
	len = sizeof(client);

	for (;;)
	{
		printf("OX: accepting clients...\n");
		temp_sock_desc = accept(sock_desc,(struct sockaddr*)&client,&len);
		printf("OX: new client connection\n");

		while(1)
		{
			handleclient(temp_sock_desc);
		}
		close(sock_desc);
	}

	close(temp_sock_desc);
}


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

/*
void init_serial(int *fd, unsigned int baud)
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
*/

#include "Serial.h"

int main(int argc, char * argv[])
{
	       if (argc>1)
	       {
	               const char * tty = argv[1];
	               printf("OX: Opening %s\n", tty);
	               int fd;
	
	               //fd = open("/dev/tty.SeriellerAnschluss", O_RDWR | O_NOCTTY | O_NDELAY);
	               fd = open(tty, O_RDWR | O_NOCTTY | O_NDELAY);
	               printf("OX: Opened RS232 on fd %i\n", fd);
	
	               if (fd == -1)
	               {
	            	   perror("OX: open_port: unable to open port");
	            	   exit(2);
	               }
	
	               init_port(&fd,9600);         //set serial port to 9600,8,n,1
	               write(fd, "H\n", 2);

	               close(0);
	               close(1);
	               dup(fd);
	               dup(fd);
	               
	               fprintf(stderr, "OX: DO YOU SEE STDOUT?\n");
	               fprintf(stderr, "OX: DO YOU SEE STDERR?\n");
	               fflush(stdout);
	       }
	

	pthread_t server_thread;

	pthread_create (&server_thread, NULL, server, NULL);

	fflush(stderr);
	usleep(10*1000);
	setup();

	fprintf(stderr, "OX: entering event loop\n");
	fflush(stderr);
	sleep(2);

	while (doRestart)
	{
		doRestart = false;

		setup();
		for (;!doRestart;)
		{
			loop();
			serialEvent();
		}
	}
}


