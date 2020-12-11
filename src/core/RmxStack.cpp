/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/

#if 0 

#include <rmxBase.h>
#include <Win32.h>
#include <stdio.h>
#include <winsock.h>
#include "RmxStack.h"
#include "SecureLoader.h"


vRmxStack::vRmxStack() {
  len = 0;
}


vRmxStack::~vRmxStack() {
  RMXSTACK *d = data.getFirstItem();
  while (d) {
    free(d);
    data.removeItem(d);
    d = data.getFirstItem();;
  }
}


void vRmxStack::appendItem(UINT intdata) {
  RMXSTACK *d = (RMXSTACK*)malloc(sizeof(RMXSTACK));
  d->format = 1;
  d->len = 4;
  d->data = intdata;
  data.appendItem(d);
  len += d->len + 8;
}

void vRmxStack::appendItem(const char *stringdata) {
  RMXSTACK *d = (RMXSTACK*)malloc(sizeof(RMXSTACK));
  if (stringdata) {
    d->format = 2;
    d->len = VSTRLEN(stringdata)+1;
    d->pstr = stringdata;
  } else {
    d->format = 3;
    d->len = 4;
    d->pstr = NULL;
  }
  data.appendItem(d);
  len += d->len + 8;
}


UINT vRmxStack::build(const vSecureParser *sender) {
  if (!sender) return 0;
  if (len == 0) return 0;
  BYTE *byte = (BYTE*)malloc(len+8);
  if (!byte) return 0;
  
  BYTE *b = byte;
  *((int*)b) = htonl(len);
  b += 4;
  *((int*)b) = htonl(sender->getClient());
  b += 4;

  RMXSTACK *d = data.getFirstItem();
  while (d) {
    *((int*)b) = htonl(d->format);
    b += 4;
    *((int*)b) = htonl(d->len);
    b += 4;

    if (d->format == 1) {
      *((int*)b) = htonl(d->data);
      b += d->len;

    } else if (d->format == 2) {
      memcpy(b, d->pstr, d->len);
      b += d->len;

    } else if (d->format == 3) {
      *((int*)b) = htonl(0);
      b += d->len;
    }
    d = data.getNextItem(d);
  }

  UINT ret = sender->sendCommand(byte, len+8);
  free(byte);
  return ret;
}


#endif //0
