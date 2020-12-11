/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/

#if 0

#ifndef __IPCPARSER_H__
#define __IPCPARSER_H__


#include "../SecureParser.h"
#include "../Plugin.h"


class ipcParser : public vSecureParser {
  public:
    ipcParser(HWND master, vPlugin *plug);
    virtual ~ipcParser();

  public:
    virtual BOOL valid(HLIST list);
    virtual BOOL valid(HPARAM param);

    virtual UINT sendCommand(BYTE *data, int len) const;
    virtual UINT sendString(const char *str, int len=-1);

    HWND getWnd() const { return hwnd; }

  private:
    HWND hwnd;
};


#endif //__IPCPARSER_H__

#endif // 0
