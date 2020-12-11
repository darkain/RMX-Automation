/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __SQL_PLUGIN_CALLBACK_H__
#define __SQL_PLUGIN_CALLBACK_H__


#include "../callbacks/cbPlugin.h"
#include "sqlCallback.h"


class sqlPluginCallback : public vPluginCallback {
public:
	sqlPluginCallback() {
		plg->sql()->beginTransaction();

		vPlugin *plugin = plg->getFirstPlugin();
		while (plugin) {
			HPLUGIN handle = plugin->getHandle();
			new sqlCallback(handle, handle->list_getRoot());
			plugin = plg->getNextPlugin(plugin);
		}

		plg->sql()->commitTransaction();
	}


	virtual ~sqlPluginCallback() {
	}


protected:
	virtual void onAppendPlugin(vPlugin *plugin) {
		plg->sql()->beginTransaction();

		HPLUGIN handle = plugin->getHandle();
		new sqlCallback(handle, handle->list_getRoot());

		plg->sql()->commitTransaction();
	}
};

#endif //__SQL_PLUGIN_CALLBACK_H__
