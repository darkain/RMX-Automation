/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __RMXSTACK_H__
#define __RMXSTACK_H__


#include <rmxList.h>


class vSecureParser;


typedef struct {
  UINT format;
  UINT len;
  union {
    UINT data;
    const char str[1];
    const char *pstr;
  };
} RMXSTACK;


class vRmxStack {
  public:
    vRmxStack();
    virtual ~vRmxStack();

    void appendItem(UINT intdata);
    void appendItem(const char *stringdata);

    UINT build(const vSecureParser *sender);

  private:
    rmxList<RMXSTACK*> data;
    int len;
};


#endif //__RMXSTACK_H__
