/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CB_PLUGIN_H_
#define _CB_PLUGIN_H_


//TODO: implement this in the core


class cbPlugin {
protected:
	cbPlugin();
	virtual ~cbPlugin();

protected:
	virtual void cb_onInsertPlugin(HPLUGIN plugin) {}
	virtual void cb_onRemovePlugin(HPLUGIN plugin) {}
};


#endif //_CB_PLUGIN_H_
