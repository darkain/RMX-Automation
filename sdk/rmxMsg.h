/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RMX_MSG_
#define _RMX_MSG_


#include "rmxTypes.h"
#include "rmxGuid.h"


struct rmxMsg {
  HPLUGIN plugin;
#ifndef _M_X64          //extra space, just in case we use 64bit OS
  UINT    reserved;  
#endif
  UINT    type;
  UINT    msg;

  union {
    struct {
      union {
        const char *p0s;
        int         p0i;
        double      p0d;
        void       *p0p;
        RMXLONG     p0l;
        FOURCC      p0f;
      };

      union {
        const char *p1s;
        int         p1i;
        double      p1d;
        void       *p1p;
        RMXLONG     p1l;
        FOURCC      p1f;
      };
    };

    GUID p01g;
  };


  union {
    struct {
      union {
        const char *p2s;
        int         p2i;
        double      p2d;
        void       *p2p;
        RMXLONG     p2l;
        FOURCC      p2f;
      };

      union {
        const char *p3s;
        int         p3i;
        double      p3d;
        void       *p3p;
        RMXLONG     p3l;
        FOURCC      p3f;
      };
    };

    GUID p23g;
  };


  union {
    struct {
      union {
        const char *p4s;
        int         p4i;
        double      p4d;
        void       *p4p;
        RMXLONG     p4l;
        FOURCC      p4f;
      };

      union {
        const char *p5s;
        int         p5i;
        double      p5d;
        void       *p5p;
        RMXLONG     p5l;
        FOURCC      p5f;
      };
    };

    GUID p45g;
  };


  union {
    struct {
      union {
        const char *p6s;
        int         p6i;
        double      p6d;
        void       *p6p;
        RMXLONG     p6l;
        FOURCC      p6f;
      };

      union {
        const char *p7s;
        int         p7i;
        double      p7d;
        void       *p7p;
        RMXLONG     p7l;
        FOURCC      p7f;
      };
    };

    GUID p67g;
  };


  union {
    struct {
      union {
        const char *p8s;
        int         p8i;
        double      p8d;
        void       *p8p;
        RMXLONG     p8l;
        FOURCC      p8f;
      };

      union {
        const char *p9s;
        int         p9i;
        double      p9d;
        void       *p9p;
        RMXLONG     p9l;
        FOURCC      p9f;
      };
    };

    GUID p89g;
  };


  union {
    const char *rs;
    int         ri;
    double      rd;
    void       *rp;
    RMXLONG     rl;
    GUID        rg;
    FOURCC      rf;
  };


  // only used #if defined _DEBUG
  int _msg_type;
};


#define vMSG rmxMsg


#define RETPTR(x) {    msg->rp = (x); return; }
#define RETSTR(x) {    msg->rs = (x); return; }
#define RETINT(x) {    msg->ri = (x); return; }
#define RETLNG(x) {    msg->rl = (x); return; }
#define RETNUM(x) {    msg->rd = (x); return; }
#define RETUID(x) {    msg->rg = (x); return; }
#define RETFCC(x) {    msg->rf = (x); return; }
#define RETNUL(x) { x; msg->rl = (0); return; }
#define RETURN    {    msg->rl = (0); return; }


#endif //_RMX_MSG_
