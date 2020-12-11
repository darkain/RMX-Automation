/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _XML_READ_FILE_H_
#define _XML_READ_FILE_H_


#include "xmlread.h"
#include <stdio.h>


class xmlReadFile : public xmlRead {
  public:
    xmlReadFile();
    virtual ~xmlReadFile();

  public:
    void loadFile(const char *filename);
    void loadFile(FILE *file);

  private:
    void parseFile();

  private:
    FILE *file;
};


#endif //_XML_READ_FILE_H_
