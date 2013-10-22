
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
#define NUM_APINS 20

#include "eEEPROM.h"

// digital io pin
typedef struct
{
  volatile int mode;
  volatile int oLevel, iLevel;
  volatile boolean pwm;
  volatile float pLevel;
}
s_dpin;

// analog input pin
typedef struct
{
  float level;
} 
s_apin;


typedef struct 
{
    s_dpin dPins[NUM_DPINS];
    s_apin aPins[NUM_APINS];
}
s_state;


unsigned long __millis_first = 0;
unsigned long millis(void) 
{	
  timeval time;
  gettimeofday(&time, NULL);
  long m = (time.tv_sec * 1000) + (time.tv_usec / 1000);
  if (!__millis_first) __millis_first=m;
  m-=__millis_first;
  //printf("millis(): returning %li\n", m);
  return m;
};


void delay(unsigned long ms)
{
  usleep(ms * 1000); 
};


s_state currState;

void pinMode(uint8_t pinNo, uint8_t mode)
{
  //if (13!=pinNo) 
  //printf("pinMode(%i, %i)\n", pinNo, mode);
  s_dpin &pin = currState.dPins[pinNo];
  pin.mode = mode;
}


void digitalWrite(uint8_t pinNo, uint8_t level)
{
  //if (13!=pinNo) printf("digitalWrite(%i, %i)\n", pinNo, level);
  currState.dPins[pinNo].oLevel = level;
  currState.dPins[pinNo].pwm    = false;  
}


boolean do_restart = true;


void handleclient(int sock)
{
  char buf[1024];

  s_state lastState;
  uint8_t lastData[E2END+1];
      
  for (;;)  
  {
    memcpy(&lastState, &currState, sizeof(lastState));
    memcpy(&lastData, eEEPROM.getData(), sizeof(lastData));
    
    
    // clear console:
    send(sock, "\x1b\x5b\x48\x1b\x5b\x32\x4a", 7, 0);
    
    for (int i=1; i<=13; i++)
    {
      s_dpin &pin = currState.dPins[i];

      if (pin.mode==OUTPUT)
      {
        if (pin.pwm)
        {
           snprintf(buf, 1024, "%02i:  ->  %2.1f\n", i, pin.pLevel);
        }
        else
        {
           snprintf(buf, 1024, "%02i:  ->  %s\n", i, pin.oLevel==HIGH ? "*" : ".");
        }
      }
      else
      {
        snprintf(buf, 1024, "%02i: %s\n", 
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
      
      int s=32;    
      for (int i=0; i<=256; i+=16)
      {
        for (int n=0; n<s; n++)
        {
          if (0==n%16) send(sock, " ", 1, 0);
          uint8_t b = lastData[i+n];
          snprintf(buf, 1024, "%02x ", b);
          send(sock, buf, strlen(buf), 0);          
        }
        send(sock, "  ", 2, 0);
        
        for (int n=0; n<s; n++)
        {
          if (0==n%16) send(sock, " ", 1, 0);
          uint8_t b = lastData[i+n];
          if (b<' ' || b>127)
            snprintf(buf, 1024, ".");
          else
            snprintf(buf, 1024, "%c", b);
 
          send(sock, buf, strlen(buf), 0);
        }
        send(sock, "\n", 1, 0);
      }

    while (
      0==memcmp(&lastState, &currState, sizeof(lastState))  &&
      0==memcpy(&lastData, eEEPROM.getData(), sizeof(lastData))
      )
    {
      usleep(20*1000);
    }
    usleep(20*1000);
  }
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
    perror("bind");

    printf("listening on port 7777...\n");
    k = listen(sock_desc,20); 
    perror("listen");
    len = sizeof(client);    
    
    for (;;)
    {
      printf("accepting clients...\n");
      temp_sock_desc = accept(sock_desc,(struct sockaddr*)&client,&len); 
      printf("new client connection\n");
    
      while(1)
      {     
        handleclient(temp_sock_desc);
      } 
      close(sock_desc); 
    }
  
    close(temp_sock_desc); 
}


int main(int argc, char * argv[])
{
    pthread_t server_thread;
  
    pthread_create (&server_thread, NULL, server, NULL);
            
    setup();
    
    printf("entering event loop\n");
    sleep(4);
    
    while (do_restart)
    {
                do_restart = false;

                setup();
                for (;!do_restart;)
                {
                        loop();
                        serialEvent();
                }
    }
}


