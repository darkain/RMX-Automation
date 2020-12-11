/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _XML_READ_H_
#define _XML_READ_H_


#include "expat.h"
#include "../stringlist.h"


class xmlParamList : public vNullStringPairList<const char*> {
  public: xmlParamList(const char **buffer) : vNullStringPairList<const char*>(buffer) {}
};


class xmlRead {
  public:
    xmlRead();
    virtual ~xmlRead();

  protected:
    virtual void xml_onStartElement(const char *xmltag, xmlParamList *params) {}
    virtual void xml_onEndElement(const char *xmltag) {}
    virtual void xml_onCharacterData(const char *str) {}
    virtual void xml_onParseError(XML_Error code, const char *errstr, int line) {}

  public:
    BOOL xml_parse(const char *buffer, int len, BOOL final=FALSE);

    inline int getDepth() { return depth; }

  private:
    static void XMLCALL _xml_startElement(void *userData, const char *name, const char **atts);
    static void XMLCALL _xml_endElement(void *userData, const char *name);
    static void XMLCALL _xml_charData(void *userData, const XML_Char *s, int len);
    void _xml_sendCharData();

  private:
    int depth;
    XML_Parser parser;

    int buflen;
    char *buffer;
};


#endif //_XML_READ_H_
