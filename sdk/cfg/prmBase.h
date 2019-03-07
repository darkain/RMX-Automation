/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _PRM_BASE_H_
#define _PRM_BASE_H_


#include "../rmxData.h"
#include "../rmxString.h"


class prmBase : public rmxData, public rmxString {
	friend class cfgBase;

public:
	inline prmBase(const char *name=NULL) :               rmxData(),      rmxString(name) { item=NULL; id=0; }
	inline prmBase(const char *name, const char *value) : rmxData(value), rmxString(name) { item=NULL; id=0; }
	inline prmBase(const char *name, int         value) : rmxData(value), rmxString(name) { item=NULL; id=0; }
	inline prmBase(const char *name, RMXLONG     value) : rmxData(value), rmxString(name) { item=NULL; id=0; }
	inline prmBase(const char *name, double      value) : rmxData(value), rmxString(name) { item=NULL; id=0; }
	inline prmBase(const char *name, GUID        value) : rmxData(value), rmxString(name) { item=NULL; id=0; }
	inline prmBase(const char *name, FOURCC      value) : rmxData(value), rmxString(name) { item=NULL; id=0; }
	virtual ~prmBase() {}

public:
	inline __int64 getId() const        { return id;  }
	inline void    setId(__int64 newid) { id = newid; }

	inline const char *getName()          const { return *this; }
	inline bool isNamed(const char *name) const { return *this == name; }

protected:
	virtual void onDataUpdated();
	void setParamConfig(cfgBase *config);

private:
	cfgBase *item;
	__int64 id;
};


#endif //_PRM_BASE_H_
