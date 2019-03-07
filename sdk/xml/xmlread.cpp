/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "xmlread.h"


xmlRead::xmlRead() {
  depth  = 0;
  buffer = NULL;
  buflen = 0;

  parser = XML_ParserCreate(NULL);
  XML_SetUserData(parser, this);
  XML_SetElementHandler(parser, _xml_startElement, _xml_endElement);
  XML_SetCharacterDataHandler(parser, _xml_charData);
}


xmlRead::~xmlRead() {
  XML_ParserFree(parser);

  free(buffer);
  buffer = NULL;
}


BOOL xmlRead::xml_parse(const char *buffer, int len, BOOL final) {
  if (XML_Parse(parser, buffer, len, final) == XML_STATUS_ERROR) {
    XML_Error code = XML_GetErrorCode(parser);
    xml_onParseError(code, XML_ErrorString(code), XML_GetCurrentLineNumber(parser));
    return FALSE;
  }
  return TRUE;
}


void XMLCALL xmlRead::_xml_startElement(void *userData, const char *name, const char **atts) {
  xmlRead *xml = (xmlRead*)userData;
  
  xml->_xml_sendCharData();

  xml->depth++;
  xmlParamList params(atts);
  xml->xml_onStartElement(name, &params);
}


void XMLCALL xmlRead::_xml_endElement(void *userData, const char *name) {
  xmlRead *xml = (xmlRead*)userData;

  xml->_xml_sendCharData();

  xml->xml_onEndElement(name);
  xml->depth--;
}


void XMLCALL xmlRead::_xml_charData(void *userData, const XML_Char *s, int len) {
  xmlRead *xml = (xmlRead*)userData;

  xml->buffer = (char*) realloc(xml->buffer, xml->buflen + len + 1);
  memcpy(xml->buffer + xml->buflen, s, len);
  xml->buflen += len;
  xml->buffer[xml->buflen] = NULL;
}


void xmlRead::_xml_sendCharData() {
  if (!buffer) return;

  char *str = buffer;

  while (*str) {
    if      (*str == ' ')  str++;
    else if (*str == '\t') str++;
    else if (*str == '\r') str++;
    else if (*str == '\n') str++;
    else break;
  }

  if (*str) {
    char *end = (char*)str;
    while (*end) end++;
    while (end > str) {
      end--;
      if      (*end == ' ')  *end = NULL;
      else if (*end == '\t') *end = NULL;
      else if (*end == '\r') *end = NULL;
      else if (*end == '\n') *end = NULL;
      else break;
    }

    xml_onCharacterData(str);
  }

  free(buffer);
  buffer = NULL;
  buflen = 0;
}
