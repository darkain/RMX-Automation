/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#define _CRT_RAND_S  //SEE: http://msdn2.microsoft.com/en-us/library/sxtz2fa8(VS.80).aspx


#include <sys/stat.h>
#include "rmxBase.h"
#include "Win32/Win32.h"


/*
this is just a temporary fix until i get all the needed libs for VS7
*/
#if (_MSC_VER >= 1300) && (_MSC_VER < 1400)
//VC7 or later, building with pre-VC7 runtime libraries
extern "C" long _ftol( double ); //defined by VC6 C libs
extern "C" long _ftol2( double dblSource ) { return _ftol( dblSource ); }
#endif


BOOL VFEXISTS(const char *path) {
  struct _stat buffer;
  return ( _stat(path, &buffer) == 0 );
}


VINT VFSIZE(const char *path) {
  struct _stat buffer;
  if (_stat(path, &buffer) == 0) return buffer.st_size;
  return 0;
}


int RAND() {
#if _MSC_VER >= 1400
  if (RMXOS::isWinXP()) {
    unsigned int ret = 0;
    if (rand_s(&ret) == 0) return (int)ret;
  }
#endif

  return ( (rand() & 0xFFFF)  |  ((rand() & 0xFFFF) << 16) );
}


#if _MSC_VER < 1400
int VFOPEN(FILE **file, const char *path, const char *open) {
  FILE *f = fopen(path, open);
  *file = f;
  if (!f) return GetLastError();
  return 0;
}
#endif
