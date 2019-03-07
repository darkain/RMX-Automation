/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __RMX_MSG__
#define __RMX_MSG__


#include "../../sdk/rmxTypes.h"
#include "../../sdk/rmxMsg.h"
#include "rmx_entry.h"
#include "rmx_lib_msg.h"


extern THREAD_LOCAL vMSG extrmxmsg;


void RmxMsg(UINT type, UINT msg);

void RmxMsg(UINT type, UINT msg, void *p0);
void RmxMsg(UINT type, UINT msg, void *p0, void *p1);
void RmxMsg(UINT type, UINT msg, void *p0, void *p1, void *p2);
void RmxMsg(UINT type, UINT msg, void *p0, void *p1, void *p2, void *p3);

void RmxMsg(UINT type, UINT msg, void *p0, int p1);
void RmxMsg(UINT type, UINT msg, void *p0, int p1, void *p2, const char *p3);
void RmxMsg(UINT type, UINT msg, void *p0, int p1, void *p2, RMXLONG     p3);
void RmxMsg(UINT type, UINT msg, void *p0, int p1, void *p2, void       *p3, void *p4);

void RmxMsg(UINT type, UINT msg, void *p0, void *p1, RMXLONG p2);
void RmxMsg(UINT type, UINT msg, void *p0, void *p1, RMXLONG p2, int p3);
void RmxMsg(UINT type, UINT msg, void *p0, void *p1, RMXLONG p2, RMXLONG p3, RMXLONG p4);

void RmxMsg(UINT type, UINT msg, void *p0, void *p1, int p2);
void RmxMsg(UINT type, UINT msg, void *p0, void *p1, double p2);
void RmxMsg(UINT type, UINT msg, void *p0, void *p1, void *p2, int p3);

void RmxMsg(UINT type, UINT msg, void *p0, void *p1, GUID p23);
void RmxMsg(UINT type, UINT msg, void *p0, void *p1, FOURCC p2);



//strings
void RmxMsg(UINT type, UINT msg, const char *p0);
void RmxMsg(UINT type, UINT msg, const char *p0, void *p1);
void RmxMsg(UINT type, UINT msg, const char *p0, int p1);
void RmxMsg(UINT type, UINT msg, const char *p0, double p1);
void RmxMsg(UINT type, UINT msg, const char *p0, const char *p1);

void RmxMsg(UINT type, UINT msg, void *p0, const char *p1);
void RmxMsg(UINT type, UINT msg, void *p0, const char *p1, int p2);
void RmxMsg(UINT type, UINT msg, void *p0, const char *p1, double p2);
void RmxMsg(UINT type, UINT msg, void *p0, const char *p1, const char *p2);

void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2);
void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2, int p3);
void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2, double p3);
void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2, const char *p3);
void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2, RMXLONG p3);
void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2, FOURCC p3);
void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2, GUID p45);
void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2, void* p3);

void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2, const char *p3, void *p4);


#ifdef _DEBUG
#  define RmxDbg(type) {}
#else
#  define RmxDbg()
#endif

#define SetExtenalType(type) extrmxmsg._msg_type = type


#define RmxNul(x)    {                             RmxMsg x;                          }
#define RmxPtr(y, x) { extrmxmsg.rp=NULL;          RmxMsg x; return (y) extrmxmsg.rp; }
#define RmxStr(x)    { extrmxmsg.rs=NULL;          RmxMsg x; return     extrmxmsg.rs; }
#define RmxSaf(x)    { extrmxmsg.rs=safe;          RmxMsg x; return extrmxmsg.rs?extrmxmsg.rs:safe; }
#define RmxInt(x)    { extrmxmsg.ri=0;             RmxMsg x; return     extrmxmsg.ri; }
#define RmxLng(x)    { extrmxmsg.rl=0;             RmxMsg x; return     extrmxmsg.rl; }
#define RmxNum(x)    { extrmxmsg.rd=0;             RmxMsg x; return     extrmxmsg.rd; }
#define RmxUid(x)    { extrmxmsg.rg=_INVALID_GUID; RmxMsg x; return     extrmxmsg.rg; }
#define RmxFcc(x)    { extrmxmsg.ri=0;             RmxMsg x; return     extrmxmsg.ri; }


enum RMX_MESSAGE_TYPES {
  RMX_MESSAGE_NONE,
  RMX_MESSAGE_P,
  RMX_MESSAGE_PP,
  RMX_MESSAGE_PPP,
  RMX_MESSAGE_PPPP,
  RMX_MESSAGE_PI,
  RMX_MESSAGE_PIPC,
  RMX_MESSAGE_PIPL,
  RMX_MESSAGE_PIPPP,
  RMX_MESSAGE_PPL,
  RMX_MESSAGE_PPLI,
  RMX_MESSAGE_PPLLL,
  RMX_MESSAGE_PPI,
  RMX_MESSAGE_PPD,
  RMX_MESSAGE_PPPI,
  RMX_MESSAGE_PPG,
  RMX_MESSAGE_PPF,
  RMX_MESSAGE_C,
  RMX_MESSAGE_CP,
  RMX_MESSAGE_CI,
  RMX_MESSAGE_CD,
  RMX_MESSAGE_CC,
  RMX_MESSAGE_PC,
  RMX_MESSAGE_PCI,
  RMX_MESSAGE_PCD,
  RMX_MESSAGE_PCC,
  RMX_MESSAGE_PPC,
  RMX_MESSAGE_PPCI,
  RMX_MESSAGE_PPCD,
  RMX_MESSAGE_PPCC,
  RMX_MESSAGE_PPCL,
  RMX_MESSAGE_PPCF,
  RMX_MESSAGE_PPCG,
  RMX_MESSAGE_PPCP,
  RMX_MESSAGE_PPCCP,
};


#endif // __RMX_MSG__
