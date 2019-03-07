/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CB_LIST_ROOT_H_
#define _CB_LIST_ROOT_H_


#include "cbRoot.h"


class cbList;


class cbListRoot : private cbRoot {
  friend class cbList;

  private:
    cbListRoot(cbList *parent, HPLUGIN plugin);
    virtual ~cbListRoot();

  protected:
    virtual void cb_onInsertChild(HLIST child, HLIST insert);

  private:
    cbList *list;
};


#endif //_CB_LIST_ROOT_H_
