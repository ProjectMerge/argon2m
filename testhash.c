#include "argon2d/argon2.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

static const size_t INPUT_BYTES = 80;
static const size_t OUTPUT_BYTES = 32;
static const unsigned int DEFAULT_ARGON2_FLAG = 2;

void argon2m_hash(void* input, void* output)
{
  argon2_context context;
  context.out = (uint8_t *)output;
  context.outlen = (uint32_t)OUTPUT_BYTES;
  context.pwd = (unsigned char*)input;
  context.pwdlen = (uint32_t)INPUT_BYTES;
  context.salt = context.pwd;
  context.saltlen = context.pwdlen;
  context.secret = NULL;
  context.secretlen = 0;
  context.ad = NULL;
  context.adlen = 0;
  context.allocate_cbk = NULL;
  context.free_cbk = NULL;
  context.flags = DEFAULT_ARGON2_FLAG;
  context.m_cost = 2;
  context.lanes = 1;
  context.threads = 1;
  context.t_cost = 1;
  context.version = ARGON2_VERSION_13;
  argon2_ctx( &context, Argon2_id );
}

long grabtime()
{
  struct timeval time;
  gettimeofday(&time, NULL);
  return time.tv_sec * 1000 + time.tv_usec / 1000;
}

int main()
{
  long s1;
  int w, x, y, z, hsec = 0;
  unsigned char header[80];
  unsigned char hash[32];

  srand(time(NULL));
  printf("");

  for (int i=0; i<80; i++)
    header[i] = rand() % 256;

  uint32_t htarget, hetarget;
  uint32_t best = 0xffffffff;

  s1 = grabtime();
  for (header[76]=0; header[76]<256; header[76]++)
  {
    for (header[77]=0; header[77]<256; header[77]++)
    {
      for (header[78]=0; header[78]<256; header[78]++)
      {
        for (header[79]=0; header[79]<256; header[79]++)
        {
          hsec++;
          argon2m_hash(header,hash);
          htarget = *(uint32_t*)&hash;
          hetarget = ((htarget>>24)&0xff) | ((htarget<<8)&0xff0000) |
                     ((htarget>>8)&0xff00) | ((htarget<<24)&0xff000000);
	  
	  if (hetarget < best)
	  {
	     best = hetarget;
             for (int i=0;i<32;i++)
	       printf("%02hhx",hash[i]);
	     printf("\n");
	  }

          if ((grabtime()-s1) > 1000) {
             printf("%d h/s\n", hsec);
             s1 = grabtime();
             hsec = 0;
          }
        }
      }
    }
  }

  return 0;
}
