/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


//TODO: i gotta do #ifdef recursion prevention shit in all my plugins


#include "../../sdk/plugBase.h"
#include "../../sdk/func/funcSingle.h"
#include "../../sdk/func/funcGroup.h"
#include "../../sdk/Win32/Thread.h"
#include "../../sdk/Win32/Timer.h"
#include <winsock2.h>  //must come before jnetlib
#include "../../libraries/jnetlib/jnetlib.h"
#include <stdlib.h>
#include <iphlpapi.h>
#include <icmpapi.h>


class Network : public plugBase {
  public:
    Network();

  protected:
    virtual void onInit();
    virtual void onQuit();

  private:
    funcGroup *table;
};


class DnsScript : public funcSingle {
  public:
    DnsScript(funcGroup *parent);
    virtual ~DnsScript() {}

  public:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};


class ReverseDnsScript : public funcSingle {
  public:
    ReverseDnsScript(funcGroup *parent);
    virtual ~ReverseDnsScript() {}

  public:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};


class SharesScript : public funcSingle {
  public:
    SharesScript(funcGroup *parent);
    virtual ~SharesScript() {}

  public:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};


class TraceRouteScript : public funcSingle {
  public:
    TraceRouteScript(funcGroup *parent);
    virtual ~TraceRouteScript() {}

  public:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};


class PingScript : public funcSingle {
  public:
    PingScript(funcGroup *parent);
    virtual ~PingScript() {}

  public:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};


class MacAddressScript : public funcSingle {
  public:
    MacAddressScript(funcGroup *parent);
    virtual ~MacAddressScript() {}

  public:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};


class vWol : public funcSingle {
  public:
    vWol(funcGroup *parent);
    virtual ~vWol();
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};


class vWolThread : public rmxThread {
  public:
    vWolThread(const char *macaddress);
    virtual ~vWolThread() {}
    virtual int threadProc();

  private:
    BYTE mac[6];
};


// Structures required to use functions in ICMP.DLL
/*
typedef struct {
    unsigned char Ttl;                         // Time To Live
    unsigned char Tos;                         // Type Of Service
    unsigned char Flags;                       // IP header flags
    unsigned char OptionsSize;                 // Size in bytes of options data
    unsigned char *OptionsData;                // Pointer to options data
} IP_OPTION_INFORMATION, * PIP_OPTION_INFORMATION;
*/
typedef struct {
    DWORD Address;                             // Replying address
    unsigned long  Status;                     // Reply status
    unsigned long  RoundTripTime;              // RTT in milliseconds
    unsigned short DataSize;                   // Echo data size
    unsigned short Reserved;                   // Reserved for system use
    void *Data;                                // Pointer to the echo data
    IP_OPTION_INFORMATION Options;             // Reply options
} IP_ECHO_REPLY, * PIP_ECHO_REPLY;




/////////////////////////////////////////////////////////////////////////////////////////



class netServer : public JNL_Connection {
};