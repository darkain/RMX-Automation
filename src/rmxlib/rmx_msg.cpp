/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#define RMX_LIB


#include "../../sdk/rmxBase.h"
#include "rmx_msg.h"


RMX_ENTRY rmx = NULL;
THREAD_LOCAL rmxMsg extrmxmsg;


void RmxMsg(UINT type, UINT msg) {
  SetExtenalType(RMX_MESSAGE_NONE);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  (rmx)(&extrmxmsg);
} 


void RmxMsg(UINT type, UINT msg, void *p0) {
  SetExtenalType(RMX_MESSAGE_P);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1) {
  SetExtenalType(RMX_MESSAGE_PP);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, void *p2) {
  SetExtenalType(RMX_MESSAGE_PPP);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2p  = p2;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, void *p2, void *p3) {
  SetExtenalType(RMX_MESSAGE_PPPP);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2p  = p2;
  extrmxmsg.p3p  = p3;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, int p1) {
  SetExtenalType(RMX_MESSAGE_PI);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1i  = p1;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, int p1, void *p2, const char *p3) {
  SetExtenalType(RMX_MESSAGE_PIPC);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1i  = p1;
  extrmxmsg.p2p  = p2;
  extrmxmsg.p3s  = p3;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, int p1, void *p2, RMXLONG p3) {
  SetExtenalType(RMX_MESSAGE_PIPL);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1i  = p1;
  extrmxmsg.p2p  = p2;
  extrmxmsg.p3l  = p3;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, int p1, void *p2, void *p3, void *p4) {
  SetExtenalType(RMX_MESSAGE_PIPPP);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1i  = p1;
  extrmxmsg.p2p  = p2;
  extrmxmsg.p3p  = p3;
  extrmxmsg.p4p  = p4;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, RMXLONG p2) {
  SetExtenalType(RMX_MESSAGE_PPL);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2l  = p2;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, RMXLONG p2, int p3) {
  SetExtenalType(RMX_MESSAGE_PPLI);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2l  = p2;
  extrmxmsg.p3i  = p3;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, RMXLONG p2, RMXLONG p3, RMXLONG p4) {
  SetExtenalType(RMX_MESSAGE_PPLLL);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2l  = p2;
  extrmxmsg.p3l  = p3;
  extrmxmsg.p4l  = p4;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, int p2) {
  SetExtenalType(RMX_MESSAGE_PPI);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2i  = p2;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, double p2) {
  SetExtenalType(RMX_MESSAGE_PPD);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2d  = p2;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, void *p2, int p3) {
  SetExtenalType(RMX_MESSAGE_PPPI);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2p  = p2;
  extrmxmsg.p3i  = p3;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, GUID p23) {
  SetExtenalType(RMX_MESSAGE_PPG);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p23g = p23;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, FOURCC p2) {
  SetExtenalType(RMX_MESSAGE_PPF);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2f  = p2;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, const char *p0) {
  SetExtenalType(RMX_MESSAGE_C);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0s  = p0;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, const char *p0, void *p1) {
  SetExtenalType(RMX_MESSAGE_CP);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0s  = p0;
  extrmxmsg.p1p  = p1;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, const char *p0, int p1) {
  SetExtenalType(RMX_MESSAGE_CI);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0s  = p0;
  extrmxmsg.p1i  = p1;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, const char *p0, double p1) {
  SetExtenalType(RMX_MESSAGE_CD);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0s  = p0;
  extrmxmsg.p1d  = p1;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, const char *p0, const char *p1) {
  SetExtenalType(RMX_MESSAGE_CC);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0s  = p0;
  extrmxmsg.p1s  = p1;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, const char *p1) {
  SetExtenalType(RMX_MESSAGE_PC);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1s  = p1;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, const char *p1, int p2) {
  SetExtenalType(RMX_MESSAGE_PCI);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1s  = p1;
  extrmxmsg.p2i  = p2;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, const char *p1, double p2) {
  SetExtenalType(RMX_MESSAGE_PCD);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1s  = p1;
  extrmxmsg.p2d  = p2;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, const char *p1, const char *p2) {
  SetExtenalType(RMX_MESSAGE_PCC);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1s  = p1;
  extrmxmsg.p2s  = p2;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2) {
  SetExtenalType(RMX_MESSAGE_PPC);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2s  = p2;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2, int p3) {
  SetExtenalType(RMX_MESSAGE_PPCI);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2s  = p2;
  extrmxmsg.p3i  = p3;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2, double p3) {
  SetExtenalType(RMX_MESSAGE_PPCD);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2s  = p2;
  extrmxmsg.p2d  = p3;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2, const char *p3) {
  SetExtenalType(RMX_MESSAGE_PPCC);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2s  = p2;
  extrmxmsg.p3s  = p3;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2, RMXLONG p3) {
  SetExtenalType(RMX_MESSAGE_PPCL);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2s  = p2;
  extrmxmsg.p3l  = p3;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2, FOURCC p3) {
  SetExtenalType(RMX_MESSAGE_PPCF);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2s  = p2;
  extrmxmsg.p2f  = p3;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2, GUID p45) {
  SetExtenalType(RMX_MESSAGE_PPCG);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2s  = p2;
  extrmxmsg.p45g = p45;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2, void* p3) {
  SetExtenalType(RMX_MESSAGE_PPCP);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2s  = p2;
  extrmxmsg.p3p  = p3;
  (rmx)(&extrmxmsg);
}


void RmxMsg(UINT type, UINT msg, void *p0, void *p1, const char *p2, const char *p3, void *p4) {
  SetExtenalType(RMX_MESSAGE_PPCCP);
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = p0;
  extrmxmsg.p1p  = p1;
  extrmxmsg.p2s  = p2;
  extrmxmsg.p3s  = p3;
  extrmxmsg.p4p  = p4;
  (rmx)(&extrmxmsg);
}
