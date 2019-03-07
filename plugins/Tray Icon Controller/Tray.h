/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/plugBase.h"
#include "../../sdk/Win32/TrayIcon.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/cfg/cfgHelper.h"
#include "../../sdk/cfg/cfgAutoList.h"
#include "../../sdk/cfg/cfgAddChild.h"
#include "../../sdk/evnt/evntSingle.h"


class vTrayPluginIcon;
#define TrayTreeParent tcfgAutoEventList<cfgTree, vTrayPluginIcon>


class vTrayPlugin : public plugBase {
  friend class vTrayPluginIcon;

  public:
    vTrayPlugin();

  protected:
    virtual void onInit();
    virtual void onQuit();

  public:
    void updateIcons();

  private:
    TrayTreeParent *tree;
};


class vTrayRoot : public TrayTreeParent {
  public:
    vTrayRoot(const char *name, cfgBase *parent) : TrayTreeParent(name ,parent) {}
    virtual ~vTrayRoot() {}

  protected:
	virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const cfgBase *child, const cfgBase *insert=NULL);
//    virtual void onMoveChild(const cfgBase *child, const cfgBase *insert);
};


class vTrayPluginIcon : public evntSingle, public vTrayIcon {
  public:
    vTrayPluginIcon(const char *name, cfgBase *parent, HPARAMLIST paramlist);
    virtual ~vTrayPluginIcon();

  public:
    virtual void onLeftClick(int x, int y) { activate(); }
    virtual void onRightClick(int x, int y);

    void moveUp();
    void moveDown();

    void update();

  private:
    cfgPath *path;
};


class vTrayPath : public cfgPath {
  public:
    vTrayPath(vTrayPluginIcon *par) : cfgPath("Icon Filename", par), parent(par) {}
    virtual ~vTrayPath() {}

	virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert) {
		if (message == CB_PARAM_UPDATED  &&  param == getParam()) {
			parent->setIcon( getPath() );
		}
		cfgPath::cb_cfgMessage(message, item, param, insert);
	}

/*
    virtual void onSetUserData(const prmBase *newdata) {
      cfgPath::onSetUserData(newdata);
      parent->setIcon(getPath());
    }
*/  
  private:
    vTrayPluginIcon *parent;
};
