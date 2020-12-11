/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "rmxBase.h"
#include "rmxUnicode.h"
#include "Win32/Win32.h"


rmxUtf8ToWide::rmxUtf8ToWide(const char *string, int length) {
  if (length < 0) length = 0x7FFFFFFF;
  len    = 0;
  buffer = NULL;
  if (!string) return;


  len           = 0;
  int remain    = length;
  const char *p = string;


  while (*p  &&  remain-- > 0) {

    //4-bytes (21 bits)
    if ((*p & 0xF8)  ==  0xF0) {

      //Correct UTF8 block
      if ( (remain > 3)  &&  ((p[1] & 0xC0)  ==  0x80)  &&  ((p[2] & 0xC0)  ==  0x80)  &&  ((p[3] & 0xC0)  ==  0x80) ) {
        len    += 1;
        remain -= 3;
        p      += 3;

      //Second byte is NULL
      } else if ( (remain > 0)  &&  (p[1] == NULL) ) {
        len    += 1;

      //Third byte is NULL
      } else if ( (remain > 1)  &&  (p[2] == NULL) ) {
        len    += 2;
        remain -= 1;
        p      += 1;

      //Fourth byte is NULL
      } else if ( (remain > 2)  &&  (p[3] == NULL) ) {
        len    += 3;
        remain -= 2;
        p      += 2;

      //Error decoding UTF8 block
      } else if (remain > 0) {
        int rest = min(remain, 3);
        len    += rest + 1;
        remain -= rest;
        p      += rest;
      }


    //3-bytes (16 bits)
    } else if ((*p & 0xF0)  ==  0xE0) {  

      //Correct UTF8 block
      if ( (remain > 2)  &&  ((p[1] & 0xC0)  ==  0x80)  &&  ((p[2] & 0xC0)  ==  0x80) ) {
        len    += 1;
        remain -= 2;
        p      += 2;

      //Second byte is NULL
      } else if ( (remain > 0)  &&  (p[1] == NULL) ) {
        len    += 1;

      //Third byte is NULL
      } else if ( (remain > 1)  &&  (p[2] == NULL) ) {
        len    += 2;
        remain -= 1;
        p      += 1;

      //Error decoding UTF8 block
      } else if (remain > 0) {
        int rest = min(remain, 2);
        len    += rest + 1;
        remain -= rest;
        p      += rest;
      }


    //2-bytes (11 bits)
    } else if ((*p & 0xE0)  ==  0xC0) {  

      //Correct UTF8 block
      if ( (remain > 1)  &&  ((p[1] & 0xC0)  ==  0x80) ) {
        len    += 1;
        remain -= 1;
        p      += 1;

      //Second byte is NULL
      } else if ( (remain > 0)  &&  (p[1] == NULL) ) {
        len    += 1;

      //Error decoding UTF8 block
      } else if (remain > 0) {
        int rest = min(remain, 1);
        len    += rest + 1;
        remain -= rest;
        p      += rest;
      }


    //1-byte  ( 7 bits)
    } else if ((*p & 0x80)  ==  0x00) {  
      len++;


    //UTF8 Error
    } else {
      len++;
    }


    p++;
  }


  buffer = (wchar_t*) malloc((len+1) * 2);

  remain = length;
  wchar_t *b = buffer;
  p = string;


  while (*p  &&  remain-- > 0) {
    //4-bytes (21 bits)
    if ((*p & 0xF8)  ==  0xF0) {

      //Correct UTF8 block
      if ( (remain > 3)  &&  ((p[1] & 0xC0)  ==  0x80)  &&  ((p[2] & 0xC0)  ==  0x80)  &&  ((p[3] & 0xC0)  ==  0x80) ) {
        *b++    = '?';
        remain -= 3;
        p      += 3;

      //Second byte is NULL
      } else if ( (remain > 0)  &&  (p[1] == NULL) ) {
        *b++ = *p;

      //Third byte is NULL
      } else if ( (remain > 1)  &&  (p[2] == NULL) ) {
        remain -= 1;
        *b++    = *p++;
        *b++    = *p;

      //Fourth byte is NULL
      } else if ( (remain > 2)  &&  (p[3] == NULL) ) {
        remain -= 2;
        *b++    = *p++;
        *b++    = *p++;
        *b++    = *p;

      //Error decoding UTF8 block
      } else if (remain > 0) {
        remain -= min(remain, 3);
        if (remain > 0) *b++ = *p++;
        if (remain > 1) *b++ = *p++;
        if (remain > 2) *b++ = *p++;
        if (remain > 3) *b++ = *p++;
        if (remain > 0) p--;
      }


    //3-bytes (16 bits)
    } else if ((*p & 0xF0)  ==  0xE0) {  

      //Correct UTF8 block
      if ( (remain > 2)  &&  ((p[1] & 0xC0)  ==  0x80)  &&  ((p[2] & 0xC0)  ==  0x80) ) {
        *b++ = ( ((p[0] & 0x0F) << 0x0C) | ((p[1] & 0x3F) << 0x06) | (p[2] & 0x3F) );
        remain -= 2;
        p += 2;

      //Second byte is NULL
      } else if ( (remain > 0)  &&  (p[1] == NULL) ) {
        *b++ = *p;

      //Third byte is NULL
      } else if ( (remain > 1)  &&  (p[2] == NULL) ) {
        remain -= 1;
        *b++ = *p++;
        *b++ = *p;

      //Error decoding UTF8 block
      } else if (remain > 0) {
        remain -= min(remain, 2);
        if (remain > 0) *b++ = *p++;
        if (remain > 1) *b++ = *p++;
        if (remain > 2) *b++ = *p++;
        if (remain > 0) p--;
      }


    //2-bytes (11 bits)
    } else if ((*p & 0xE0)  ==  0xC0) {  

      //Correct UTF8 block
      if ( (remain > 1)  &&  ((p[1] & 0xC0)  ==  0x80) ) {
        *b++ = ( ((p[0] & 0x1F) << 0x06) | (p[1] & 0x3F) );
        p += 1;

      //Second byte is NULL
      } else if ( (remain > 0)  &&  (p[1] == NULL) ) {
        *b++ = *p;

      //Error decoding UTF8 block
      } else if (remain > 0) {
        remain -= 1;
        if (remain > 0) *b++ = *p++;
        if (remain > 1) *b++ = *p++;
        if (remain > 0) p--;
      }


    //1-byte  ( 7 bits)
    } else if ((*p & 0x80)  ==  0x00) {  
      *b++ = *p;


    //UTF8 Error
    } else {
      *b++ = *p;
    }


    p++;
  }


  *b = NULL;
}


rmxUtf8ToWide::~rmxUtf8ToWide() {
  if (buffer) free(buffer);
}



//-----------------------------------------------------------------------------------------------------------



rmxToUtf8::rmxToUtf8(const char *string, int length) {
  if (length < 0) length = 0x7FFFFFFF;
  len    = 0;
  buffer = NULL;
  if (!string) return;


  int remain       = length;
  unsigned char *p = (unsigned char*) string;


  while (*p  &&  remain-- > 0) {
    if (*p >= 0x80) len += 2;
    else            len += 1;
    p++;
  }


  buffer = (char*) malloc(len + 1);

  remain  = length;
  char *b = buffer;
  p       = (unsigned char*) string;


  while (*p  &&  remain-- > 0) {
    if (*p >= 0x80) {
      *b++ = ((*p >> 0x06) & 0x1F) | 0xC0;
      *b++ = ((*p >> 0x00) & 0x3F) | 0x80;
    } else {
      *b++ = *p & 0x7F;
    }
    p++;
  }

  *b = NULL;
}



rmxToUtf8::rmxToUtf8(const wchar_t *string, int length) {
  if (length < 0) length = 0x7FFFFFFF;
  len    = 0;
  buffer = NULL;
  if (!string) return;


  int remain        = length;
  unsigned short *p = (unsigned short*) string;


  while (*p  &&  remain-- > 0) {
    if      (*p >= 0x08FF) len += 3;
    else if (*p >= 0x0080) len += 2;
    else                   len += 1;
    p++;
  }


  buffer = (char*) malloc(len + 1);

  remain  = length;
  char *b = buffer;
  p       = (unsigned short*) string;


  while (*p  &&  remain-- > 0) {
    if (*p >= 0x08FF) {
      *b++ = ((*p >> 0x0C) & 0x0F) | 0xE0;
      *b++ = ((*p >> 0x06) & 0x3F) | 0x80;
      *b++ = ((*p >> 0x00) & 0x3F) | 0x80;
    } else if (*p >= 0x0080) {
      *b++ = ((*p >> 0x06) & 0x1F) | 0xC0;
      *b++ = ((*p >> 0x00) & 0x3F) | 0x80;
    } else {
      *b++ = *p & 0x7F;
    }
    p++;
  }

  *b = NULL;
}



rmxToUtf8::rmxToUtf8(const unsigned int *string, int length) {
  if (length < 0) length = 0x7FFFFFFF;
  len    = 0;
  buffer = NULL;
  if (!string) return;


  int remain            = length;
  const unsigned int *p = string;


  while (*p  &&  remain-- > 0) {
    if      (*p >= 0xFFFF) len += 4;
    else if (*p >= 0x08FF) len += 3;
    else if (*p >= 0x0080) len += 2;
    else                   len += 1;
    p++;
  }


  buffer = (char*) malloc(len + 1);

  remain  = length;
  char *b = buffer;
  p       = string;


  while (*p  &&  remain-- > 0) {
    if (*p >= 0xFFFF) {
      *b++ = ((*p >> 0x12) & 0x0F) | 0xE0;
      *b++ = ((*p >> 0x0C) & 0x3F) | 0x80;
      *b++ = ((*p >> 0x06) & 0x3F) | 0x80;
      *b++ = ((*p >> 0x00) & 0x3F) | 0x80;
    } else if (*p >= 0x08FF) {
      *b++ = ((*p >> 0x0C) & 0x0F) | 0xE0;
      *b++ = ((*p >> 0x06) & 0x3F) | 0x80;
      *b++ = ((*p >> 0x00) & 0x3F) | 0x80;
    } else if (*p >= 0x0080) {
      *b++ = ((*p >> 0x06) & 0x1F) | 0xC0;
      *b++ = ((*p >> 0x00) & 0x3F) | 0x80;
    } else {
      *b++ = *p & 0x7F;
    }
    p++;
  }

  *b = NULL;
}



rmxToUtf8::~rmxToUtf8() {
  if (buffer) free(buffer);
}

