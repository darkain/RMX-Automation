/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


//class to recieve callbacks/events from a cfgBase


#ifndef _CB_BASE_H_
#define _CB_BASE_H_


#include "../rmxList.h"
#include "../cfg/prmBase.h"

class cfgBase;


typedef enum {
	CB_ON_DELETE          = 0x01,
	CB_ON_NAMECHANGE,

	CB_CHILD_INSERT       = 0x10,
	CB_CHILD_REMOVE,
	CB_CHILD_PREMOVE,
	CB_CHILD_MOVE,

	CB_PARAM_INSERT       = 0x20,
	CB_PARAM_REMOVE,
	CB_PARAM_PREMOVE,
	CB_PARAM_MOVE,
	CB_PARAM_UPDATED,

	CB_SET_VISIBLE        = 0x30,
	CB_SET_ENABLED,
	CB_SET_EXPANDED,
	CB_SET_HIGHLIGHTED,
	CB_SET_SAVABLE,
	CB_SET_READONLY,
} cbMessage;


class cbBase : protected rmxList<cfgBase*> {
	friend class cfgBase;

protected:
	cbBase(cfgBase *item=NULL);
	virtual ~cbBase();

protected:
	//Inherited from rmxList<> 
	//DON'T MESS WITH THESE
	virtual void onInsertItem(cfgBase *data, cfgBase *before);
	virtual void onDelete(cfgBase *data);


protected:

	//called when watched list is being deleted
	//default action is to delete this instance as well, as it will no longer be valid
	//NOTE: this will NOT be called for objects that inherit cfgBase
	virtual void cb_onDelete(cfgBase *item);


	virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const char *text) {}

	virtual void cb_cfgMessage(cbMessage message, cfgBase *item, RMXLONG data) {}

	virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const cfgBase *child, const cfgBase *insert=NULL) {}

	virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert=NULL) {}
};


#endif //_CFG_CALLBACK_H_
