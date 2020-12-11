/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CORE_PLUGIN_CALLBACK_H_
#define _CORE_PLUGIN_CALLBACK_H_


class vPlugin;


class plugCallback {
  friend class vPlugin;

  public:
    plugCallback(vPlugin *dsthplug, HLIST dsthlist, vPlugin *srchplug, void *callback) {
      dstplug = dsthplug;
      dstlist = dsthlist;
      srcplug = srchplug;
      cb      = callback;

      dstplug->RmxMsg(VMSG::CALLBACK_REMOTE, CBCFGINSERT, dstlist, this);
      remote = dstplug->extrmxmsg.rp;

#ifdef _RMX_DEBUG_
      char str[256]="";
      VPRINTF(str, sizeof(str), "NEW: %p %p %p %p %p %p", dstplug, dstlist, srcplug, cb, remote, this);
      CONSOLE_MAIN->print("CB", str);
#endif //_RMX_DEBUG_
    }

    ~plugCallback() {
#ifdef _RMX_DEBUG_
      char str[256]="";
      VPRINTF(str, sizeof(str), "DELETE: %p %p %p %p %p %p", dstplug, dstlist, srcplug, cb, remote, this);
      CONSOLE_MAIN->print("CB", str);
#endif //_RMX_DEBUG_

      if (dstplug) {
        dstplug->RmxMsg(VMSG::CALLBACK_REMOTE, CBCFGREMOVE, remote);
      }
    }

    inline void *getCallback() const { return cb; }
    
    inline void message(rmxMsg *msg) {
      msg->p0p  = cb;
      srcplug->RmxMsg(msg);
    }

  private:
    vPlugin *dstplug;
    HLIST    dstlist;
    vPlugin *srcplug;
    void    *cb;
    void    *remote;
};


#endif //_CORE_PLUGIN_CALLBACK_H_
