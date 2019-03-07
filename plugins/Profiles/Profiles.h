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
#include "../../sdk/cb/cbBase.h"
#include "../../sdk/func/funcSingle.h"
#include "../../sdk/func/funcGroup.h"
#include "../../sdk/evnt/evntSingle.h"


class vProfileTree;
class vProfileItem;
class vProfileTreeItem;
class funcSingleProfile;
class vProfileTreeRoot;


class vProfiles : public plugBase {
  friend class vProfileTree;
  friend class vProfileItem;

  public:
    vProfiles();

  protected:
    virtual void onInit();
    virtual void onQuit();
    virtual void onPreLoadSettings();

  public:
    inline funcGroup   *getGroup()    { return group;    }
    vProfileTree *getActiveTree();
    void setActiveTree(const char *item) { active->setText(item); }
    funcParam *getFunctionParam();

  private:
    cfgTree          *tree;
    funcSingleProfile     *function;
    funcGroup       *group;
    cfgText          *active;
    rmxList<vProfileTree*>  trees;
    rmxList<vProfileItem*>  items;
};


class vProfileTreeRoot : public cfgTree {
  public:
    vProfileTreeRoot(const char *name, cfgBase *parent);
    virtual ~vProfileTreeRoot();

  protected:
    virtual cfgBase *onNewExternalChild(const char *name, const char *type, HPARAMLIST paramlist);
};


class funcSingleProfile : public funcSingle {
  public:
    funcSingleProfile(cfgRoot *parent);
    virtual ~funcSingleProfile();

  protected:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);

  public:
    inline funcParam *getFunctionParam() { return param; }

  private:
    funcParam *param;
};


class vProfileTree : public cfgTree {
  public:
    vProfileTree(const char *name, cfgBase *parent);
    virtual ~vProfileTree();

  protected:
    virtual void onNameChange(const char *newname);

  public:
    void addItem(vProfileItem *item);
    evntSingle *getEvent(cfgBase *item);

    inline void appendTreeItem(vProfileTreeItem *item) { items.appendItem(item); }
    inline void removeTreeItem(vProfileTreeItem *item) { items.removeItem(item); }

  private:
    cfgBase *param;
    rmxList<vProfileTreeItem*> items;
};


class vProfileTreeItem : public evntSingle {
  public:
    vProfileTreeItem(const char *name, vProfileTree *parent, cfgBase *watch);
    virtual ~vProfileTreeItem();

  protected:
    virtual void cb_onSetUserData(const prmBase *userdata);

  private:
    vProfileTree *par;
};


class vAddTree : public cfgButton {
  public:
    vAddTree(cfgBase *parent) : cfgButton("Add profile", parent) {
      setButtonText("Add Profile");
      setSavable(FALSE);
    }

    virtual ~vAddTree() {}

  protected:
    virtual void onClick(HPLUGIN source);
};


class vProfileItem : public cfgText {
  public:
    vProfileItem(cfgBase *parent);
    virtual ~vProfileItem();

  protected:
    virtual void validateUserData(prmBase *userdata);
    virtual void onSetUserData(const prmBase *userdata);

  private:
    rmxName oldval;
    funcSingle *function;
};


class vAddItem : public cfgButton {
  public:
    vAddItem(cfgBase *parent) : cfgButton("Add profile item", parent) {
      setButtonText("Add Item");
      setSavable(FALSE);
    }

    virtual ~vAddItem() {}

  protected:
    virtual void onClick(HPLUGIN source);
};



class vItemFunction : public funcSingle {
  public:
    vItemFunction(const char *name, funcGroup *parent, cfgBase *item);
    virtual ~vItemFunction();

  protected:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);

  private:
    cfgBase *profileitem;
};
