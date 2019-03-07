/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _EVNT_PRESS_H_
#define _EVNT_PRESS_H_


#include "evntBase.h"


class evntPress : public evntBase {
  public:
    evntPress(const char *initname, cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    virtual ~evntPress();

  public:
    void activatePress();
    void deactivatePress();
    void setPress(BOOL press);
};


#endif //_EVNT_PRESS_H_
