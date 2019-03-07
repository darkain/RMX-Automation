/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __COMM_H_
#define __COMM_H_


#include "../rmxThread.h"
#include "Timer.h"


#define COM1 "com1"
#define COM2 "com2"
#define COM3 "com3"
#define COM4 "com4"

#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

#define READ_TIMEOUT     500      // milliseconds


class vComm : protected rmxThread, protected vTimer {
  public:
    vComm();
    virtual ~vComm();

  public:
    BOOL openPort(const char *port=COM1);
    void closePort();

    BOOL readBytes(unsigned char *data, int len);
    BOOL writeBytes(const unsigned char *data, int len);

    void purge();

    inline BOOL isRinging() const { return ring; }

  protected:
    virtual int threadProc();
    virtual void onTimer(DWORD systemTime);

    virtual void onRing() {}

  private:
	DCB dcb;
    HANDLE hcomm;
    volatile BOOL ring;
};


#endif //__COMM_H_
