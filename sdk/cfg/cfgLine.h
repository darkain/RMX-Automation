/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CFG_LINE_H_
#define _CFG_LINE_H_


#include "cfgBase.h"


/*
this class does not directly use UserData
*/


class cfgLine : public cfgBase {
  public:
    cfgLine(cfgBase *par, cfgBase *insert=NULL) : cfgBase("", "hr", par, insert) { setSavable(FALSE); }
    virtual ~cfgLine() {}
};


#endif //_CFG_LINE_H_
