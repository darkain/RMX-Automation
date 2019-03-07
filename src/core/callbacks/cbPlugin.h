/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _PLUGIN_CALLBACK_H_
#define _PLUGIN_CALLBACK_H_


#include "../plugin.h"


//TODO:  add this to SDK API


class vPluginLoader;


class vPluginCallback {
friend class vPluginLoader;

public:
	vPluginCallback() { plg->appendCallback(this); }
	virtual ~vPluginCallback() { plg->removeCallback(this); }

protected:
	virtual void onAppendPlugin(vPlugin *plugin) {}
	virtual void onRemovePlugin(vPlugin *plugin) {}
};


#endif //_PLUGIN_CALLBACK_H_
