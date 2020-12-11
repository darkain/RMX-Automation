/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "cbBase.h"
#include "../cfg/cfgMessage.h"
#include "../handles/hplugin.h"


#ifndef _CB_CORE_H_
#define _CB_CORE_H_


class cbCore : public cbBase {
protected:
	cbCore(HPLUGIN plugin, HLIST item);
	virtual ~cbCore();

public:
	inline HPLUGIN getRemotePlugin() const { return plug; }
	inline HLIST   getRemoteConfig() const { return list; }
	inline HPARAM  getRemoteParam()  const { return prm;  }
	void    setRemoteConfig(HLIST item);

public:
//for all other callbacks, they are the exact same as cbBase

	virtual void cb_onDelete(HLIST list) {}

	virtual void cb_cfgMessage(cbMessage message, const char *text) {}

	virtual void cb_cfgMessage(cbMessage message, RMXLONG data) {}

	virtual void cb_cfgMessage(cbMessage message, HLIST  child, HLIST  insert=NULL) {}

	virtual void cb_cfgMessage(cbMessage message, HPARAM param, HPARAM insert=NULL) {}
  
/*
//called when user data is changed.  (such as string, int, etc value, or vSetting function string
virtual void cb_onSetUserData(HPARAM userdata) {}

//called when a param is added to the watched list
virtual void cb_onInsertParam(HPARAM param, HPARAM insert) {}

//called when a param is removed from the watched list
virtual void cb_onRemoveParam(HPARAM param) {}

//called when the posisition of a param with a list is changed
virtual void cb_onMoveParam(HPARAM param, HPARAM insert) {}

//called when a param is set to a new string
virtual void cb_onSetParam(HPARAM param) {}

//called when a new child list is added to the watched list
virtual void cb_onInsertChild(HLIST child, HLIST insert) {}

//called when a child list is removed from the watched list
virtual void cb_onRemoveChild(HLIST child) {}

//called when a child is moved within the list to a new location
virtual void cb_onMoveChild(HLIST child, HLIST insert) {}
*/

public:  //tons of helper functions

	inline const char *getName()      const { return plug->list_getName(list); }
	inline const char *getType()      const { return plug->list_getType(list); }
	inline __int64     getId()        const { return plug->list_getId(  list); }
	inline void        setId(__int64 newid) { plug->list_setId(list,   newid); }

	inline HPARAM getParam() const { return plug->list_getListParam(list); }

	inline const char *getParam(    const char *param)                      const { return plug->list_getParam(    list, param);       }
	inline const char *getParamSafe(const char *param, const char *safe="") const { return plug->list_getParamSafe(list, param, safe); }
	inline void        setParam(    const char *param, const char *data)          {        plug->list_setParam(    list, param, data); }

	inline BOOL isParamUserData(HPARAM param) const { return plug->list_isParamUserData(list, param); }

	inline int getParamInt( const char *param) const { return plug->list_getParamInt(list, param);       }
	inline void setParamInt(const char *param, int data) {    plug->list_setParamInt(list, param, data); }

	inline BOOL isVisible()     const { return plug->list_isVisible(    list); }
	inline BOOL isEnabled()     const { return plug->list_isEnabled(    list); }
	inline BOOL isHighlighted() const { return plug->list_isHighlighted(list); }
	inline BOOL isExpanded()    const { return plug->list_isExpanded(   list); }
	inline BOOL isSavable()     const { return plug->list_isSavable(    list); }

	inline void setVisible(    BOOL visible)     { plug->list_setVisible(    list, visible);     }
	inline void setEnabled(    BOOL enable)      { plug->list_setEnabled(    list, enable);      }
	inline void setHighlighted(BOOL highlighted) { plug->list_setHighlighted(list, highlighted); }
	inline void setExpanded(   BOOL expand)      { plug->list_setExpanded(   list, expand);      }
	inline void setSavable(    BOOL savable)     { plug->list_setSavable(    list, savable);     }
/*
	inline const char *getUserData()                        const { return plug->list_getUserData(      list      ); }
	inline const char *getUserDataSafe(const char *safe="") const { return plug->list_getUserDataSafe(  list, safe); }
	inline void setUserData(           const char *data)          {        plug->list_setUserData(      list, data); }
	inline void setUserDataString(     const char *data)          {        plug->list_setUserDataString(list, data); }
	inline int  getUserDataInt()                            const { return plug->list_getUserDataInt(   list      ); }
	inline void setUserDataInt(int data)                          {        plug->list_setUserDataInt(   list, data); }
*/
	inline int getChildCount() const { return plug->list_getChildCount(list); }
	inline int getParamCount() const { return plug->list_getParamCount(list); }

	inline HLIST getFirstChild()          const { return plug->list_getFirstChild(list);      }
	inline HLIST getNextChild(HLIST prev) const { return plug->list_getNextChild(list, prev); }

	inline RMXLONG message(RMXLONG message, RMXLONG param1=0, RMXLONG param2=0) {
		return plug->list_message(list, message, param1, param2);
	}


private:
	HPLUGIN plug;
	HLIST   list;
	HPARAM  prm;
};


#endif //_CB_CORE_H_
