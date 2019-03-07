/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _XML_READER_H_
#define _XML_READER_H_


//#include <api/xml/xmlreader.h>
//TODO:  reimplement this


class xmlReader /*: public XmlReader, public XmlReaderCallbackI */{
  public:
    xmlReader(const char *filter, void *remoteptr, vPlugin *plug) {
/*      remote = remoteptr;
      plugin = plug;

      if (filter) {
        registerCallback(filter, this);  //come back and make this prettie, like auto-appending '/*' if it doesnt have it
      } else {
        registerCallback("*", this);
      }*/
    }

    virtual ~xmlReader() {
//      unregisterCallback(this);
    }

  public:/*
    virtual void xmlReaderOnStartElementCallback(const char *xmlpath, const char *xmltag, XmlReaderParams *params) {
      plugin->xmlreader_onElementStart(remote, xmlpath, xmltag, params);
    }

    virtual void xmlReaderOnCharacterDataCallback(const char *xmlpath, const char *xmltag, const char *str) {
      plugin->xmlreader_onCharacterData(remote, xmlpath, xmltag, str);
    }

    virtual void xmlReaderOnEndElementCallback(const char *xmlpath, const char *xmltag) {
      plugin->xmlreader_onElementEnd(remote, xmlpath, xmltag);
    }

    virtual void xmlReaderOnError(const char *filename, int linenum, const char *incpath, int errcode, const char *errstr) {
      plugin->xmlreader_onError(remote, linenum, errcode, errstr);
    }

    virtual int xmlReaderDisplayErrors() { return 0; }
    virtual int xmlReaderIncludeFiles()  { return 0; }
*/
  private:
    void    *remote;
    vPlugin *plugin;
};


#endif //_XML_READER_H_
