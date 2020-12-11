/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RMX_GUID_H_
#define _RMX_GUID_H_


#include "guiddef.h"
#include "rmxBase.h"


extern const GUID _INVALID_GUID;


class rmxGuid {
protected:
	rmxGuid () {}

public:
	static char *guidToChar(GUID g, char *str);
	static GUID guidFromChar(const char *str);

	static GUID guidrand();

	static int guidcmp(GUID guid1, GUID guid2);

	static bool isValidGuid(GUID g);
	static bool isValidGuid(const char *guid, VINT len=-1);
	static bool isValidGuidString(const char *guid, VINT len=-1);
};


#endif _RMX_GUID_H_
