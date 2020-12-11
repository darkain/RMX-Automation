/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../../sdk/rmxBase.h"
#include "../../../sdk/win32/win32.h"
#include "base.h"



httpBase::httpBase(JNL_HTTPServ *httpserver) : server(httpserver) {
	position = 0;
}


httpBase::~httpBase() {
}


void httpBase::setBuffer(const char *data) {
	position = 0;
	*this = data;
}


int httpBase::GetData(char *buffer, int size) {
	if (size > length() - position) size = length() - position;
	const char *data = *this;
	memcpy(buffer, data+position, size);
	position += size;
	return size;
}
