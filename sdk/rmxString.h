/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RMX_STRING_H_
#define _RMX_STRING_H_


#include "rmxData.h"
#include "../libraries/bstring/bstrwrap.h"


class rmxString : public CBString {
public:
	rmxString()                       : CBString(      ) {}
	rmxString(const char          *s) : CBString(     s) {}
	rmxString(int len, const char *s) : CBString(len, s) {}
	rmxString(const rmxData       *s) : CBString(s->getValueSafe())            {}
	rmxString(const wchar_t       *s) : CBString(rmxToUtf8(s).getBufferSafe()) {}

	virtual ~rmxString() {}

public:
	inline const char *get() const { return *this; }

	using CBString::operator  =;
	using CBString::operator +=;
	using CBString::operator  +;
	using CBString::operator ==;
	using CBString::operator !=;
	using CBString::operator  <;
	using CBString::operator <=;
	using CBString::operator  >;
	using CBString::operator >=;

	const rmxString& operator  = (const rmxData *data);
	const rmxString& operator  = (int            data);
	const rmxString& operator  = (__int64        data);
	const rmxString& operator  = (double         data);
	const rmxString& operator  = (GUID           data);
	const rmxString& operator  = (FOURCC         data);

	const rmxString& operator += (const rmxData *data);
	const rmxString& operator += (int            data);
	const rmxString& operator += (__int64        data);
	const rmxString& operator += (double         data);
	const rmxString& operator += (GUID           data);
	const rmxString& operator += (FOURCC         data);

	const rmxString  operator  + (const rmxData *data) const;
	const rmxString  operator  + (int            data) const;
	const rmxString  operator  + (__int64        data) const;
	const rmxString  operator  + (double         data) const;
	const rmxString  operator  + (GUID           data) const;
	const rmxString  operator  + (FOURCC         data) const;

	bool operator == (const rmxData *data) const;
	bool operator == (int            data) const;
	bool operator == (__int64        data) const;
	bool operator == (double         data) const;
	bool operator == (GUID           data) const;
	bool operator == (FOURCC         data) const;

	bool operator != (const rmxData *data) const;
	bool operator != (int            data) const;
	bool operator != (__int64        data) const;
	bool operator != (double         data) const;
	bool operator != (GUID           data) const;
	bool operator != (FOURCC         data) const;

	bool operator  < (const rmxData *data) const;
	bool operator  < (int            data) const;
	bool operator  < (__int64        data) const;
	bool operator  < (double         data) const;
	bool operator  < (GUID           data) const;
	bool operator  < (FOURCC         data) const;

	bool operator <= (const rmxData *data) const;
	bool operator <= (int            data) const;
	bool operator <= (__int64        data) const;
	bool operator <= (double         data) const;
	bool operator <= (GUID           data) const;
	bool operator <= (FOURCC         data) const;

	bool operator  > (const rmxData *data) const;
	bool operator  > (int            data) const;
	bool operator  > (__int64        data) const;
	bool operator  > (double         data) const;
	bool operator  > (GUID           data) const;
	bool operator  > (FOURCC         data) const;

	bool operator >= (const rmxData *data) const;
	bool operator >= (int            data) const;
	bool operator >= (__int64        data) const;
	bool operator >= (double         data) const;
	bool operator >= (GUID           data) const;
	bool operator >= (FOURCC         data) const;
};


#endif //_RMX_STRING_H_
