/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/plugBase.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/cfg/cfgAutoList.h"
#include "../../sdk/cfg/cfgAddChild.h"
#include "../../sdk/evnt/evntSingle.h"


class MenuItem;
#define MenuTreeParent tcfgAutoEventList<cfgTree, MenuItem>


class vMenuPlugin : public plugBase {
public:
	vMenuPlugin();

protected:
	virtual void onInit();
	virtual void onQuit();

private:
	MenuTreeParent *tree;
};


class MenuTree : public MenuTreeParent {
public:
	MenuTree(const char *name, cfgBase *parent) : MenuTreeParent(name, parent) {}
	virtual ~MenuTree() {}

protected:
//	virtual void onMoveChild(const cfgBase *child, const cfgBase *insert);
	virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const cfgBase *child, const cfgBase *insert=NULL);
};


class MenuItem : public evntSingle {
public:
	MenuItem(const char *name, cfgBase *parent, HPARAMLIST paramlist);
	virtual ~MenuItem();

public:
	void updateText();

	inline cfgMenu *getMenu() const { return menuitem; }

private:
	cfgText *text;
	cfgMenu *menuitem;
};


class MenuItemClick : public cfgMenu {
public:
	MenuItemClick(evntSingle *par) : cfgMenu("Menu Item"), parent(par) {}
	virtual ~MenuItemClick() {}

protected:
	virtual void onClick(HPLUGIN source) { parent->activate(); }

private:
	evntSingle *parent;
};


class MenuText : public cfgText {
public:
	MenuText(MenuItem *parent) : cfgText("Item Text", parent), par(parent) { setText("Menu Item"); }
	virtual ~MenuText() {}

protected:
	virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert=NULL) {
		if (message == CB_PARAM_UPDATED) {
			if (param == getParam()) {
				par->updateText();
			}
		}
		cfgText::cb_cfgMessage(message, item, param, insert);
	}


private:
	MenuItem *par;
};
