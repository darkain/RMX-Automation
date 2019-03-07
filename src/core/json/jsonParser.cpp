/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007, 2009 Vincent E. Milum Jr., All rights reserved.  *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "jsonParser.h"



rmxJsonParser::rmxJsonParser(cfgRoot *parent) : funcSingle("JSON Command", NULL, parent) {
  funcParam *sub = new funcParam("Command", "f_json", "string", this);
}

rmxJsonParser::~rmxJsonParser() {
}


void rmxJsonParser::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  const char *json = source->list_getParam(input, "f_json");
  rmxJsonParser::parseJson(json);
}



BOOL rmxJsonParser::parseJson(const char *json) {
  BOOL ret = FALSE;
  if (!json) return ret;
  if (!*json) return ret;

  JSON_config config;
  init_JSON_config(&config);

  config.depth                  = 20;
  config.callback               = &rmxJsonParser::printJson;
  config.allow_comments         = 1;
  config.handle_floats_manually = 0;

  struct JSON_parser_struct *jc = new_JSON_parser(&config);

  while (*json) {
    if (!JSON_parser_char(jc, *json)) {
      char err[100] = "";
      VPRINTF(err, sizeof(err), "Character Error: %c", *json);
      CONSOLE_ACTIVE->print("JSON", err);
    }
    json++;
  }
  JSON_parser_char(jc, *json);

  if (!JSON_parser_done(jc)) {
    CONSOLE_ACTIVE->print("JSON", "Syntax Error - Code:");
    CONSOLE_ACTIVE->print("JSON", json);
  } else {
    ret = TRUE;
  }

  delete_JSON_parser(jc);

  return ret;
}


int rmxJsonParser::printJson(void* ctx, int type, const JSON_value* value) {
  switch(type) {
    case JSON_T_ARRAY_BEGIN:    
      CONSOLE_ACTIVE->print("JSON", "[");
        //printf("[\n");
      //  ++s_Level;
        break;
    case JSON_T_ARRAY_END:
        //assert(!s_IsKey);
        //if (s_Level > 0) --s_Level;
        //print_indention();
        //printf("]\n");
      CONSOLE_ACTIVE->print("JSON", "]");
        break;
   case JSON_T_OBJECT_BEGIN:
     CONSOLE_ACTIVE->print("JSON", "{");
//       if (!s_IsKey) print_indention();
//       s_IsKey = 0;
//       printf("{\n");
 //       ++s_Level;
        break;
    case JSON_T_OBJECT_END:
      CONSOLE_ACTIVE->print("JSON", "}");
//        assert(!s_IsKey);
//        if (s_Level > 0) --s_Level;
//        print_indention();
//        printf("}\n");
        break;
    case JSON_T_INTEGER:
      CONSOLE_ACTIVE->print("JSON", "INT");
//        if (!s_IsKey) print_indention();
//        s_IsKey = 0;
//        printf("integer: "JSON_PARSER_INTEGER_SPRINTF_TOKEN"\n", value->vu.integer_value);
        break;
    case JSON_T_FLOAT:
      CONSOLE_ACTIVE->print("JSON", "FLOAT");
//        if (!s_IsKey) print_indention();
//        s_IsKey = 0;
//        printf("float: %Lf\n", value->vu.float_value); /* We wanted stringified floats */
        break;
    case JSON_T_NULL:
      CONSOLE_ACTIVE->print("JSON", "NULL");
//        if (!s_IsKey) print_indention();
//        s_IsKey = 0;
//        printf("null\n");
        break;
    case JSON_T_TRUE:
      CONSOLE_ACTIVE->print("JSON", "TRUE");
//        if (!s_IsKey) print_indention();
//        s_IsKey = 0;
//        printf("true\n");
        break;
    case JSON_T_FALSE:
      CONSOLE_ACTIVE->print("JSON", "FALSE");
//        if (!s_IsKey) print_indention();
//        s_IsKey = 0;
//        printf("false\n");
        break;
    case JSON_T_KEY:
      CONSOLE_ACTIVE->print("JSON", value->vu.str.value);
//        s_IsKey = 1;
//        print_indention();
//        printf("key = '%s', value = ", value->vu.str.value);
        break;   
    case JSON_T_STRING:
      CONSOLE_ACTIVE->print("JSON", value->vu.str.value);
//        if (!s_IsKey) print_indention();
//        s_IsKey = 0;
//        printf("string: '%s'\n", value->vu.str.value);
        break;
    default:
      CONSOLE_ACTIVE->print("JSON", "OTHER");
//        assert(0);
        break;
    }

    return 1;
}
