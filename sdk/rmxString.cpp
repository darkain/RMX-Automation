/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "rmxString.h"



const rmxString& rmxString::operator = (const rmxData *data) {
	*this = data->getValue();
	return *this;
}

const rmxString& rmxString::operator = (int data) {
	*this = rmxData(data).getValue();
	return *this;
}

const rmxString& rmxString::operator = (__int64 data) {
	*this = rmxData(data).getValue();
	return *this;
}

const rmxString& rmxString::operator = (double data) {
	*this = rmxData(data).getValue();
	return *this;
}

const rmxString& rmxString::operator = (GUID data) {
	*this = rmxData(data).getValue();
	return *this;
}

const rmxString& rmxString::operator = (FOURCC data) {
	*this = rmxData(data).getValue();
	return *this;
}


////////////////////////////////////////////////////////////////


const rmxString& rmxString::operator += (const rmxData *data) {
	*this += data->getValue();
	return *this;
}

const rmxString& rmxString::operator += (int data) {
	*this += rmxData(data).getValue();
	return *this;
}

const rmxString& rmxString::operator += (__int64 data) {
	*this += rmxData(data).getValue();
	return *this;
}

const rmxString& rmxString::operator += (double data) {
	*this += rmxData(data).getValue();
	return *this;
}

const rmxString& rmxString::operator += (GUID data) {
	*this += rmxData(data).getValue();
	return *this;
}

const rmxString& rmxString::operator += (FOURCC data) {
	*this += rmxData(data).getValue();
	return *this;
}


////////////////////////////////////////////////////////////////


const rmxString rmxString::operator + (const rmxData *data) const {
	rmxString retval (*this);
	return retval += data;
}

const rmxString rmxString::operator + (int data) const {
	rmxString retval (*this);
	return retval += data;
}

const rmxString rmxString::operator + (__int64 data) const {
	rmxString retval (*this);
	return retval += data;
}

const rmxString rmxString::operator + (double data) const {
	rmxString retval (*this);
	return retval += data;
}

const rmxString rmxString::operator + (GUID data) const {
	rmxString retval (*this);
	return retval += data;
}

const rmxString rmxString::operator + (FOURCC data) const {
	rmxString retval (*this);
	return retval += data;
}




////////////////////////////////////////////////////////////////


bool rmxString::operator == (const rmxData *data) const {
	return (*this) == data->getValue();
}


bool rmxString::operator == (const char *data) const {
	return (*this) == data;
}

bool rmxString::operator == (int data) const {
	return (*this) == rmxData(data).getValue();
}

bool rmxString::operator == (__int64 data) const {
	return (*this) == rmxData(data).getValue();
}

bool rmxString::operator == (double data) const {
	return (*this) == rmxData(data).getValue();
}

bool rmxString::operator == (GUID data) const {
	return (*this) == rmxData(data).getValue();;
}

bool rmxString::operator == (FOURCC data) const {
	return (*this) == rmxData(data).getValue();
}



////////////////////////////////////////////////////////////////


bool rmxString::operator != (const rmxData *data) const {
	return (*this) != data->getValue();
}

bool rmxString::operator != (const char *data) const {
	return (*this) != data;
}

bool rmxString::operator != (int data) const {
	return (*this) != rmxData(data).getValue();
}

bool rmxString::operator != (__int64 data) const {
	return (*this) != rmxData(data).getValue();
}

bool rmxString::operator != (double data) const {
	return (*this) != rmxData(data).getValue();
}

bool rmxString::operator != (GUID data) const {
	return (*this) != rmxData(data).getValue();
}

bool rmxString::operator != (FOURCC data) const {
	return (*this) != rmxData(data).getValue();
}


////////////////////////////////////////////////////////////////


bool rmxString::operator < (const rmxData *data) const {
	return (*this) < data->getValue();
}

bool rmxString::operator < (int data) const {
	return rmxData((const char*)(*this)).getValueInt() < data;
}

bool rmxString::operator < (__int64 data) const {
	return rmxData((const char*)(*this)).getValueLong() < data;
}

bool rmxString::operator < (double data) const {
	return rmxData((const char*)(*this)).getValueNumber() < data;
}

bool rmxString::operator < (GUID data) const {
	return rmxGuid::guidcmp( rmxData((const char*)(*this)).getValueGuid(), data ) < 0;
}

bool rmxString::operator < (FOURCC data) const {
	return rmxData((const char*)(*this)).getValueFourCC() < data;
}


////////////////////////////////////////////////////////////////


bool rmxString::operator <= (const rmxData *data) const {
	return (*this) <= data->getValue();
}

bool rmxString::operator <= (int data) const {
	return rmxData((const char*)(*this)).getValueInt() <= data;
}

bool rmxString::operator <= (__int64 data) const {
	return rmxData((const char*)(*this)).getValueLong() <= data;
}

bool rmxString::operator <= (double data) const {
	return rmxData((const char*)(*this)).getValueNumber() <= data;
}

bool rmxString::operator <= (GUID data) const {
	return rmxGuid::guidcmp( rmxData((const char*)(*this)).getValueGuid(), data ) <= 0;
}

bool rmxString::operator <= (FOURCC data) const {
	return rmxData((const char*)(*this)).getValueFourCC() <= data;
}


////////////////////////////////////////////////////////////////


bool rmxString::operator > (const rmxData *data) const {
	return (*this) > data->getValue();
}

bool rmxString::operator > (int data) const {
	return rmxData((const char*)(*this)).getValueInt() > data;
}

bool rmxString::operator > (__int64 data) const {
	return rmxData((const char*)(*this)).getValueLong() > data;
}

bool rmxString::operator > (double data) const {
	return rmxData((const char*)(*this)).getValueNumber() > data;
}

bool rmxString::operator > (GUID data) const {
	return rmxGuid::guidcmp( rmxData((const char*)(*this)).getValueGuid(), data ) > 0;
}

bool rmxString::operator > (FOURCC data) const {
	return rmxData((const char*)(*this)).getValueFourCC() > data;
}

////////////////////////////////////////////////////////////////


bool rmxString::operator >= (const rmxData *data) const {
	return (*this) >= data->getValue();
}

bool rmxString::operator >= (int data) const {
	return rmxData((const char*)(*this)).getValueInt() >= data;
}

bool rmxString::operator >= (__int64 data) const {
	return rmxData((const char*)(*this)).getValueLong() >= data;
}

bool rmxString::operator >= (double data) const {
	return rmxData((const char*)(*this)).getValueNumber() >= data;
}

bool rmxString::operator >= (GUID data) const {
	return rmxGuid::guidcmp( rmxData((const char*)(*this)).getValueGuid(), data ) >= 0;
}

bool rmxString::operator >= (FOURCC data) const {
	return rmxData((const char*)(*this)).getValueFourCC() >= data;
}
