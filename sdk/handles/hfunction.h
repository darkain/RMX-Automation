/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __HFUNCTION_H_
#define __HFUNCTION_H_


#include "../rmxTypes.h"


/*
Don't directly create instances of this class.  Use the instances
provided to you from the functions listed in "plugBase.h"
*/


class vHFUNCTION {
  private:
    vHFUNCTION() {}
    ~vHFUNCTION() {}

  public:
    const char *getFunctionName() const;
    const char *getScriptName() const;
    
    const char *getToolTip() const;

    int getType() const;
    BOOL isEnabled() const;
    BOOL isVisible() const;

    HFUNCTION getParent() const;

    HPLUGIN getPlugin() const;
    HLIST getList() const;

    //only for script functions
    int getParamCount() const;  

    //only for function groups
    HFUNCTION child_getFirst() const;
    HFUNCTION child_getLast()  const;
    HFUNCTION child_getPrev(HFUNCTION script) const;
    HFUNCTION child_getNext(HFUNCTION script) const;

  public:
    static HFUNCTION getFirst();
    static HFUNCTION getLast();
    static HFUNCTION getPrev(HFUNCTION function);
    static HFUNCTION getNext(HFUNCTION function);
    
    static BOOL global_isValueNull(  const char *name);
    static BOOL global_isValueBool(  const char *name);
    static BOOL global_isValueInt(   const char *name);
    static BOOL global_isValueNumber(const char *name);
    static BOOL global_isValueString(const char *name);

    static void global_setValueBoolean(const char *name, BOOL        value);
    static void global_setValueInteger(const char *name, int         value);
    static void global_setValueNumber( const char *name, double      value);
    static void global_setValueString( const char *name, const char *value);
    inline static void global_setValueNull(const char *name) { global_setValueString(name, NULL); }

    static BOOL        global_getValueBoolean(const char *name);
    static int         global_getValueInteger(const char *name);
    static double      global_getValueNumber( const char *name);
    static const char *global_getValueString( const char *name);
    static const char *global_getValueSafe(   const char *name, const char *safe="");
};


#endif //__HFUNCTION_H_
