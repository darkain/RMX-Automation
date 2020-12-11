/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _DEBRIS_H_
#define _DEBRIS_H_


#include "../../sdk/plugBase.h"
#include "../../sdk/func/funcSingle.h"
#include "../../sdk/func/funcGroup.h"


class plugDebris : public plugBase {
  public:
    plugDebris();

  public:
    void command(UINT command);

  protected:
    virtual void onInit();
    virtual void onQuit();
};


class debrisCommand : public funcSingle {
  public:
    debrisCommand(const char *function_name, const char *script_name, funcGroup *parent, int debrisid);
    virtual ~debrisCommand() {}

  protected:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};


#endif //_DEBRIS_H_
