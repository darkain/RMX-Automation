/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "xmltok_impl.h"
#include "xmlwrite.h"


#define XMLVERSION "1.0"


static unsigned char xmltypecheck[256] = {
#include "asciitab.h"
#include "utf8tab.h"
};


xmlWrite::xmlWrite(const char *filename, const char *doctype, const char *dtddoctype) {
  FILE *f = NULL;
  VFOPEN(&f, filename, "wt");
  Init(f, doctype, dtddoctype);
}


xmlWrite::xmlWrite(FILE *file, const char *doctype, const char *dtddoctype) {
  Init(file, doctype, dtddoctype);
}


void xmlWrite::Init(FILE *file, const char *doctype, const char *dtddoctype) {
  fp = file;
//  ASSERT(fp != NULL);   //TODO: sheet, need exceptions here
  indent = 0;
  fprintf(fp, "<?xml version=\"" XMLVERSION "\" encoding='UTF-8' standalone=\"yes\"?>\n");
  if (dtddoctype != NULL) fprintf(fp, "<!DOCTYPE %s>\n", dtddoctype);
  pushCategory(doctype, 1, 0);
}


xmlWrite::~xmlWrite() {
  popCategory(1, 0);
  fflush(fp);
  fclose(fp);
//  ASSERT(titles.peek() == 0);
}


void xmlWrite::comment(const char *comment) {
  fprintf(fp, "<!-- %s -->\n", comment);
}


void xmlWrite::pushCategory(const char *title, int wantcr, int wantindent) {
  if (wantindent) { // because %*s will put one space even if ident == 0 :/
    fprintf(fp, "%*s<%s>%s", indent*2, " ", title, wantcr ? "\n": "");
  } else {
    fprintf(fp, "<%s>%s", title, wantcr ? "\n": "");
  }

  indent++;
  titles.appendItem(new rmxString(title));
}


void xmlWrite::pushCategoryAttrib(const char *title, int nodata) {
  fprintf(fp, "%*s<%s", indent*2, " ", title);
  indent++;
  titles.appendItem( new rmxString( (nodata) ? (XML_WRITE_EMPTY) : (title) ) );
}


void xmlWrite::writeCategoryAttrib(const char *title, const int val) {
  fprintf(fp, " %s=\"%d\"", title, val);
}


void xmlWrite::writeCategoryAttrib(const char *title, const char *str) {
  if (!str) str = "";
  fprintf(fp, " %s=\"", title);
  writeRawData(str, 0);
  fprintf(fp, "\"");
}


void xmlWrite::closeCategoryAttrib(int wantcr) {
  if ((void*)titles.getLastItem() == ((void*)XML_WRITE_EMPTY)) fprintf(fp, " /");
  fprintf( fp, ">%s", (wantcr) ? ("\n") : ("") );
}


void xmlWrite::writeAttrib(const char *title, const char *text, int wantcr, int wantindent) {
  if (!text) text = "";

  if (wantindent)
    fprintf(fp, "%*s<%s>", indent*2, " ", title);
  else
    fprintf(fp, "<%s>", title);

  writeRawData(text, 0);
  fprintf(fp, "</%s>%s", title, wantcr ? "\n" : "");
}


void xmlWrite::writeAttrib(const char *title, int val, int wantcr, int wantindent) {
  if (wantindent)
    fprintf(fp, "%*s<%s>%d</%s>%s", indent*2, " ", title, val, title, wantcr ? "\n" : "");
  else
    fprintf(fp, "<%s>%d</%s>%s", title, val, title, wantcr ? "\n" : "");
}


int xmlWrite::popCategory(int wantcr, int wantindent) {
  indent--;

  rmxString *title = titles.getLastItem();
  titles.removeLastItem();

  if (title != NULL  &&  (void*)title != (void*)XML_WRITE_EMPTY) {
    if (wantindent) {
      fprintf(fp, "%*s</%s>%s", indent*2, " ", title, wantcr ? "\n" : "");
    } else {
      fprintf(fp, "</%s>%s", title, wantcr ? "\n" : "");
    }
  }

  delete title;
  return titles.getItemCount();
}


int xmlWrite::writeRawData(const char *text, int wantcr) {
  int count = 0;

  while (*text) {
    switch (*text) {
      case '<':  fputs("&lt;",   fp); count += 4; break;
      case '>':  fputs("&gt;",   fp); count += 4; break;
      case '&':  fputs("&amp;",  fp); count += 5; break;
      case '\"': fputs("&quot;", fp); count += 6; break;
      case '\'': fputs("&apos;", fp); count += 6; break;

      default:
        unsigned char x = (unsigned char) *text;
        if (xmltypecheck[x] != 0) {
          fputc(*text, fp);
          count++;
        }
        break;
    }

    text++;
  }


  if (wantcr) {
    fputs("\n", fp);
    count++;
  }

  return count;
}
