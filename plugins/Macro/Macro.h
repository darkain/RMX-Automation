/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/plugBase.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/cfg/cfgHelper.h"
#include "../../sdk/func/funcSingle.h"
#include "../../sdk/func/funcGroup.h"
#include "../../sdk/evnt/evntSingle.h"


class vMacro : public plugBase {
  public:
    vMacro();

  protected:
    virtual void onInit();

  public:
    inline funcGroup *getGroup() { return group; }

  private:
    funcGroup *group;
};


class vMacroTree : public cfgTree {
  public:
    vMacroTree(const char *name, cfgBase *parent);
    virtual ~vMacroTree();

  protected:
    virtual cfgBase *onNewExternalChild(const char *name, const char *type, HPARAMLIST paramlist);
    virtual void onPreLoadSettings();
};



class vMacroList : public cfgTree {
  public:
    vMacroList(const char *name, cfgBase *parent);
    ~vMacroList();

  protected:
    virtual cfgBase *onNewExternalChild(const char *name, const char *type, HPARAMLIST paramlist);
    virtual void onRemoveChild(const cfgBase* child);
    virtual void onNameChange(const char *newname);

  public:
    cfgBase *addEvent(const char *name=NULL);
    void activate();

  private:
    rmxList<evntSingle*>  events;
    funcSingle      *function;
};


class vMacroFunction : public funcSingle {
  public:
    vMacroFunction(const char *name, funcGroup *parent, vMacroList *list)
      : funcSingle(name, NULL, parent) {
      watch = list;
    }

    virtual ~vMacroFunction() {}

  protected:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
      watch->activate();
    }

  private:
    vMacroList *watch;
};


class vAddMacroItem : public cfgButton {
  public:
    vAddMacroItem(vMacroList *parent) : cfgButton("Add Macro Item", parent) {
      setButtonText("Add Macro Item");
      setSavable(FALSE);
      par = parent;
    }

    virtual ~vAddMacroItem() {}

  protected:
    virtual void onClick(HPLUGIN source) {
      cfgButton::onClick(source);
      par->addEvent();
    }

  private:
    vMacroList *par;
};


class vAddMacro : public cfgButton {
  public:
    vAddMacro(cfgBase *parent) : cfgButton("Add Macro", parent) {
      setButtonText("Add Macro");
      setSavable(FALSE);
    }

    virtual ~vAddMacro() {}

  protected:
    virtual void onClick(HPLUGIN source);
};
