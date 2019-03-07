/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


//#include "../rmx_Win32/rmx_win32.h"
#include "../core/Loader.h"
#include "../../sdk/rmxString.h"
#include "../../sdk/rmxThread.h"
#include "../../sdk/func/funcSingle.h"


//class saPlugins : public vRmxWin32 {
class saPlugins : public vPluginLoader {
public:
	saPlugins();
	virtual ~saPlugins();

protected:
	virtual void initMasterLists(rmxMasterPlugin *master);
};


class saLaunchThread : public rmxThread {
public:
	saLaunchThread(const char *thepath, const char *theargs) : path(thepath), args(theargs) {}
	virtual ~saLaunchThread() {}
	virtual int threadProc();

private:
	rmxString path;
	rmxString args;
};


class saLaunch : public funcSingle {
public:
	saLaunch(cfgRoot *parent);
	virtual ~saLaunch();

protected:
	virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};


class saExitFunction : public funcSingle {
public:
	saExitFunction(cfgRoot *parent) : funcSingle("Exit", "exit", parent) {}
	virtual ~saExitFunction() {}

protected:
	virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
		plg->destroy();
	}
};
