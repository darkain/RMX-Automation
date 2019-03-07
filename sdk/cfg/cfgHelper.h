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


#ifndef _CFG_HELPER_H_
#define _CFG_HELPER_H_


#include "Config.h"


class cfgRemoveParent : public cfgButton {
  public:
    cfgRemoveParent(const char *initname, cfgBase *parent)
      : cfgButton(initname, parent) {
      setButtonText("Remove");
      setSavable(FALSE);
    }

    virtual ~cfgRemoveParent() {}

  protected:
    virtual void onClick(HPLUGIN source) {
      cfgButton::onClick(source);
      delete getParent();
    }
};


class cfgRenameParent : public cfgText {
  public:
    cfgRenameParent(const char *name, cfgBase *parent)
      : cfgText(name, parent) {
      setText(parent->getName());
      setSavable(FALSE);
    }

    virtual ~cfgRenameParent() {}

  protected:
	virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert=NULL) {
		if (message == CB_PARAM_UPDATED) {
			if (param == getParam()) {
				getParent()->setName( param->getValueSafe() );
			}
		}
		cfgText::cb_cfgMessage(message, item, param, insert);
	}
};


class cfgValidateRenameParent : public cfgRenameParent {
  public:
    cfgValidateRenameParent(const char *name, cfgBase *parent)
      : cfgRenameParent(name, parent) {
      oldval = getText();
    }

    virtual ~cfgValidateRenameParent() {}

  protected:
    virtual void validateUserData(prmBase *userdata) {
      cfgBase *thislist = this;
      if (!getParent()  ||  !getParent()->getParent()) return;
      cfgBase *par = getParent()->getParent();
      cfgBase *list = par->getFirstChild();
      while (list) {
        if (list != thislist) {
          const char *data = list->getName();
          if (data  &&  VSTRCMP(userdata->getValue(), data)==0) {
            userdata->setValue(oldval);
            return;
          }
        }
        list = par->getNextChild(list);
      }
      oldval = userdata->getValue();
    }

  private:
    rmxString oldval;
};


class cfgMoveParentUp : public cfgButton {
  public:
    cfgMoveParentUp(cfgBase *parent)
      : cfgButton("Move Up", parent) {
      setButtonText("Move Up");
      setSavable(FALSE);
    }

    virtual ~cfgMoveParentUp() {}

  protected:
    virtual void onClick(HPLUGIN source) {
      cfgButton::onClick(source);
      if (getParent()) getParent()->moveListUp();
    }
};


class cfgMoveParentDown : public cfgButton {
  public:
    cfgMoveParentDown(cfgBase *parent)
      : cfgButton("Move Down", parent) {
      setButtonText("Move Down");
      setSavable(FALSE);
    }

    virtual ~cfgMoveParentDown() {}

  protected:
    virtual void onClick(HPLUGIN source) {
      cfgButton::onClick(source);
      if (getParent()) getParent()->moveListDown();
    }
};


template<class TPARENT>
class tcfgMoveParentUp : public cfgButton {
  public:
    tcfgMoveParentUp(TPARENT *par) : cfgButton("Move Up", parent), parent(par) {
      setSavable(FALSE);
      setButtonText("Move Up");
    }

    virtual ~tcfgMoveParentUp() {}

  protected:
    virtual void onClick(HPLUGIN source) {
      cfgButton::onClick(source);
      parent->moveUp();
    }

  private:
    TPARENT *parent;
};


template<class TPARENT>
class tcfgMoveParentDown : public cfgButton {
  public:
    tcfgMoveParentDown(TPARENT *par) : cfgButton("Move Down", par), parent(par) {
      setSavable(FALSE);
      setButtonText("Move Down");
    }

    virtual ~tcfgMoveParentDown() {}

  protected:
    virtual void onClick(HPLUGIN source) {
      cfgButton::onClick(source);
      parent->moveDown();
    }

  private:
    TPARENT *parent;
};


#endif //_CFG_HELPER_H_
