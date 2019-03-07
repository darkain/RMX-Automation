/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CFG_TREE_H_
#define _CFG_TREE_H_


#include "cfgBase.h"


/*
this class currently does not use UserData, not sure if will in future
*/


class cfgTree : public cfgBase {
  public:
    cfgTree(const char *itemName, cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    virtual ~cfgTree();

    const char *getTitle() const { return getParam("title"); }
    void setTitle(const char *title) { setParam("title", title); }
};


#endif //_CFG_TREE_H_
