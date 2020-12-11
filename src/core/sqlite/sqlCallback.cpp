/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../../sdk/rmxBase.h"
#include "../../../sdk/rmxString.h"

#include "sqlCallback.h"
#include "sqlite.h"

#include "../loader.h"


sqlCallback::sqlCallback(HPLUGIN cb_plugin, HLIST cb_list, sqlCallback *par)
: cbCore(cb_plugin, cb_list), parent(par) {
	id   = 0;
	type = 0;

	//Assign Parent Information
	if (parent) {
		root = parent->root;
		parent->children.appendItem(this);
	} else {
		root = this;
	}


	//Link To Database Entry
	load(false);
	save(false);


	//Enumerate Child Objects
	HLIST child = cb_plugin->list_getFirstChild(cb_list);
	while (child) {
		new sqlCallback(getRemotePlugin(), child, this);
		child = cb_plugin->list_getNextChild(cb_list, child);
	}
}



sqlCallback::~sqlCallback() {
	sqlCallback *child = children.getFirstItem();
	while (child) {
		delete child;
		children.removeItem(child);
		child = children.getFirstItem();
	}

	if (parent) parent->children.removeItem(this);
}



void sqlCallback::cb_cfgMessage(cbMessage message, const char *text) {
	switch (message) {
		case CB_ON_NAMECHANGE: {
			bool ret = updateConfig(false);
			if (!ret) ret = insertConfig();
		} break;

	}
	cbCore::cb_cfgMessage(message, text);
}



void sqlCallback::cb_cfgMessage(cbMessage message, RMXLONG data) {
	switch (message) {
		case CB_SET_SAVABLE:
			if (data) save(true);
		break;

	}
	cbCore::cb_cfgMessage(message, data);
}



void sqlCallback::cb_cfgMessage(cbMessage message, HLIST  child, HLIST  insert) {
	switch (message) {
		case CB_CHILD_INSERT:
			new sqlCallback(getRemotePlugin(), child, this);
		break;
	}

	cbCore::cb_cfgMessage(message, child, insert);
}



void sqlCallback::cb_cfgMessage(cbMessage message, HPARAM param, HPARAM insert) {
	switch (message) {
		case CB_PARAM_INSERT:
			if (!selectParam(param)) insertParam(param);
		break;

		case CB_PARAM_UPDATED:
			if (!updateParam(param)) insertParam(param);
		break;

	}
	cbCore::cb_cfgMessage(message, param, insert);
}


__int64 sqlCallback::getTypeId() const {
	if (!type) type = plg->sql()->getObjectTypeId( getType() );
	return type;
}



__int64 sqlCallback::getId() {
	if (!id) load();
	return id;
}


void sqlCallback::setId(__int64 newid) {
	id = newid;
	cbCore::setId(newid);
}


bool sqlCallback::load(bool recursive) {
	bool ret = selectConfig();
	if (!ret) ret = insertConfig();
	if (!ret) return ret;

	if (recursive) {
		rmxList<sqlCallback*> items(children);
		sqlCallback *item = items.getFirstItem();
		while (item) {
			if (children.hasItem(item)) item->load(recursive);
			item = items.getNextItem(item);
		}
	}

	return ret;
}


bool sqlCallback::save(bool recursive) {
	bool ret = updateConfig();
	if (!ret) ret = insertConfig();
	if (!ret) return ret;

	if (recursive) {
		rmxList<sqlCallback*> items(children);
		sqlCallback *item = items.getFirstItem();
		while (item) {
			if (children.hasItem(item)) item->save(recursive);
			item = items.getNextItem(item);
		}
	}

	return ret;
}


bool sqlCallback::insertConfig() {
	HPLUGIN plugin = getRemotePlugin();
	HLIST   list   = getRemoteConfig();

	rmxString str;
	str += "INSERT INTO [objects] ([object_type_id],[root_id],[parent_id],[name]) VALUES (";
	
	str += getTypeId();
	if (root == this) {
		str += ",NULL,NULL,'";
	} else {
		str += ",";
		str += root->getId();
		str += ",";
		str += parent->getId();
		str += ",'";
	}

	//insert.concat_sql( getName() );  //TODO: sql safe version of append string
	str += getName();
	str += "')";

	int result = plg->sql()->sql(str);

	if (result != SQLITE_OK  ||  plg->sql()->getRowsChanged() < 1) return false;

	setId( plg->sql()->getInsertId() );

	if (isSavable()) {
		HPARAM param = plugin->list_getParamFirst(list);
		while (param) {
			insertParam(param);
			param = plugin->list_getParamNext(list, param);
		}
	}

	plugin->list_onFirstLoad(list);

	return true;
}



bool sqlCallback::insertParam(HPARAM param) {
	HPLUGIN plugin = getRemotePlugin();

	bool savable = (isSavable()  &&  plugin->param_isSavable(param));
	if (!savable) {
		if (param != getRemoteParam()) return false;
	}

	rmxString str;
	str += "INSERT INTO [params] ([param_name_id],[object_id],[param_value]) VALUES (";
	
	if (param == getRemoteParam()) {
		str += "NULL,";
	} else {
		str += plg->sql()->getParamNameId( plugin->param_getName(param) );
		str += ",";
	}
	
	str += getId();

	if (savable) {
		str += ",'";
		str += plugin->param_getValue(param);
		str += "')";
	} else {
		str += ",NULL)";
	}

	int result = plg->sql()->sql(str);

	if (result != SQLITE_OK  ||  plg->sql()->getRowsChanged() < 1) return false;

	plugin->param_setId( param, plg->sql()->getInsertId() );
	return true;
}



bool sqlCallback::updateConfig(bool update_params) {
	bool ret   = false;
	bool first = true;
	if (!isSavable()) return ret;


	rmxString str;
	str += "UPDATE [objects] SET [name]=? WHERE [object_id]=";
	str += getId();
//	str += " LIMIT 1"; //UPDATE LIMIT not supported by default SQLite

	const char *items[] = { getName() };
	sqlite3_stmt *statement = plg->sql()->sqlPrepare(str, items, 1);
	if (!statement) return ret;

	if (sqlite3_step(statement) == SQLITE_DONE) ret = true;
	sqlite3_finalize(statement);


	//Update params
	if (update_params) {
		HPLUGIN plugin = getRemotePlugin();
		HLIST   list   = getRemoteConfig();
		HPARAM  param  = plugin->list_getParamFirst(list);
		while (param) {
			if (!updateParam(param)) insertParam(param);
			param = plugin->list_getParamNext(list, param);
		}
	}

	return ret;
}


bool sqlCallback::updateParam(HPARAM param) {
	bool ret = false;

	HPLUGIN plugin = getRemotePlugin();
	__int64 id = plugin->param_getId(param);

	if (id == 0) return ret;

	rmxString str;
	str += "UPDATE [params] SET [param_value]=? WHERE [param_id]=";
	str += id;
//	str += " LIMIT 1"; //UPDATE LIMIT not supported by default SQLite

	const char *items[] = { plugin->param_getValueSafe(param) };

	sqlite3_stmt *statement = plg->sql()->sqlPrepare(str, items, 1);
	if (!statement) return ret;

	if (sqlite3_step(statement) == SQLITE_DONE) ret = true;
	sqlite3_finalize(statement);

	if (plg->sql()->getRowsChanged() < 1) ret = false;

	return ret;
}



bool sqlCallback::selectConfig(bool select_params) {
	bool ret = false;

	rmxString str;
	str += "SELECT [object_id], [param_id], [param_value] FROM [objects] LEFT OUTER JOIN [params] USING([object_id]) WHERE [param_name_id] IS NULL";

	if (id) {
		str += " AND [object_id]=";
		str += id;

	} else {
		str += " AND [object_type_id]=";
		str += getTypeId();

		if (root == this) {
			str += " AND root_id IS NULL";
		} else {
			str += " AND parent_id=";
			str += parent->getId();
		}
		str += " AND name=?";
	}

	str += " LIMIT 1";

	const char *items[] = { getName() };
	sqlite3_stmt *statement = plg->sql()->sqlPrepare(str, items, 1);
	if (!statement) return ret;

	int result = sqlite3_step(statement);

	if (result == SQLITE_ROW) {
		HPLUGIN plugin = getRemotePlugin();
		HPARAM  hparam = getRemoteParam();

		setId( sqlite3_column_int(statement, 0) );
		plugin->param_setId(hparam, sqlite3_column_int(statement, 1));

//		if (isSavable()) plugin->param_setValue( hparam, sqlite3_column_char(statement, 2) );

		ret = true;
	}

	sqlite3_finalize(statement);

	//TODO: if this is called and the params dont already exist in the DB, they wont be created!
	if (ret  &&  select_params) selectAllParams();

	return ret;
}


bool sqlCallback::selectParam(HPARAM param) {
	bool ret = false;
	HPLUGIN plugin  = getRemotePlugin();

	if (!param  ||  !isSavable()) return ret;
	if (!plugin->param_isSavable(param)) return ret;

	rmxString str;

	__int64 paramid = plugin->param_getId(param);
	if (paramid) {
		str += "SELECT [param_value] FROM [params] WHERE [param_id]=";
		str += paramid;
		str += " LIMIT 1";
	} else {
		str += "SELECT [param_id], [param_value] FROM [params] JOIN [param_names] USING ([param_name_id]) WHERE [object_id]=";
		str += getId();
		str += " AND [param_name]='";
		str += plugin->param_getName(param);
		str += "' LIMIT 1";
	}

	sqlite3_stmt *statement = plg->sql()->sqlPrepare(str);
	if (!statement) return ret;

	if (sqlite3_step(statement) == SQLITE_ROW) {
		ret = true;

		if (paramid) {
//			plugin->param_setValue( param, sqlite3_column_char(statement, 0) );
		} else {
			plugin->param_setId(    param, sqlite3_column_int( statement, 0) );
//			plugin->param_setValue( param, sqlite3_column_char(statement, 1) );
		}
	}

	sqlite3_finalize(statement);
	return ret;
}


bool sqlCallback::selectAllParams() {
	bool ret = false;

	if (!isSavable()) return ret;

	HPLUGIN plugin = getRemotePlugin();
	HLIST   list   = getRemoteConfig();

	rmxString str;
	str += "SELECT [param_id], [param_name], [param_value] FROM [params] LEFT OUTER JOIN [param_names] USING ([param_name_id]) WHERE [object_id]=";
	str += getId();

	sqlite3_stmt *statement = plg->sql()->sqlPrepare(str);
	if (!statement) return ret;

	while (sqlite3_step(statement) == SQLITE_ROW) {
		ret = true;

		HPARAM      param = plugin->list_getParamById(   list, sqlite3_column_int( statement, 0) );
		if (!param) param = plugin->list_getParamHandle( list, sqlite3_column_char(statement, 1) );
//		if ( param) plugin->param_setValue( param, sqlite3_column_char(statement, 2) );
	}

	sqlite3_finalize(statement);
	return ret;
}
