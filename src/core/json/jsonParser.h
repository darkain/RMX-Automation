/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __JSON_PARSER_H__
#define __JSON_PARSER_H__


#include "../../../sdk/rmxBase.h"
#include "../../../sdk/func/funcSingle.h"
#include "../../../sdk/json/JSON_parser.h"



class rmxJsonParser : public funcSingle {
  public:
    rmxJsonParser(cfgRoot *parent);
    virtual ~rmxJsonParser();

  protected:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);


  public:
    static BOOL parseJson(const char *json);

  protected:
    static int printJson(void* ctx, int type, const JSON_value* value);
};



#endif //__JSON_PARSER_H__
