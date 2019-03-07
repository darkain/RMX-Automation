/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CB_LIST_ITEM_H_
#define _CB_LIST_ITEM_H_


#include "cbCore.h"


class cbList;


class cbListItem : public cbCore {
  friend class cbList;

  protected:
    cbListItem(cbList *parent, HPLUGIN plugin, HLIST item);
    virtual ~cbListItem();

  private:
    cbList *parlist;
};


#endif //_CB_LIST_ITEM_H_