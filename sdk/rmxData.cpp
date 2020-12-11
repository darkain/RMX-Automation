/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/



#include "rmxBase.h"
#include "rmxData.h"


char rmxData::buffer[64] = "";


rmxData::rmxData() {
	datalock.lock();
	flags.flags = 0;
	_clear_data();
}


rmxData::rmxData(const char *value, bool trim) {
	datalock.lock();
	flags.flags = 0;
	_clear_data();
	setValue(value, trim);
}


rmxData::rmxData(int value) {
	datalock.lock();
	flags.flags = 0;
	_clear_data();
	setValueInt(value);
}


rmxData::rmxData(__int64 value) {
	datalock.lock();
	flags.flags = 0;
	_clear_data();
	setValueLong(value);
}


rmxData::rmxData(double value) {
	datalock.lock();
	flags.flags = 0;
	_clear_data();
	setValueNumber(value);
}


rmxData::rmxData(GUID value) {
	datalock.lock();
	flags.flags = 0;
	_clear_data();
	setValueGuid(value);
}


rmxData::rmxData(FOURCC value) {
	datalock.lock();
	flags.flags = 0;
	_clear_data();
	setValueFourCC(value);
}


rmxData::~rmxData() {
	datalock.lock();
	_clear_data();
}



void rmxData::dispatchDataUpdate() {
	if (datalock.isLocked()) return;
	datalock.lock();
	onDataUpdated();
	datalock.unlock();
}




rmxData *rmxData::clone() const {
	switch (flags.datatype) {
		case RDT_NULL:        return new rmxData();
		case RDT_SHORTSTRING: return new rmxData(value.string2, FALSE);
		case RDT_STRING:      return new rmxData(value.string1, FALSE);
		case RDT_INT:         return new rmxData(value.integer);
		case RDT_LONG:        return new rmxData(value.longint);
		case RDT_NUMBER:      return new rmxData(value.number);
		case RDT_GUID:        return new rmxData(value.guid);
		case RDT_FOURCC:      return new rmxData(value.fourcc);
	}
	return new rmxData(getValue());
}


rmxData *rmxData::clone(rmxData *dest) const {
	if (!dest) return NULL;

	switch (flags.datatype) {
		case RDT_NULL:        dest->setValueNull();                 break;
		case RDT_SHORTSTRING: dest->setValueString(value.string2);  break;
		case RDT_STRING:      dest->setValueString(value.string1);  break;
		case RDT_INT:         dest->setValueInt(   value.integer);  break;
		case RDT_LONG:        dest->setValueLong(  value.longint);  break;
		case RDT_NUMBER:      dest->setValueNumber(value.number );  break;
		case RDT_GUID:        dest->setValueGuid(  value.guid   );  break;
		case RDT_FOURCC:      dest->setValueFourCC(value.fourcc );  break;
		default:              dest->setValue(      getValue()   );  break;
	}

	return dest;
}




const char *rmxData::getValue() const {
	switch (flags.datatype) {
		case RDT_NULL:        return NULL;
		case RDT_SHORTSTRING: return value.string2;
		case RDT_FOURCC:      return value.string2;  //when setting value as 4CC, we NULL the 5th character
		case RDT_STRING:      return value.string1;

		case RDT_INT:
			VPRINTF(buffer, sizeof(buffer), "%d", value.integer);
			return buffer;

		case RDT_LONG:
			VPRINTF(buffer, sizeof(buffer), "%I64d", value.longint);
			return buffer;

		case RDT_NUMBER:
			VPRINTF(buffer, sizeof(buffer), "%f", value.number);
			return buffer;

		case RDT_GUID: return rmxGuid::guidToChar(value.guid, buffer);
	}
	return NULL;
}


int rmxData::getValueInt() const {
	switch (flags.datatype) {
		case RDT_NULL:   return 0;
		case RDT_INT:    return value.integer;
		case RDT_FOURCC: return value.integer;
		case RDT_LONG:   return static_cast<int>(value.longint);
		case RDT_NUMBER: return static_cast<int>(value.number);

		case RDT_SHORTSTRING: {
			int ret = 0;
			VSCANF(value.string2, "%d", &ret);
			return ret;
		}

		case RDT_STRING: {
			int ret = 0;
			if (value.string1) VSCANF(value.string1, "%d", &ret);
			return ret;
		}
	}
	return 0;
}


__int64 rmxData::getValueLong() const {
	switch (flags.datatype) {
		case RDT_NULL:   return 0;
		case RDT_LONG:   return value.longint;
		case RDT_INT:    return value.integer;
		case RDT_FOURCC: return value.integer;
		case RDT_NUMBER: return static_cast<__int64>(value.number);

		case RDT_SHORTSTRING: {
			__int64 ret = 0;
			VSCANF(value.string2, "%I64d", &ret);
			return ret;
		}

		case RDT_STRING: {
			__int64 ret = 0;
			if (value.string1) VSCANF(value.string1, "%I64d", &ret);
			return ret;
		}
	}
	return 0;
}


double rmxData::getValueNumber() const {
	switch (flags.datatype) {
		case RDT_NULL:   return 0;
		case RDT_NUMBER: return value.number;
		case RDT_INT:    return static_cast<double>(value.integer);
		case RDT_FOURCC: return static_cast<double>(value.integer);
		case RDT_LONG:   return static_cast<double>(value.longint);

		case RDT_SHORTSTRING: {
			char *extra = NULL;
			return strtod(value.string2, &extra);
		}

		case RDT_STRING: {
			char *extra = NULL;
			return strtod(value.string1, &extra);
		}
	}
	return 0;
}


GUID rmxData::getValueGuid() const {
	if (flags.datatype == RDT_GUID) return value.guid;
	return _INVALID_GUID;
}


FOURCC rmxData::getValueFourCC() const {
	switch (flags.datatype) {
		case RDT_NULL:   return 0;
		case RDT_INT:    return value.fourcc;
		case RDT_FOURCC: return value.fourcc;
		case RDT_LONG:   return static_cast<FOURCC>(value.longint);
		case RDT_NUMBER: return static_cast<FOURCC>(value.number);
		case RDT_SHORTSTRING: return value.fourcc;

		case RDT_STRING: {
			FOURCC ret = 0;
			if (value.string1) VSCANF(value.string1, "%4c", &ret);
			return ret;
		}
	}
	return 0;
}


bool rmxData::isValued(const char *value) const {
	const char *v = getValue();
	if (value == NULL  &&  v == NULL) return true;
	if (value == NULL  ||  v == NULL) return false;
	return (VSTRCMP(value, v) == 0);
}


void rmxData::setValue(const char *newvalue, bool trim) {
	if (isReadOnly()) return;

	if (!newvalue) {
		_clear_data();
		dispatchDataUpdate();
		return;
	}


	VINT len = VSTRLEN(newvalue);


	if (len < 1) {
		_clear_data();
		flags.datatype = RDT_SHORTSTRING;
		dispatchDataUpdate();
		return;
	}


	if (trim) {
		while (*newvalue) {
			if ( *newvalue ==  ' '  ||  *newvalue == '\r'
			  || *newvalue == '\n'  ||  *newvalue == '\t' ) {
				len--;
				newvalue++;
			} else {
				break;
			}
		}
	} else {
		if ( *newvalue ==  ' '  ||  *newvalue == '\r'
		  || *newvalue == '\n'  ||  *newvalue == '\t'
		  || *newvalue == NULL  ) {
			setValueString(newvalue);
			return;
		}
	}

	if (*newvalue == NULL) {
		_clear_data();
		flags.datatype = RDT_SHORTSTRING;
		dispatchDataUpdate();
		return;
	}

	if ( (len == 38) && (rmxGuid::isValidGuidString(newvalue, len)) ) {
		setValueGuid(rmxGuid::guidFromChar(newvalue));
		return;
	}

	int special = specialString(newvalue);
	if (special == 0  ||  special == 1) {
		setValueInt(special);
		return;
	}


	if (*newvalue != '0') {

		char *extra = NULL;
		double numeric = strtod(newvalue, &extra);
		if (numeric != 0) {
			char *p = extra;
			bool ok = true;
			if (p) while (*p) {
				if (*p == ' '  ||  *p == '\t'  ||  *p == '\r'  ||  *p == '\n') {
				} else {
					ok = false;
					break;
				}
				p++;
			}

			if (ok) {
				__int64 lng = static_cast<__int64>(numeric);
				if (static_cast<double>(lng) - numeric == 0) {
					if ( (lng <= ((__int64)(long)0x7fffffff))  &&  (lng >= ((__int64)(long)0x80000000)) ) {
						setValueInt((int)lng);
					} else {
						setValueLong(lng);
					}
				} else {
					setValueNumber(numeric);
				}
				return;
			}
		}

	}

	setValueString(newvalue, len);
}


void rmxData::setValue(const wchar_t *newvalue, bool trim) {
	if (isReadOnly()) return;

	rmxToUtf8 str(newvalue);
	setValue(str.getBuffer(), trim);
}


void rmxData::setValueString(const char *newvalue, VINT len) {
	if (isReadOnly()) return;

	if (!newvalue) {
		_clear_data();
		dispatchDataUpdate();
		return;
	}

	if (len == -1) len = VSTRLEN(newvalue);

	if (len < sizeof(value.string2)) {
		_clear_data();
		memcpy(value.string2, newvalue, len);
		value.string2[len] = NULL;
		flags.datatype = RDT_SHORTSTRING;
	}

	else if (flags.datatype == RDT_STRING) {
		value.string1 = (char*)realloc(value.string1, len+1);
		memcpy(value.string1, newvalue, len);
		value.string1[len] = NULL;
	}

	else {
		_clear_data();
		value.string1 = (char*) malloc(len+1);
		memcpy(value.string1, newvalue, len);
		value.string1[len] = NULL;
		flags.datatype = RDT_STRING;
	}

	dispatchDataUpdate();
}


void rmxData::setValueWide(const wchar_t *newvalue) {
	rmxToUtf8 str(newvalue);
	setValueString(str.getBuffer());
}


void rmxData::setValueInt(int newvalue) {
	if (isReadOnly()) return;
	if (flags.datatype == RDT_INT  &&  value.integer == newvalue) return;

	_clear_data();
	flags.datatype = RDT_INT;
	value.integer  = newvalue;

	dispatchDataUpdate();
}


void rmxData::setValueLong(__int64 newvalue) {
	if (isReadOnly()) return;
	if (flags.datatype == RDT_LONG  &&  value.longint == newvalue) return;

	_clear_data();
	flags.datatype = RDT_LONG;
	value.longint  = newvalue;

	dispatchDataUpdate();
}


void rmxData::setValueNumber(double newvalue) {
	if (isReadOnly()) return;
	if (flags.datatype == RDT_NUMBER  &&  value.number == newvalue) return;

	_clear_data();
	flags.datatype = RDT_NUMBER;
	value.number   = newvalue;

	dispatchDataUpdate();
}

void rmxData::setValueGuid(GUID newvalue) {
	if (isReadOnly()) return;

	_clear_data();
	flags.datatype = RDT_GUID;
	value.guid     = newvalue;

	dispatchDataUpdate();
}

void rmxData::setValueFourCC(FOURCC newvalue) {
	if (isReadOnly()) return;
	if (flags.datatype == RDT_FOURCC  &&  value.fourcc == newvalue) return;

	_clear_data();
	flags.datatype = RDT_FOURCC;
	value.fourcc   = newvalue;

	dispatchDataUpdate();
}


void rmxData::setValueNull() {
	if (isReadOnly()) return;
	_clear_data();
	dispatchDataUpdate();
}


int rmxData::specialString(const char *string) {
	if (!string  ||  !*string) return -2;

	if (VSTRCMPI(string, "1")        == 0) return  1;
	if (VSTRCMPI(string, "ok")       == 0) return  1;
	if (VSTRCMPI(string, "yes")      == 0) return  1;
	if (VSTRCMPI(string, "true")     == 0) return  1;
	if (VSTRCMPI(string, "enable")   == 0) return  1;
	if (VSTRCMPI(string, "enabled")  == 0) return  1;
	if (VSTRCMPI(string, "accept")   == 0) return  1;

	if (VSTRCMPI(string, "0")        == 0) return  0;
	if (VSTRCMPI(string, "nil")      == 0) return  0;
	if (VSTRCMPI(string, "null")     == 0) return  0;
	if (VSTRCMPI(string, "no")       == 0) return  0;
	if (VSTRCMPI(string, "false")    == 0) return  0;
	if (VSTRCMPI(string, "disable")  == 0) return  0;
	if (VSTRCMPI(string, "disabled") == 0) return  0;
	if (VSTRCMPI(string, "cancel")   == 0) return  0;

	if (VSTRCMPI(string, "-1")       == 0) return -1;
	if (VSTRCMPI(string, "toggle")   == 0) return -1;
	if (VSTRCMPI(string, "invert")   == 0) return -1;
	if (VSTRCMPI(string, "inverse")  == 0) return -1;
	if (VSTRCMPI(string, "not")      == 0) return -1;

	return -2;
}
