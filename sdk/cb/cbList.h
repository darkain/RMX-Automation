/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CB_LIST_H_
#define _CB_LIST_H_


#include "cbPlugin.h"
#include "cbCore.h"
#include "../rmxList.h"


class cbListRoot;
class cbListItem;


class cbList : public cbPlugin {
  friend class cbListRoot;
  friend class cbListItem;

  protected:
    cbList();
    virtual ~cbList();

  protected:
    virtual cbListItem *validateConfig(HLIST config)=0;

  protected:
     virtual void cb_onInsertPlugin(HPLUGIN plugin);

  private:
    rmxList<cbListRoot*> roots;
    rmxList<cbListItem*> items;
};


#endif //_CB_LIST_H_
