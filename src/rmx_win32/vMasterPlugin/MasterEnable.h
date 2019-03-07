/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __RMXWIN32_MASTERENABLE_H__
#define __RMXWIN32_MASTERENABLE_H__


#include "../../../sdk/cfg/cfgBool.h"
#include "../../../sdk/cb/cbBase.h"
#include "../../../sdk//rmxLock.h"


class vMasterEnableBool : public cfgBool, rmxLock {
  public:
    vMasterEnableBool(cfgBase *parent, cfgBase *watch)
      : cfgBool("Enabled", parent) {
      setValue( watch->isEnabled() );
	  appendItem(watch);
    }

    virtual ~vMasterEnableBool() {}

  protected:
	virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert=NULL) {
		if (isUnlocked()  &&  message == CB_PARAM_UPDATED) {
			lock();
//TODO: FIX THIS SHIT!!
//			getConfig()->setEnabled( getValue() );
			unlock();
		}
		cfgBool::cb_cfgMessage(message, item, param, insert);
    }


    virtual void cb_cfgMessage(cbMessage message, cfgBase *item, RMXLONG data) {
		if (isUnlocked()  &&  message == CB_SET_ENABLED) {
			lock();
//TODO: FIX THIS SHIT!!
//			setValue( getConfig()->isEnabled() );
			unlock();
		}
		cfgBool::cb_cfgMessage(message, item, data);
    }
};


#endif //__RMXWIN32_MASTERENABLE_H__
