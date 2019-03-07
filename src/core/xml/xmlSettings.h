/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _XML_SETTINGS_H_
#define _XML_SETTINGS_H_


#ifdef _RMX_XML_SETTINGS_


#include "../../../sdk/rmxList.h"
#include "../../../sdk/xml/xmlreadfile.h"
#include "../plugin.h"


class xmlSetting : public xmlReadFile {
  public:
    xmlSetting(vPlugin *plug);
    virtual ~xmlSetting();


  protected:
    virtual void xml_onStartElement( const char *xmltag, xmlParamList *params);
    virtual void xml_onEndElement(   const char *xmltag);
    virtual void xml_onCharacterData(const char *str);


  private:
    struct xmlItem {
      rmxName name;
      HLIST list;
    };

  private:
    char path[MAX_PATH];
    vPlugin *plugin;
    UINT section;
    UINT list;
    rmxList<xmlItem*> items;
};


#endif //_RMX_XML_SETTINGS_

#endif //_XML_SETTINGS_H_
