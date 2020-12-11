/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __SQL_CALLBACK_H__
#define __SQL_CALLBACK_H__


#include "../../../sdk/cb/cbCore.h"


class sqlCallback : public cbCore {
public:
	sqlCallback(HPLUGIN cb_plugin, HLIST cb_list, sqlCallback *par=NULL);
	virtual ~sqlCallback();


protected:
	virtual void cb_cfgMessage(cbMessage message, const char *text);

	virtual void cb_cfgMessage(cbMessage message, RMXLONG data);

	virtual void cb_cfgMessage(cbMessage message, HLIST  child, HLIST  insert=NULL);

	virtual void cb_cfgMessage(cbMessage message, HPARAM param, HPARAM insert=NULL);

public:
	__int64 getTypeId() const;
	__int64 getId();
	void setId(__int64 newid);

	bool load(bool recursive=false);
	bool save(bool recursive=false);

private:
	bool insertConfig();
	bool insertParam(HPARAM param);

	bool updateConfig(bool update_params=true);
	bool updateParam(HPARAM param);

	bool selectConfig(bool select_params=true);
	bool selectParam(HPARAM param);
	bool selectAllParams();


private:
	mutable __int64 id;
	mutable __int64 type;
	sqlCallback *parent;
	sqlCallback *root;
	rmxList<sqlCallback*> children;
};


#endif // __SQL_CALLBACK_H__
