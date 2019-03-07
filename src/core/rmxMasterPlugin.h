/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __RMX_MASTER_PLUGIN_H__
#define __RMX_MASTER_PLUGIN_H__


#include "LocalPlugin.h"


class rmxMasterPlugin : public vLocalPlugin {
public:
	rmxMasterPlugin(const char *path);
	virtual ~rmxMasterPlugin();

protected:
    virtual RMXLONG onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2);
};


#endif //__RMX_MASTER_PLUGIN_H__
