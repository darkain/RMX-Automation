/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RMX_UNICODE_H_
#define _RMX_UNICODE_H_


class rmxUtf8ToWide {
  public:
    rmxUtf8ToWide(const char *string, int length=-1);
    ~rmxUtf8ToWide();

  public:
    inline const wchar_t *getBuffer() { return buffer; }
    inline int getLength() { return len; }

  private:
    wchar_t *buffer;
    int len;
};


//-----------------------------------------------------------------------------------------------------------


class rmxToUtf8 {
  public:
    rmxToUtf8(const char *string, int length=-1);
    rmxToUtf8(const wchar_t *string, int length=-1);
    rmxToUtf8(const unsigned int *string, int length=-1);
    ~rmxToUtf8();

  public:
    inline const char *getBuffer() { return buffer; }
	inline const char *getBufferSafe(const char *s="") { return buffer ? buffer : s; }
    inline int getLength() { return len; }

  private:
    char *buffer;
    int len;
};


#endif //_RMX_UNICODE_H_
