/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _HID_BASE_H_
#define _HID_BASE_H_


#include "../../sdk/Win32/Win32.h"
#include "../../sdk/Win32/Thread.h"
#include "../../sdk/Win32/MessageWnd.h"


extern "C"  {
#  include <hidsdi.h>
}


class hidBase : public vThread, public vMessageWnd {
  public:
    hidBase(HANDLE devhandle, HIDD_ATTRIBUTES *devattrib, const char *devpath);
    virtual ~hidBase();

  protected:
    virtual DWORD ThreadProc();
    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);

    virtual void onReadReport(unsigned char *data, int len)=0;

  public:
//    BOOL connect(unsigned short vendor, unsigned short product);
//    void disconnect();

    inline unsigned short getVendor()  const { return attrib.VendorID;      }
    inline unsigned short getProduct() const { return attrib.ProductID;     }
    inline unsigned short getVersion() const { return attrib.VersionNumber; }
    inline const char    *getPath()    const { return path.getName();       }

    inline void setForceWrite(BOOL force) { forcewrite = !!force; }
    inline BOOL isForceWrite() const { return forcewrite; }

    inline BOOL canRead()  const { return hread != INVALID_HANDLE_VALUE; }
    inline BOOL canWrite() const { return hread != INVALID_HANDLE_VALUE; }

    BOOL readReport(       unsigned char *data, int len);
    BOOL writeReport(const unsigned char *data, int len);

    //some helper functions
    BOOL reportByte(unsigned char report, unsigned char byte);  //writes a single byte to a report

  private:
    HIDD_ATTRIBUTES attrib;
	HIDP_CAPS  caps;
    OVERLAPPED overlap;
	HANDLE     hevent;
	HANDLE     hdevice;
	HANDLE     hread;
	HANDLE     hwrite;
    BOOL       forcewrite;
//    GUID       guid;

    rmxName path;
    rmxFreeList<unsigned char*> buffers;
};


#endif //_HID_BASE_H_
