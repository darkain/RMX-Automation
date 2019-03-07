/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "rmxGuid.h"



const GUID _INVALID_GUID = { 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0} };


char *rmxGuid::guidToChar(GUID guid, char *str) {
	if (str == NULL) return NULL;

	//TODO: hmm.... what to do for this one? We need a better GUID verification
	//currently using sizeof a guid string - 39 characters.  (38 + NULL)
	//i should force the caller to pass in a buffer size, and error if too small
	VPRINTF(str, 39, "{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		(int)guid.Data1,    (int)guid.Data2,    (int)guid.Data3,
		(int)guid.Data4[0], (int)guid.Data4[1], 
		(int)guid.Data4[2], (int)guid.Data4[3], 
		(int)guid.Data4[4], (int)guid.Data4[5], 
		(int)guid.Data4[6], (int)guid.Data4[7]);

	return str;
}


GUID rmxGuid::guidFromChar(const char *str) {
	if (!str) return _INVALID_GUID;
	if (VSTRLEN(str) < 10) return _INVALID_GUID;

	GUID guid;
	int Data1, Data2, Data3;
	int Data4[8];

	VSCANF(str, " { %08x - %04x - %04x - %02x%02x - %02x%02x%02x%02x%02x%02x } ",
		&Data1, &Data2, &Data3, Data4 + 0, Data4 + 1, 
		Data4 + 2, Data4 + 3, Data4 + 4, Data4 + 5, Data4 + 6, Data4 + 7 );

	// Cross assign all the values
	guid.Data1    = Data1;
	guid.Data2    = Data2;
	guid.Data3    = Data3;
	guid.Data4[0] = Data4[0];
	guid.Data4[1] = Data4[1];
	guid.Data4[2] = Data4[2];
	guid.Data4[3] = Data4[3];
	guid.Data4[4] = Data4[4];
	guid.Data4[5] = Data4[5];
	guid.Data4[6] = Data4[6];
	guid.Data4[7] = Data4[7];

	return guid;
}


int rmxGuid::guidcmp(GUID a, GUID b) {
	int delta = 0;
	
	delta = a.Data1 - b.Data1;
	if (delta) return delta;
	
	delta = a.Data2 - b.Data2;
	if (delta) return delta;
	
	delta = a.Data3 - b.Data3;
	if (delta) return delta;
	
	for (int i=0; i<8; i++) {
		delta = a.Data4[i] - b.Data4[i];
		if (delta) return delta;
	}

	return delta;
}


GUID rmxGuid::guidrand() {
	GUID g;
	g.Data1 = RAND();
	g.Data2 = RAND() & 0xFFFF;
	g.Data3 = RAND() & 0xFFFF;

	for (int i=0; i<8; i++) {
		g.Data4[i] = RAND() & 0xFF;
	}

	return g;
}


bool rmxGuid::isValidGuid(GUID g) {
	return (guidcmp(_INVALID_GUID, g) != 0);
}


bool rmxGuid::isValidGuid(const char *guid, VINT len) {
	if (!guid) return false;
	if (len == -1) len = VSTRLEN(guid);

	if (!isValidGuidString(guid, len)) return false;

	return isValidGuid(guidFromChar(guid));
}


bool rmxGuid::isValidGuidString(const char *guid, VINT len) {
	if (!guid) return false;
	if (len == -1) len = VSTRLEN(guid);
	if (len < 38) return false;

	if ( (guid[0] == '{') & (guid[37] == '}') ) {
		for (int i=1; i<37; i++) {
			if ( (i == 9) || (i == 14) || (i == 19) || (i == 24) ) {
				if (guid[i] != '-') {
					return false;
				}
			} else {
				if (!( ((guid[i]>='0')&(guid[i]<='9')) | ((guid[i]>='a')&(guid[i]<='f')) | ((guid[i]>='A')&(guid[i]<='F')) )) {
					return false;
				}
			}
		}
		return true;
	}

	return false;
}
