/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cfgBase.h"
#include "cfgRoot.h"


cfgBase *cfgBase::rootlist = NULL;


cfgBase::cfgBase(const char *name, const char *vtype, cfgBase *listParent, cfgBase *insert, HPARAMLIST paramlist)
 : prmBase("name", name), type(vtype) {

	id = 0;

	cbBase::appendItem(this);

	setParamConfig(this);

	//default flag values
	flags.visible     = true;
	flags.enabled     = true;
	flags.expanded    = false;
	flags.highlighted = false;
	flags.expanded_ro = false;
	flags.queryparams = true;
	flags.inited      = false;

	//parent stuffz
	parent = listParent;
	if (!parent) parent = rootlist;
	if (parent) {
		flags.security = parent->getSecurity();
		parent->insertChild(this, insert);
	}

	//predefined params  (do this last due to inherited security)
	setParams(paramlist);
}


cfgBase::~cfgBase() {
	callbacks.removeItem(this);
	params.removeItem(this);

	if (parent) parent->removeChild(this);

	cbBase *cb = callbacks.getFirstItem();
	while (cb) {
		cb->cb_onDelete(this);
		callbacks.removeItem(cb);
		cb = callbacks.getFirstItem();
	}

	setReadOnly(FALSE);  //fixes a mem leak
	removeAllChildren();
	removeAllParams();
}



//NOTE: for safety, we clone the list, enumerate, and ensure items still exist.
//      the callback function may delete that object or other callback objects.
//      any NEW callbacks created from this callback function will NOT be called.
void cfgBase::dispatchMessage(cbMessage message, const char *text) {
	rmxList<cbBase*> cbs(callbacks);
	cbBase *cb = cbs.getFirstItem();
	while (cb) {
		if (callbacks.hasItem(cb)) cb->cb_cfgMessage(message, this, text);
		cb = cbs.getNextItem(cb);
	}
}


void cfgBase::dispatchMessage(cbMessage message, RMXLONG data) {
	rmxList<cbBase*> cbs(callbacks);
	cbBase *cb = cbs.getFirstItem();
	while (cb) {
		if (callbacks.hasItem(cb)) cb->cb_cfgMessage(message, this, data);
		cb = cbs.getNextItem(cb);
	}
}


void cfgBase::dispatchMessage(cbMessage message, prmBase *param) {
	if (message == CB_PARAM_INSERT) param->rmxDataUnlock();

	rmxList<cbBase*> cbs(callbacks);
	cbBase *cb = cbs.getFirstItem();
	while (cb) {
		if (callbacks.hasItem(cb)) cb->cb_cfgMessage(message, this, param);
		cb = cbs.getNextItem(cb);
	}
}


void cfgBase::dispatchMessage(cbMessage message, const cfgBase *child, const cfgBase *insert) {
	rmxList<cbBase*> cbs(callbacks);
	cbBase *cb = cbs.getFirstItem();
	while (cb) {
		if (callbacks.hasItem(cb)) cb->cb_cfgMessage(message, this, child, insert);
		cb = cbs.getNextItem(cb);
	}
}



void cfgBase::initConfig() {
	if (isInited()) return;
	if (parent  &&  !parent->isInited()) return;

	flags.inited = TRUE;

	onInitConfig();

	cfgBase *child = children.getFirstItem();
	while (child) {
		child->initConfig();
		child = children.getNextItem(child);
	}

	if (parent) {
		parent->dispatchMessage(CB_CHILD_INSERT, this, parent->getNextChild(this));
	}
}





//VVVVVVVVVVVVVVVVVVVVVVVV//
// basic list information //
//^^^^^^^^^^^^^^^^^^^^^^^^//

void cfgBase::setName(const char *name) {
	if (isReadOnly()) return;
	setValueString(name);
	dispatchMessage(CB_ON_NAMECHANGE, name);
}





//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV//
// list properties and access rights //
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//

//visible
void cfgBase::setVisible(BOOL visible) {
	if (isReadOnly()) return;
	visible = !!visible;
	if (flags.visible == (unsigned)visible) return;
	flags.visible = visible;
	dispatchMessage(CB_SET_VISIBLE, visible);
}

BOOL cfgBase::hasChildrenVisible(BOOL recursive) const {
	cfgBase *list = children.getFirstItem();
	while (list) {
		if (list->isVisible() == TRUE) return TRUE;
		if (recursive) {
			if (list->hasChildrenVisible(TRUE)) return TRUE;
		}
		list = children.getNextItem(list);
	}
	return FALSE;
}


//enabled
void cfgBase::setEnabled(BOOL enabled) {
	if (isReadOnly()) return;
	enabled = !!enabled;
	if (flags.enabled == (unsigned)enabled) return;
	flags.enabled = enabled;
	dispatchMessage(CB_SET_ENABLED, enabled);
}

BOOL cfgBase::hasChildrenEnabled(BOOL recursive) const {
	cfgBase *list = children.getFirstItem();
	while (list) {
		if (list->isEnabled() == TRUE) return TRUE;
		if (recursive) {
			if (list->hasChildrenEnabled(TRUE)) return TRUE;
		}
		list = children.getNextItem(list);
	}
	return FALSE;
}


//expanded
void cfgBase::setExpanded(BOOL expanded) {
	if (isReadOnly()) return;
	expanded = !!expanded;
	if (flags.expanded == (unsigned)expanded) return;
	flags.expanded = expanded;
	dispatchMessage(CB_SET_EXPANDED, expanded);
}

BOOL cfgBase::hasChildrenExpanded(BOOL recursive) const {
	cfgBase *list = children.getFirstItem();
	while (list) {
		if (list->isExpanded() == TRUE) return TRUE;
		if (recursive) {
			if (list->hasChildrenExpanded(TRUE)) return TRUE;
		}
		list = children.getNextItem(list);
	}
	return FALSE;
}


//highlighted
void cfgBase::setHighlighted(BOOL highlighted) {
	if (isReadOnly()) return;
	highlighted = !!highlighted;
	if (flags.highlighted == (unsigned)highlighted) return;
	flags.highlighted = highlighted;
	dispatchMessage(CB_SET_HIGHLIGHTED, highlighted);
}

BOOL cfgBase::hasChildrenHighlighted(BOOL recursive) const {
	cfgBase *list = children.getFirstItem();
	while (list) {
		if (list->isHighlighted() == TRUE) return TRUE;
		if (recursive) {
			if (list->hasChildrenHighlighted(TRUE)) return TRUE;
		}
		list = children.getNextItem(list);
	}
	return FALSE;
}


//loadable/savable to XML/SQL
void cfgBase::setSavable(BOOL savable) {
	if (isReadOnly()) return;
	savable = !!savable;
	if (flags.savable == (unsigned)savable) return;
	flags.savable = savable;
	dispatchMessage(CB_SET_SAVABLE, savable);

}

BOOL cfgBase::hasChildrenSavable(BOOL recursive) const {
	cfgBase *list = children.getFirstItem();
	while (list) {
		if (list->isSavable() == TRUE) return TRUE;
		if (recursive) {
			if (list->hasChildrenSavable(TRUE)) return TRUE;
		}
		list = children.getNextItem(list);
	}
	return FALSE;
}


//read only from everything?
void cfgBase::setReadOnly(BOOL readonly) {
	readonly = !!readonly;
	if (flags.readonly == (unsigned)readonly) return;
	flags.readonly = readonly;
	dispatchMessage(CB_SET_READONLY, readonly);
}

BOOL cfgBase::hasChildrenReadOnly(BOOL recursive) const {
	cfgBase *list = children.getFirstItem();
	while (list) {
		if (list->isReadOnly() == TRUE) return TRUE;
		if (recursive) {
			if (list->hasChildrenReadOnly(TRUE)) return TRUE;
		}
		list = children.getNextItem(list);
	}
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////


//VVVVVVVVVVVVVVVV//
// list paramater //
//^^^^^^^^^^^^^^^^//

// basic get/set param info
const char *cfgBase::getParam(const char *name) const {
	prmBase *param = getParamData(name);
	return (param ? param->getValue() : NULL);
}

const char *cfgBase::getParamSafe(const char *name, const char *safe) const {
	prmBase *param = getParamData(name);
	if (!param) return safe;
	const char *value = param->getValue();
	return (value ? value : safe);
}

int cfgBase::getParamInt(const char *name) const {
	prmBase *param = getParamData(name);
	return (param ? param->getValueInt() : 0);
}

RMXLONG cfgBase::getParamLong(const char *name) const {
	prmBase *param = getParamData(name);
	return (param ? param->getValueLong() : 0);
}

double cfgBase::getParamNumber(const char *name) const {
	prmBase *param = getParamData(name);
	return (param ? param->getValueNumber() : 0);
}

GUID cfgBase::getParamGuid(const char *name) const {
	prmBase *param = getParamData(name);
	return (param ? param->getValueGuid() : _INVALID_GUID);
}


prmBase *cfgBase::setParamValue(const char *string, char separator) {
	if (isReadOnly()  ||  !string  ||  separator == NULL) return NULL;

	rmxString str(string);
	int pos = str.find(separator);
	if (pos < 1) return setParamNull(string);

	char sep[2] = { separator, 0 };
	str.replace(pos, 1, sep);
	const char *data = str;
	return setParam(data, data + pos + 1 );
}


prmBase *cfgBase::setParam(const char *name, const char *value) {
	if (isReadOnly()  ||  !name  ||  !*name) return NULL;
	prmBase *param = getParamData(name);
	if (!param) {
		param = new prmBase(name, value);
		param->setParamConfig(this);
	} else param->setValue(value);
	return param;
}

prmBase *cfgBase::setParam(const char *name, const rmxData *value) {
	if (isReadOnly()  ||  !name  ||  !*name  ||  !value) return NULL;
	prmBase *param = getParamData(name);
	if (!param) {
		param = new prmBase(name);
		value->clone(param);
		param->setParamConfig(this);
	} else param->setValue(value);
	return param;
}

prmBase *cfgBase::setParamString(const char *name, const char *value) {
	if (isReadOnly()  ||  !name  ||  !*name) return NULL;
	prmBase *param = getParamData(name);
	if (!param) {
		param = new prmBase(name, value);
		param->setParamConfig(this);
	} else param->setValueString(value);
	return param;
}

prmBase *cfgBase::setParamInt(const char *name, int value) {
	if (isReadOnly()  ||  !name  ||  !*name) return NULL;
	prmBase *param = getParamData(name);
	if (!param) {
		param = new prmBase(name, value);
		param->setParamConfig(this);
	} else param->setValueInt(value);
	return param;
}

prmBase *cfgBase::setParamLong(const char *name, RMXLONG value) {
	if (isReadOnly()  ||  !name  ||  !*name) return NULL;
	prmBase *param = getParamData(name);
	if (!param) {
		param = new prmBase(name, value);
		param->setParamConfig(this);
	} else param->setValueLong(value);
	return param;
}

prmBase *cfgBase::setParamNumber(const char *name, double value) {
	if (isReadOnly()  ||  !name  ||  !*name) return NULL;
	prmBase *param = getParamData(name);
	if (!param) {
		param = new prmBase(name, value);
		param->setParamConfig(this);
	} else param->setValueNumber(value);
	return param;
}

prmBase *cfgBase::setParamGuid(const char *name, GUID value) {
	if (isReadOnly()  ||  !name  ||  !*name) return NULL;
	prmBase *param = getParamData(name);
	if (!param) {
		param = new prmBase(name, value);
		param->setParamConfig(this);
	} else param->setValueGuid(value);
	return param;
}


void cfgBase::setParams(HPARAMLIST paramlist) {
	if (isReadOnly()  ||  !paramlist) return;

	const char *prm = paramlist->getFirstParam();
	while (prm) {
		if (!VSTRCMP(prm, "name")) {}
		else if (!VSTRCMP(prm, "expanded")) {}
		else {
			prmBase *param = new prmBase(prm, paramlist->getParam(prm));
			param->setParamConfig(this);
		}
		prm = paramlist->getNextParam(prm);
	}
}



//what params do we have?
prmBase *cfgBase::getParamData(const char *name) const {
	if (!name  ||  !*name) return NULL;

	prmBase *param = params.getCachedItem();
	if (param  &&  param->isNamed(name)) return param;

	param = params.getFirstItem();
	while (param) {
		if (param->isNamed(name)) return param;
		param = params.getNextItem(param);
	}

	return NULL;
}


prmBase *cfgBase::getParamById(__int64 id) const {
	if (id == 0) return NULL;

	prmBase *param = params.getCachedItem();
	if (param  &&  param->getId() == id) return param;

	param = params.getFirstItem();
	while (param) {
		if (param->getId() == id) return param;
		param = params.getNextItem(param);
	}

	return NULL;
}


__int64 cfgBase::getParamId(const char *name) const {
	prmBase *param = getParamData(name);
	return (param ? param->getId() : 0);
}

void cfgBase::setParamId(const char *name, __int64 newid) {
	prmBase *param = getParamData(name);
	if (param) param->setId(newid);
}



//what are the param's flags?
UINT cfgBase::getParamFlags(const char *name) const {
	prmBase *param = getParamData(name);
	return (param ? param->flags.flags : 0);
}


void cfgBase::setParamSavable(const char *name, BOOL savable) {
	if (isReadOnly()) return;
	prmBase *param = getParamData(name);
	if (param) param->setSavable(!!savable);
}

BOOL cfgBase::isParamSavable(const char *name) const {
	prmBase *param = getParamData(name);
	return (param ? param->isSavable() : FALSE);
}

void cfgBase::setParamReadOnly(const char *name, BOOL readonly) {
	if (isReadOnly()) return;
	prmBase *param = getParamData(name);
	if (param) param->setReadOnly(!!readonly);
}

BOOL cfgBase::isParamReadOnly(const char *name) const {
	prmBase *param = getParamData(name);
	return (param ? param->isReadOnly() : FALSE);
}


//remove a param
void cfgBase::removeParam(prmBase *param) {
	if (isReadOnly()) return;
	if (!param) return;
	if (!params.hasItem(param)) return;
	params.removeItem(param);
	dispatchMessage(CB_PARAM_REMOVE, param);
	delete param;
}


void cfgBase::removeAllParams() {
	if (isReadOnly()) return;
	prmBase *param = params.getFirstItem();
	while (param) {
		removeParam(param);
		param = params.getFirstItem();
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////

//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV//
//list hierarchy (parent info, child info, etc) //
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//
cfgBase *cfgBase::getChild(const char *name, const char *type, HPARAMLIST paramlist) const {
	if (name == NULL || *name == NULL) return NULL;

	BOOL allTypes = (type == NULL  ||  *type == NULL);

	cfgBase *child = children.getFirstItem();
	while (child) {
		if (!VSTRCMP(child->getName(), name)) {
			if (allTypes  ||  !VSTRCMP(child->getType(), type)) {
				if (isCorrectChild(child, paramlist)) return child;
			}
		}
		child = children.getNextItem(child);
	}

	return NULL;
}


cfgBase *cfgBase::getChildById(RMXLONG rmxid, BOOL recursive) const {
	cfgBase *child = children.getFirstItem();
	while (child) {
		if (child->getId() == rmxid) return child;
		if (recursive) {
			cfgBase *item = child->getChildById(rmxid, recursive);
			if (item) return item;
		}
		child = children.getNextItem(child);
	}
	return NULL;
}


BOOL cfgBase::hasChild(const cfgBase *item, BOOL recursive) {
	if (!item)      return FALSE;
	if (!recursive) return children.hasItem(const_cast<cfgBase*>(item));
	if (children.getCachedItem() == item) return TRUE;

	cfgBase *child = children.getFirstItem();
	while (child) {
		if (child == item) return TRUE;
		if (child->hasChild(item, recursive)) return TRUE;
		child = children.getNextItem(child);
	}

	return FALSE;
}


//similar to hasChild, but only checks item cache, and is always recursive
BOOL cfgBase::hasChildCached(const cfgBase *item) {
	cfgBase *child = children.getCachedItem();
	return ( child  &&  ( (child == item)  ||  (child->hasChildCached(item)) ) );
}


//remove children
void cfgBase::removeAllChildren() {
	if (isReadOnly()) return;
	cfgBase *child = children.getFirstItem();
	while (child) {
		delete child;  //deleting the child will automatically run callbacks
		children.removeItem(child);
		child = children.getFirstItem();
	}
}


void cfgBase::moveListUp() {
	if (isReadOnly()) return;
	cfgBase *parent = getParent();
	if (!parent) return;
	cfgBase *prev = parent->getPrevChild(this);
	if (prev) parent->moveChild(this, prev);
}


void cfgBase::moveListDown() {
	if (isReadOnly()) return;
	cfgBase *parent = getParent();
	if (!parent) return;
	cfgBase *next = parent->getNextChild(parent->getNextChild(this));
	parent->moveChild(this, next);
}


void cfgBase::moveListToBegining() {
	if (isReadOnly()) return;
	cfgBase *parent = getParent();
	if (!parent) return;
	cfgBase *first = parent->getFirstChild();
	if (first) parent->moveChild(this, first);
}


void cfgBase::moveListToEnd() {
	if (isReadOnly()) return;
	cfgBase *parent = getParent();
	if (!parent) return;
	parent->moveChild(this, NULL);
}


void cfgBase::moveChild(const cfgBase *child, const cfgBase *insert) {
	if (isReadOnly()) return;
	dispatchMessage(CB_CHILD_PREMOVE, child, insert);
	children.moveItem(const_cast<cfgBase*>(child), const_cast<cfgBase*>(insert));
	dispatchMessage(CB_CHILD_MOVE, child, insert);
}


void cfgBase::removeChild(cfgBase *child) {
	if (!children.hasItem(child)) return;
	children.removeItem(child);
	dispatchMessage(CB_CHILD_REMOVE, child);  //TODO: see is this is even needed...?
}
