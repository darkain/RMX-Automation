/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


/***********************************************************************\
*   ALL class within any library that inherit from the cfgBase          *
*   class must strictly state their UserDate usage, as there is only    *
*   one user data per instance, and this is the primary peice of        *
*   information that will be passed around between object instances     *
*   and even application instances.                                     *
\***********************************************************************/


#ifndef _CFG_BASE_H_
#define _CFG_BASE_H_


#include "../rmxBase.h"
#include "../rmxList.h"
#include "../rmxString.h"
#include "../rmxTypes.h"
#include "../cb/cbBase.h"
#include "prmBase.h"
#include "cfgMessage.h"


class cfgRoot;
class prmBase;


//this class will auto-save/load, but will NOT be shown in the configuration screen
//Note:  children automatically delete themselves when the parent is deleted
class cfgBase : public prmBase, public cbBase {
friend class cfgRoot;
friend class cbBase;
friend class prmBase;

public:
	cfgBase(const char *name, const char *vtype, cfgBase *listParent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
	virtual ~cfgBase();

protected:
	void dispatchMessage(cbMessage message, const char *text);
	void dispatchMessage(cbMessage message, RMXLONG data);
	void dispatchMessage(cbMessage message, prmBase *param);
	void dispatchMessage(cbMessage message, const cfgBase *child, const cfgBase *insert=NULL);


public:
	void initConfig();


	//VVVVVVVVVVVVVVVVVVV//
	// basic list events //
	//^^^^^^^^^^^^^^^^^^^//

	virtual void onInitConfig() {}

	virtual void onFirstLoad() {}



	//we received a custom message
	virtual RMXLONG onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2) { return 0; }


	//this function is called when someone/thing tries to set UserData for our object
	//in here we can verify that the data being passed in is correct for what we need
	//example:  IntMax may be "5", you try to set UserData to "10", this could validate it to "5" and return "5"
	//example:  If we want only Int based data, and try to pass it "a", it could return NULL to prevent data from being changed
	//by default, what ever is passed in counts as valid and returns the same value.  orverride for your custom validation
	virtual void validateUserData(prmBase *userdata) {}

	//same as validateUserData, but for params instead
	virtual void validateParam(prmBase *param) {}


	//this function is called when the XML parser sees a child entity to your object that doesnt exist
	//this is a great way to create dynamically generated lists, but it is also a great way to break things
	//if you use this to create a new item... RETURN THE POINTER TO THE NEW ITEM!!!!
	//this is a 100% must so that way the internal XML system can keep track of whats going on
	//return NULL if you dont create any new objects
	virtual cfgBase *onNewExternalChild(const char *name, const char *type, HPARAMLIST paramlist) { return NULL; }

protected:
	virtual BOOL isCorrectChild(cfgBase *child, HPARAMLIST paramlist) const { return TRUE; }


public:

	//similar to onNewExternalChild(), only this is called when there is an XML param we currently dont
	//have registered in our class.  by default, we do nothing.  if you add the param, return
	//a pointer to the new prmBase, just like how it is done in onNewExternalChild()
	virtual prmBase *onNewExternalParam(const char *name, const char *value) { return NULL; }


	//ID information. Do no assign IDs yourself. This is handled by the RMX Core instead
	inline void    setId(__int64 newid) { id = newid; }
	inline __int64 getId()        const { return  id; }
	inline static __int64 getRootId()   { return (rootlist ? rootlist->getId() : 0); }


	//VVVVVVVVVVVVVVVVVVVVVVVV//
	// basic list information //
	//^^^^^^^^^^^^^^^^^^^^^^^^//
	void setName(const char *name);
	inline const char *getName() const { return getValueSafe(); }
	inline const char *getType() const { return type; }



	//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV//
	// list properties and access rights //
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//

	//all flags for the config list
	inline UINT getFlags() const { return flags.flags; }

	//visible
	void setVisible(BOOL visible);
	inline BOOL isVisible() const { return flags.visible; }
	BOOL hasChildrenVisible(BOOL recursive=FALSE) const;

	//enabled
	void setEnabled(BOOL enabled);
	inline BOOL isEnabled() const { return flags.enabled; }
	BOOL hasChildrenEnabled(BOOL recursive=FALSE) const;

	//expanded
	void setExpanded(BOOL expanded);
	inline BOOL isExpanded() const { return flags.expanded; }
	BOOL hasChildrenExpanded(BOOL recursive=FALSE) const;

	//highlighted
	void setHighlighted(BOOL highlighted);
	inline BOOL isHighlighted() const { return flags.highlighted; }
	BOOL hasChildrenHighlighted(BOOL recursive=FALSE) const;

	//loadable/savable to persistent storage
	void setSavable(BOOL savable);
	inline BOOL isSavable() const { return flags.savable; }
	BOOL hasChildrenSavable(BOOL recursive=FALSE) const;

	//can we write userdata and params to this object
	void setReadOnly(BOOL readonly);
	inline BOOL isReadOnly() const { return flags.readonly; }
	BOOL hasChildrenReadOnly(BOOL recursive=FALSE) const;

	//is this object initialized
	inline BOOL isInited() const { return flags.inited; }


	//all security access at once
	inline unsigned char getSecurity()   const { return flags.security; }
	inline unsigned char getSecurityEx() const { return flags.external; }




	//VVVVVVVVVVVVVVVV//
	// list paramater //
	//^^^^^^^^^^^^^^^^//

	//This is the param object for this list itself
	inline prmBase *getParam() { return this; }

	//basic get/set param info
	const char *getParam(      const char *name) const;
	const char *getParamSafe(  const char *name, const char *safe="") const;
	int         getParamInt(   const char *name) const;
	RMXLONG     getParamLong(  const char *name) const;
	double      getParamNumber(const char *name) const;
	GUID        getParamGuid(  const char *name) const;

	prmBase *setParam(         const char *name, const char    *value=NULL);
	prmBase *setParam(         const char *name, const rmxData *value);
	prmBase *setParamString(   const char *name, const char    *value);
	prmBase *setParamInt(      const char *name, int            value);
	prmBase *setParamLong(     const char *name, RMXLONG        value);
	prmBase *setParamNumber(   const char *name, double         value);
	prmBase *setParamGuid(     const char *name, GUID           value);
	prmBase *setParamValue(    const char *string, char separator='=');
	void     setParams(        HPARAMLIST  paramlist);
	inline prmBase *setParamNull(const char *name) { return setParam(name, (char*)NULL); }

	//do we have a param?
	inline BOOL hasParam(const char *name)     const { return !!getParamData(name);                        }
	inline BOOL hasParam(const prmBase *param) const { return params.hasItem(const_cast<prmBase*>(param)); }
	inline BOOL hasParams()                    const { return params.hasItems();                           }

	//what params do we have?
	inline BOOL canEnumerateParams()               const { return flags.queryparams;     }
	inline int getParamCount()                     const { return params.getItemCount(); }
	inline prmBase *getFirstParam()                const { return params.getFirstItem(); }
	inline prmBase *getLastParam()                 const { return params.getLastItem();  }
	inline prmBase *getPrevParam(const prmBase *p) const { return params.getPrevItem(const_cast<prmBase*>(p)); }
	inline prmBase *getNextParam(const prmBase *p) const { return params.getNextItem(const_cast<prmBase*>(p)); }
	prmBase *getParamData(       const char *name) const;
	prmBase *getParamById(__int64 id) const;

	__int64 getParamId(const char *name) const;
	void    setParamId(const char *name, __int64 newid);

	//what are the param's flags?
	UINT getParamFlags(   const char *name) const;
	void setParamSavable( const char *name, BOOL savable);
	BOOL isParamSavable(  const char *name) const;
	void setParamReadOnly(const char *name, BOOL readonly);
	BOOL isParamReadOnly( const char *name) const;

	inline BOOL isParamUserData(prmBase *param) const { return ((void*)param) == ((void*)this); }

	//remove a param
	void removeParam(prmBase *p);
	inline void removeParam(const char *name) { removeParam(getParamData(name)); }
	void removeAllParams();


	//VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV//
	//list hierarchy (parent info, child info, etc) //
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//

	//parent and root
	static cfgBase *getListRoot() { return rootlist; }
	inline cfgBase *getParent() const { return parent; }

	//get a child
	cfgBase *getChild(const char *name, const char *type=NULL, HPARAMLIST paramlist=NULL) const;
	cfgBase *getChildById(RMXLONG rmxid, BOOL recursive=FALSE) const;

	//do we have a specific child?
	BOOL hasChild(      const cfgBase *item, BOOL recursive=FALSE);
	BOOL hasChildCached(const cfgBase *item);  //for internal use, automatically recursive

	//do we have children at all?
	inline BOOL hasChildren()   const { return children.hasItems();     }
	inline int  getChildCount() const { return children.getItemCount(); }

	//enumerate children
	inline cfgBase *getFirstChild()                   const { return children.getFirstItem(); }
	inline cfgBase *getLastChild()                    const { return children.getLastItem();  }
	inline cfgBase *getNextChild(const cfgBase *item) const { return children.getNextItem(const_cast<cfgBase*>(item)); }
	inline cfgBase *getPrevChild(const cfgBase *item) const { return children.getPrevItem(const_cast<cfgBase*>(item)); }


	//remove children
	void removeAllChildren();

	//move list position
	void moveListUp();
	void moveListDown();
	void moveListToBegining();
	void moveListToEnd();
	void moveChild(const cfgBase *child, const cfgBase *insert);



//FOR INTERNAL USE ONLY
//do !!NOT!! mess with any function after this point directly, unless you know what you are doing
public:
	inline void setSecurityEx(unsigned char ex) { flags.external = ex; }
	inline HLIST getListHandle() const { return (HLIST) this; }

private:
	inline void insertChild(cfgBase *child, cfgBase *insert=NULL) { children.insertItem(child, insert); }
	void removeChild(cfgBase *child);

	inline void setSecurity(unsigned char sec) { flags.security = sec; }

	inline void insertCallback(cbBase *cb) { callbacks.appendItem(cb); }
	inline void removeCallback(cbBase *cb) { callbacks.removeItem(cb); }


private:
	cfgBase *parent;

	__int64 id;

	//TODO: change to a new custom type, so that way things like "tree" dont need to malloc new RAM
	rmxString type;

	rmxList<cfgBase*> children;
	rmxList<prmBase*> params;
	rmxList<cbBase*>  callbacks;

private:
	static cfgBase *rootlist;
};


#endif //_CFG_BASE_H_
