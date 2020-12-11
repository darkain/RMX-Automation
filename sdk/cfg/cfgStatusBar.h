/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CFG_STATUSBAR_H_
#define _CFG_STATUSBAR_H_


#include "cfgBase.h"


class cfgStatusBar : public cfgBase {
  public:
    cfgStatusBar(cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    virtual ~cfgStatusBar();
};


#endif //_CFG_STATUSBAR_H_
