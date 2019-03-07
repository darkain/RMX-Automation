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


#ifndef _CFG_AUTOLIST_H_
#define _CFG_AUTOLIST_H_



#include "cfgBase.h"
#include "cfgButton.h"



#define TCFGAUTOLIST         tcfgAutoList<       TPARENT, TCHILD>
#define TCFGAUTOREFRESHLIST  tcfgAutoRefreshList<TPARENT, TCHILD>
#define TCFGAUTOEVENTLIST    tcfgAutoEventList<  TPARENT, TCHILD>



template <class TPARENT, class TCHILD>
class tcfgAutoList : public TPARENT {
  public:
    tcfgAutoList(const char *name, cfgBase *parent, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL)
      : TPARENT(name, parent, insert, paramlist) {

      automatic = TRUE;
      count     = 0;
    }
    
    virtual ~tcfgAutoList() {}


  protected:
    virtual BOOL isCorrectType(const char *type) { return TRUE; }


  public:
    inline void setAutomatic(BOOL a)   { automatic = a;    }
    inline BOOL isAutomatic()    const { return automatic; }
    inline int  getUniqueCount() const { return count;     }


    inline BOOL isUnique(const cfgBase *item) {
      if (!item) return FALSE;
      if (!hasChild(item)) return FALSE;
      return item->hasParam("unique");
    }


    int getUniqueIndex(const cfgBase *item) {
      int index = 0;

      cfgBase *child = getFirstChild();
      while (child) {
        if (child == item) return index;
        if (child->hasParam("unique")) index++;
        child = getNextChild(child);
      }

      return -1;
    }


    cfgBase *getFirstUnique() const {
      cfgBase *child = getFirstChild();
      while (child) {
        if (child->hasParam("unique")) return child;
        child = getNextChild(child);
      }
      return NULL;
    }


    cfgBase *getLastUnique() const {
      cfgBase *child = getLastChild();
      while (child) {
        if (child->hasParam("unique")) return child;
        child = getPrevChild(child);
      }
      return NULL;
    }


    cfgBase *getNextUnique(const cfgBase *child) const {
      child = getNextChild(child);
      while (child) {
        if (child->hasParam("unique")) return const_cast<cfgBase*>(child);
        child = getNextChild(child);
      }
      return NULL;
    }


    cfgBase *getPrevUnique(const cfgBase *child) const {
      child = getPrevChild(child);
      while (child) {
        if (child->hasParam("unique")) return const_cast<cfgBase*>(child);
        child = getPrevChild(child);
      }
      return NULL;
    }


    void removeAllUniques() {
      cfgBase *child = getFirstChild();
      while (child) {
        cfgBase *next = getNextChild(child);
        if (child->hasParam("unique")) delete child;
        child = next;
      }
    }


  protected:
    virtual cfgBase *onNewExternalChild(const char *name, const char *type, HPARAMLIST paramlist) {
      const char *unique = paramlist->getParamSafe("unique");
      if ( (*unique)  &&  (isCorrectType(type)) ) {
        cfgBase *item = new TCHILD(name, this, paramlist);
        if (item) item->initConfig();
        return item;
      }
      return TPARENT::onNewExternalChild(name, type, paramlist);
    }


    virtual BOOL isCorrectChild(cfgBase *child, HPARAMLIST paramlist) const {
      const char *in_unique  = child->getParam("unique");
      const char *out_unique = paramlist->getParam("unique");
      if (!in_unique  &&  !out_unique) return TRUE;
      if (!in_unique  ||  !out_unique) return FALSE;
      return (VSTRCMP(in_unique, out_unique) == 0);
    }


	virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const cfgBase *child, const cfgBase *insert=NULL) {
		if (message == CB_CHILD_INSERT) {
			if ( (isAutomatic())  &&  (!child->hasParam("unique")) ) {
				char str[20] = "";
				VPRINTF(str, sizeof(str), "%08X%08X", RAND(), RAND());
				child->insertParam("unique", str);
				child->setParamReadOnly("unique", TRUE);
				count++;
			} else if (child->hasParam("unique")) {
				count++;
			}
		}

		if (message == CB_CHILD_REMOVE) {
			if (child->hasParam("unique")) {
				count--;
			}
		}

		TPARENT::cb_cfgMessage(message, item, child, insert);
	}

/*
    virtual void onInsertChild(cfgBase *child, const cfgBase *insert) {
      if ( (isAutomatic())  &&  (!child->hasParam("unique")) ) {
        char str[20] = "";
        VPRINTF(str, sizeof(str), "%08X%08X", RAND(), RAND());
        child->insertParam("unique", str);
        child->setParamReadOnly("unique", TRUE);
        count++;
      } else if (child->hasParam("unique")) {
        count++;
      }
      TPARENT::onInsertChild(child, insert);
    }


    virtual void onRemoveChild(const cfgBase* child) {
      if (child->hasParam("unique")) {
        count--;
      }
      TPARENT::onRemoveChild(child);
    }
*/

  private:
    BOOL automatic;
    int count;
};



template <class TPARENT, class TCHILD>
class tcfgAutoRefreshList : public TCFGAUTOLIST {
  public:
    tcfgAutoRefreshList(const char *name, cfgBase *parent, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL)
      : TCFGAUTOLIST(name, parent, insert, paramlist) {
      setAutomatic(FALSE);
    }
    
    virtual ~tcfgAutoRefreshList() {}

  protected:
/*
    virtual void onPreLoadSettings() {
      setAutomatic(TRUE);
      removeAllUniques();
      TCFGAUTOLIST::onPreLoadSettings();
    }
*/
};



template <class TPARENT, class TCHILD>
class tcfgAutoEventList : public TCFGAUTOREFRESHLIST {
  public:
    tcfgAutoEventList(const char *name, cfgBase *parent, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL)
      : TCFGAUTOREFRESHLIST(name, parent, insert, paramlist) {
    }
    
    virtual ~tcfgAutoEventList() {}

  protected:
    virtual BOOL isCorrectType(const char *type) {
      if (!type) return FALSE;
      return (VSTRCMP(type, "event") == 0);
    }
};



template <class TCLASS>
class tcfgMoveUniqueUp : public cfgButton {
  public:
    tcfgMoveUniqueUp(const char *name, const char *text, cfgBase *par, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL)
      : cfgButton(name, text, par, insert, paramlist) {
      parent = static_cast<TCLASS*>(par->getParent());
      setSavable(FALSE);
    }

    virtual ~tcfgMoveUniqueUp() {}

  protected:
    virtual void onClick(HPLUGIN source) {
      cfgButton::onClick(source);
      cfgBase *item = parent->getPrevUnique(getParent());
      if (item) parent->moveChild(getParent(), item);
    }

  private:
    TCLASS *parent;
};



template <class TCLASS>
class tcfgMoveUniqueDown : public cfgButton {
  public:
    tcfgMoveUniqueDown(const char *name, const char *text, cfgBase *par, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL)
      : cfgButton(name, text, par, insert, paramlist) {
      parent = static_cast<TCLASS*>(par->getParent());
      setSavable(FALSE);
    }

    virtual ~tcfgMoveUniqueDown() {}

  protected:
    virtual void onClick(HPLUGIN source) {
      cfgButton::onClick(source);
      cfgBase *item = parent->getNextUnique(getParent());
      if (item) parent->moveChild(getParent(), parent->getNextUnique(item));
    }

  private:
    TCLASS *parent;
};



#endif //_CFG_AUTOLIST_H_
