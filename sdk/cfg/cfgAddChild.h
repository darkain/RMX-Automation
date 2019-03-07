/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


/***********************************************************************\
* this file contains a bunch of pre-made helper classes for cfgBase     *
* - none of these classes directly rely on UserData                     *
\***********************************************************************/


#ifndef _CFG_ADDCHILD_H_
#define _CFG_ADDCHILD_H_


#include "cfgButton.h"


template <class TCHILD>
class tcfgAddChild : public cfgButton {
  public:
    tcfgAddChild(const char *name, const char *text, cfgBase *parent, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL)
      : cfgButton(name, "Add", parent, insert, paramlist) {
      setSavable(FALSE);
      insertParamNoSave("addtext", text);
    }

    virtual ~tcfgAddChild() {}

  protected:
    virtual void onCreateChild(TCHILD *child) {}

  protected:
    virtual void onClick(HPLUGIN source) {
      TCHILD *child = new TCHILD(getParam("addtext"), getParent(), NULL);
      onCreateChild(child);
      child->initConfig();
      cfgButton::onClick(source);
    }
};


template <class TCHILD>
class tcfgAddChildExpand : public tcfgAddChild<TCHILD> {
  public:
    tcfgAddChildExpand(const char *name, const char *text, cfgBase *parent, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL)
      : tcfgAddChild<TCHILD>(name, text, parent, insert, paramlist) {}

    virtual ~tcfgAddChildExpand() {}

  protected:
    virtual void onCreateChild(TCHILD *child) {
      child->setExpanded(TRUE);
      tcfgAddChild<TCHILD>::onCreateChild(child);
    }
};


#endif //_CFG_ADDCHILD_H_
