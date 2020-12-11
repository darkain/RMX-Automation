/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _FUNC_GROUP_H_
#define _FUNC_GROUP_H_


#include "../cfg/cfgBase.h"


class cfgRoot;


class funcGroup : public cfgBase {
  public:
    funcGroup(const char *function_name, const char *script_name, cfgRoot *parent,        HPARAMLIST paramlist=NULL);
    funcGroup(const char *function_name, const char *script_name, funcGroup *parent=NULL, HPARAMLIST paramlist=NULL);
    virtual ~funcGroup();

  public:
    inline const char *getFunctionName() const { return getName();          }
    inline const char *getScriptName()   const { return getParam("script"); }


    BOOL script_isValueNull(   const char *name) const;
    BOOL script_isValueBoolean(const char *name) const;
    BOOL script_isValueInteger(const char *name) const;
    BOOL script_isValueNumber( const char *name) const;
    BOOL script_isValueString( const char *name) const;

    void script_setValueBoolean(const char *name, BOOL        value);
    void script_setValueInteger(const char *name, int         value);
    void script_setValueNumber( const char *name, double      value);
    void script_setValueString( const char *name, const char *value);
    inline void script_setValueNull(const char *name) { script_setValueString(name, NULL); }

    BOOL        script_getValueBoolean(const char *name) const;
    int         script_getValueInteger(const char *name) const;
    double      script_getValueNumber( const char *name) const;
    const char *script_getValueString( const char *name) const;
    const char *script_getValueSafe(   const char *name, const char *safe="") const;
};


#endif //_FUNC_GROUP_H_
