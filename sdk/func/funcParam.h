/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _FUNC_PARAM_H_
#define _FUNC_PARAM_H_


#include "../cfg/cfgBase.h"


/*
UserData status is unsure
*/

/*
soon to be obsoleted by generic vconfiglist types, maybe?
*/



class funcParam : public cfgBase {
  public:
    funcParam(const char *itemName, const char *flagName, const char *type, cfgBase *parent, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    virtual ~funcParam();

  public:
    cfgBase *addListItem(const char *name);

    void setDefaultValue(cfgBase *list);
    inline void setDefaultValue(      const char *value) { setParamString("default", value); }
    inline void setDefaultValueInt(   int         value) { setParamInt(   "default", value); }
    inline void setDefaultValueLong(  RMXLONG     value) { setParamLong(  "default", value); }
    inline void setDefaultValueNumber(double      value) { setParamNumber("default", value); }
    inline void setDefaultValueGuid(  GUID        value) { setParamGuid(  "default", value); }

    inline const char *getDefaultValue()       const { return getParam(      "default"); }
    inline int         getDefaultValueInt()    const { return getParamInt(   "default"); }
    inline RMXLONG     getDefaultValueLong()   const { return getParamLong(  "default"); }
    inline double      getDefaultValueNumber() const { return getParamNumber("default"); }
    inline GUID        getDefaultValueGuid()   const { return getParamGuid(  "default"); }

    inline const char *getPrefix() const { return getParam("prefix"); }
    inline const char *getSuffix() const { return getParam("suffix"); }
    inline void setPrefix(const char *prefix) { setParamString("prefix", prefix); }
    inline void setSuffix(const char *suffix) { setParamString("suffix", suffix); }
};


#endif //_FUNC_PARAM_H_
