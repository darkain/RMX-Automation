/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/

#if 0

#ifndef __SECUREPARSER_H__
#define __SECUREPARSER_H__


#include <Core/RMX DLL LIB/DllEnums.h>
#include "RmxStack.h"
#include "RootListCallback.h"


class vPlugin;
class ipcConfigCallback;


class vSecureParser {
  friend class ipcConfigCallback;
  friend class ipcRootListCallback;

  public:
    vSecureParser(vPlugin *plug, UINT client);
    virtual ~vSecureParser();

    virtual UINT sendCommand(BYTE *data, int len) const { return 0; }
    static  UINT beginParser(BYTE *data, int len);
    virtual UINT parseCommand(rmxList<RMXSTACK*> *stack);

    virtual BOOL valid(HLIST list)   { return FALSE; }
    virtual BOOL valid(HPARAM param) { return FALSE; }

    virtual UINT sendString(const char *str, int len=-1)=0;

    inline void setClient(UINT client) { remoteclient = client; }
    inline UINT getClient() const { return remoteclient; }
//    vPlugin *getPlugin() const { return plugin; }

    
    //add item to local string table from remote string table
    //dont use this unless you know what you are doing
    static void addString(const char *string);


//    virtual void onInsertRootList(HLIST list, HLIST insert) { RmxMsg(5, ONNEWROOTLIST, (UINT)list, (UINT)insert); }


  protected:
    UINT RmxMsg(UINT type, UINT message) const;
    UINT RmxMsg(UINT type, UINT message, UINT param1) const;
    UINT RmxMsg(UINT type, UINT message, UINT param1, UINT param2) const;
    UINT RmxMsg(UINT type, UINT message, UINT param1, UINT param2, UINT param3) const;
    UINT RmxMsg(UINT type, UINT message, UINT param1, UINT param2, UINT param3, UINT param4) const;
    UINT RmxMsg(UINT type, UINT message, UINT param1, UINT param2, UINT param3, UINT param4, UINT param5) const;
    UINT RmxMsg(UINT type, UINT message, UINT param1, UINT param2, UINT param3, UINT param4, UINT param5, UINT param6) const;
  
    UINT RmxMsg(UINT type, UINT message, const char *param1) const;
    UINT RmxMsg(UINT type, UINT message, UINT param1, const char *param2) const;
    UINT RmxMsg(UINT type, UINT message, UINT param1, UINT param2, const char *param3) const;
    UINT RmxMsg(UINT type, UINT message, UINT param1, const char *param2, UINT param3) const;
    UINT RmxMsg(UINT type, UINT message, UINT param1, const char *param2, const char *param3) const;
    UINT RmxMsg(UINT type, UINT message, const char *param1, const char *param2, const char *param3) const;
    UINT RmxMsg(UINT type, UINT message, UINT param1, const char *param2, const char *param3, const char *param4) const;

  protected:
    UINT remoteclient;
    vPlugin *plugin;

  private:
    static rmxList<const char*> stringTable;
};


#endif //__SECUREPARSER_H__


#endif //0
