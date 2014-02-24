

#include "Oxduino.h"
#include <stdio.h>
#include <string.h>

static char buf[1024];

char *itoa(int __val, char *__s, int __radix)
{
  sprintf(__s, "%i", __val);
  return buf;
}

char *ltoa(long int __val, char *__s, int __radix)
{
  sprintf(__s, "%li", __val);
  return buf;   
}

char *utoa(unsigned int __val, char *__s, int __radix)
{

  sprintf(__s, "%i", (unsigned int)__val);
  return buf;
}

char *ultoa(unsigned long int __val, char *__s, int __radix)
{
  sprintf(__s, "%li", (unsigned long)__val);
  return buf;
}

long atol(const char *__s)
{
  long l;
  sscanf(__s, "%li", &l);
  return l;
}

int atoi(const char *__s)
{
  int i;
  sscanf(__s, "%i", &i);
  return i;
}

double atof(const char *__nptr)
{
  double d;
  sscanf(__nptr, "%lf", &d);
  return d;
}

