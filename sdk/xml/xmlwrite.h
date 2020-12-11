/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _XML_WRITE_H_
#define _XML_WRITE_H_


#include "../rmxList.h"
#include "../rmxString.h"
#include <stdio.h>


#define XML_WRITE_EMPTY ((const char*)-1)


class xmlWrite {
  public:
    xmlWrite(const char *filename, const char *doctype, const char *dtddoctype=NULL);
    xmlWrite(FILE *file, const char *doctype, const char *dtddoctype=NULL);
    ~xmlWrite();

  private:
    void Init(FILE *file, const char *doctype, const char *dtddoctype);

  public:
    void comment(const char *comment);

    void pushCategory(const char *title, int wantcr=1, int wantindent=1);

    void pushCategoryAttrib(const char *title, int nodata=FALSE);
    void writeCategoryAttrib(const char *title, const int val);
    void writeCategoryAttrib(const char *title, const char *val);
    void closeCategoryAttrib(int wantcr=1);

    void writeAttrib(const char *title, const char *text, int wantcr=1, int wantindent=1);
    void writeAttrib(const char *title, int val, int wantcr=1, int wantindent=1);
    int popCategory(int wantcr=1, int wantindent=1);	// returns nonzero if more categories are open

    int writeRawData(const char *text, int wantcr=1);   // returns number of characters writen


  private:
    FILE *fp;
    int indent;
    rmxList<rmxString*> titles;
};


#endif //_XML_WRITE_H_
