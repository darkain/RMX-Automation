/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "xmlreadfile.h"


xmlReadFile::xmlReadFile() {
  file = NULL;
}


xmlReadFile::~xmlReadFile() {
  if (file) fclose(file);
}


void xmlReadFile::loadFile(const char *fn) {
  DEBUGMSG("XML", fn);
  VFOPEN(&file, fn, "rb");
  parseFile();
}


void xmlReadFile::loadFile(FILE *f) {
  file = f;
  parseFile();
}


void xmlReadFile::parseFile() {
  if (!file) {
    xml_onParseError(XML_ERROR_NONE, "Unable to open file", 0);
    return;
  }

  BOOL done = FALSE;
  char buffer[1024];

  while (!done) {
    size_t len = fread(buffer, 1, sizeof(buffer), file);
    done = len < sizeof(buffer);
    if (!xml_parse(buffer, len, done)) break;
  }
}
