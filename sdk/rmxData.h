/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RMX_DATA_H_
#define _RMX_DATA_H_


#include "rmxTypes.h"
#include "rmxGuid.h"
#include "rmxLock.h"


typedef enum {
	RDT_NULL,         //NULL data
	RDT_SHORTSTRING,  //string that uses local buffer
	RDT_STRING,       //string that mallocs a buffer
	RDT_INT,          //32-bit integer
	RDT_LONG,         //64-bit integer
	RDT_NUMBER,       //64-bit float
	RDT_GUID,         //128-bit GUID
	RDT_FOURCC,       //32-bit four character code
	RDT_IPV4,         //IPv4 address (current unused)
	RDT_IPV6,         //IPv6 address (current unused)
} rmxDataType;


class cfgBase;
class prmBase;

class rmxData {
	friend class cfgBase;
	friend class prmBase;


public:
	rmxData();
	rmxData(const char *value, bool trim=true);
	rmxData(int         value);
	rmxData(__int64     value);
	rmxData(double      value);
	rmxData(GUID        value);
	rmxData(FOURCC      value);
	virtual ~rmxData();

private:
	void dispatchDataUpdate();

	inline void rmxDataLock()   { datalock.lock();   }
	inline void rmxDataUnlock() { datalock.unlock(); }


protected:
	virtual void onDataUpdated() {}

public:
	rmxData *clone() const;
	rmxData *clone(rmxData *dest) const;

	const char   *getValue() const;
	//    const wchar_t getValueWide() const;      //TODO: implement this
	//    const wchar_t getValueWideSafe() const;  //TODO: implement this
	int           getValueInt() const;
	__int64       getValueLong() const;
	double        getValueNumber() const;
	GUID          getValueGuid() const;
	FOURCC        getValueFourCC() const;
	bool          isValued(const char *value) const;


	inline const char *getValueSafe(const char *safe="") const {
		const char *value = getValue();
		return (value ? value : safe);
	}


	inline void setValue(const rmxData *source) { source->clone(this); }
	void setValue(const char *newvalue, bool trim=true);
	void setValue(const wchar_t *newvalue, bool trim=true);
	void setValueString(const char *newvalue, VINT len=-1);
	void setValueWide(const wchar_t *newvalue);
	void setValueInt(int newvalue);
	void setValueLong(__int64 newvalue);
	void setValueNumber(double newvalue);
	void setValueGuid(GUID newvalue);
	void setValueFourCC(FOURCC newval);
	void setValueNull();

	inline rmxDataType getType() const { return (rmxDataType) flags.datatype; }

	inline bool isNull()     const { return (flags.datatype == RDT_NULL  ); }
	inline bool isString()   const { return (flags.datatype == RDT_STRING  ||  flags.datatype == RDT_SHORTSTRING); }
	inline bool isInteger()  const { return (flags.datatype == RDT_INT   ); }
	inline bool isLong()     const { return (flags.datatype == RDT_LONG  ); }
	inline bool isNumber()   const { return (flags.datatype == RDT_NUMBER); }
	inline bool isGuid()     const { return (flags.datatype == RDT_GUID  ); }
	inline bool isFourCC()   const { return (flags.datatype == RDT_FOURCC); }

	inline bool isSavable()  const { return flags.savable;  }
	inline void setSavable(bool savable) { flags.savable = !!savable; }

	inline bool isReadOnly() const { return flags.readonly; }
	inline void setReadOnly(bool readonly) { flags.readonly = !!readonly; }

	inline UINT getFlags()   const { return flags.flags;    }
	inline unsigned char getSecurity()   const { return flags.security; }
	inline unsigned char getSecurityEx() const { return flags.external; }



	//this is for internal use by RMX, try not to mess with it
	inline void _setSecurityEx(unsigned char ex) { flags.external = ex; }


private:  //for now
	inline void setFlags(UINT flgs)            { flags.flags    = flgs; }
	inline void setSecurity(unsigned char sec) { flags.security = sec;  }

	inline void _clear_data() {
		if (flags.datatype == RDT_STRING) free(value.string1);
		value._fill = value.longint = 0;
		flags.datatype = RDT_NULL;
	}

public:
	static int specialString(const char *string);


private:
	typedef union {
		UINT flags;
		struct {
			//data type - used by rmxData
			unsigned datatype      :8;

			//configuration information - used by cfgBase
			unsigned visible       :1;  //is this object visible to the GUI?
			unsigned enabled       :1;  //is this object enabled for user input?
			unsigned expanded      :1;  //is this object currently expanded to show children?
			unsigned highlighted   :1;  //is this object currently highlighted?
			unsigned expanded_ro   :1;  //is this object's expanded attrib locked from user input?
			unsigned queryparams   :1;  //can we enumerate params of this list?
			unsigned inited        :1;  //is this object initialized?
			unsigned reserved0     :1;  //RESERVED

			//8-bits of reserved data
			unsigned reserved1     :8;

			//access restrictions, used by cfgBase and RMX Core
			unsigned savable       :1;
			unsigned readonly      :1;
			unsigned external      :6;
		};
		struct {
			unsigned char reserved_ex0;
			unsigned char settings;
			unsigned char reserved_ex1;
			unsigned char security;
		};
	} FLAGS;


	typedef union {
		int      integer;
		double   number;
		char    *string1;
		char     string2[16];
		GUID     guid;
		FOURCC   fourcc;

		struct {
			__int64  longint;
			__int64  _fill;
		};
	} VALUE;


private:
	FLAGS   flags;
	VALUE   value;
	rmxLock datalock;

private:
	THREAD_LOCAL static char buffer[64];
};


#endif // _RMX_DATA_H_
