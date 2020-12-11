/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CFG_BOOL_H_
#define _CFG_BOOL_H_


#include "cfgBase.h"


/*
this class uses UserData to store its displayed boolean value
*/

class cfgBool : public cfgBase {
  public:
    cfgBool(const char *initname, cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    virtual ~cfgBool();

  public:
    void setValue(BOOL value, BOOL force=FALSE);
    BOOL getValue() const;

  protected:
    virtual void validateUserData(prmBase *userdata);
};


#endif //_CFG_BOOL_H_
