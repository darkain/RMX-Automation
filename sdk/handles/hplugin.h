/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __HPLUGIN_H_
#define __HPLUGIN_H_


#include "../rmxTypes.h"
#include "../rmxGuid.h"


class cbCore;


class vHPLUGIN {
  private:
    vHPLUGIN() {}
    ~vHPLUGIN() {}


  public:

    //--------------------------------------------
    //HPLUGIN info related functions
    //--------------------------------------------

    //get a plugin handle from RMX's core
    static HPLUGIN getFirst();
    static HPLUGIN getLast();
    static HPLUGIN getPrev(HPLUGIN plugin);
    static HPLUGIN getNext(HPLUGIN plugin);
    static HPLUGIN getMaster();
    static HPLUGIN getByName(    const char *name);
    static HPLUGIN getByFilename(const char *filename);
    static HPLUGIN loadPlugin(   const char *filename);

    //get some basic information about the plugin
    BOOL isValid()    const;
    BOOL isMaster()   const;
    BOOL unload();

    const char *getPath() const;
    const char *getFilename() const;

    inline RMXLONG     getId()   const { return list_getId(  list_getRoot()); }
    inline const char *getName() const { return list_getName(list_getRoot()); }


    //--------------------------------------------
    //HLIST related functions
    //--------------------------------------------

    //some basic helper functions
    inline HLIST root_getFirstChild()                             const { return list_getFirstChild(list_getRoot());             }
    inline HLIST root_getLastChild()                              const { return list_getLastChild( list_getRoot());             }
    inline HLIST root_getPrevChild(HLIST child)                   const { return list_getPrevChild( list_getRoot(), child);      }
    inline HLIST root_getNextChild(HLIST child)                   const { return list_getNextChild( list_getRoot(), child);      }
    inline HLIST root_getList(const char *name, const char *type) const { return list_getChild(     list_getRoot(), name, type); }
    inline HLIST root_getList(RMXLONG id)                         const { return list_getChildById( list_getRoot(), id,   TRUE); }
    inline BOOL  root_hasList(HLIST list)                         const { return list_hasChild(     list_getRoot(), list, TRUE); }

    HLIST list_getRoot() const;

    RMXLONG list_message(HLIST list, RMXLONG message, RMXLONG param1=0, RMXLONG param2=0);
    
    inline RMXLONG root_message(RMXLONG message, RMXLONG param1=0, RMXLONG param2=0) {
      return list_message(list_getRoot(), message, param1, param2);
    }



    const char *list_getType(HLIST list) const;


    //primary list information - TODO: THIS WHOLE SECTION IS DEPRICATED
    const char *list_getName(   HLIST list) const;
    void        list_setName(   HLIST list, const char *name);
    __int64     list_getId(     HLIST list) const;
    void        list_setId(     HLIST list, __int64 newid);

	void list_onFirstLoad(HLIST list);


    // list properties and access rights
    UINT list_getFlags(     HLIST list) const;
    UINT list_getSecurity(  HLIST list) const;
    UINT list_getSecurityEx(HLIST list) const;
    

    // list properties and access rights - specifics
    BOOL list_isVisible(             HLIST list) const;
    void list_setVisible(            HLIST list, BOOL visible);
    BOOL list_hasChildrenVisible(    HLIST list, BOOL recursive=FALSE) const;

    BOOL list_isEnabled(             HLIST list) const;
    void list_setEnabled(            HLIST list, BOOL enable);
    BOOL list_hasChildrenEnabled(    HLIST list, BOOL recursive=FALSE) const;
    
	BOOL list_isHighlighted(         HLIST list) const;
    void list_setHighlighted(        HLIST list, BOOL highlight);
    BOOL list_hasChildrenHighlighted(HLIST list, BOOL recursive=FALSE) const;
    
	BOOL list_isExpanded(            HLIST list) const;
    void list_setExpanded(           HLIST list, BOOL expanded);
    BOOL list_hasChildrenExpanded(   HLIST list, BOOL recursive=FALSE) const;
    
	BOOL list_isSavable(             HLIST list) const;
    void list_setSavable(            HLIST list, BOOL savable);
    BOOL list_hasChildrenSavable(    HLIST list, BOOL recursive=FALSE) const;
    
	BOOL list_isReadOnly(            HLIST list) const;
    BOOL list_hasChildrenReadOnly(   HLIST list, BOOL recursive=FALSE) const;


	HPARAM list_getListParam(HLIST list) const;

    //get param value from a list
    const char *list_getParam(      HLIST list, const char *name) const;
    const char *list_getParamSafe(  HLIST list, const char *name, const char *safe="") const;
    int         list_getParamInt(   HLIST list, const char *name) const;
    RMXLONG     list_getParamLong(  HLIST list, const char *name) const;
    double      list_getParamNumber(HLIST list, const char *name) const;
    GUID        list_getParamGuid(  HLIST list, const char *name) const;
    FOURCC      list_getParamFourCC(HLIST list, const char *name) const;

    //set param value from a list
    void list_setParam(      HLIST list, const char *name, const char *value);
    void list_setParamString(HLIST list, const char *name, const char *value);
    void list_setParamInt(   HLIST list, const char *name, int         value);
    void list_setParamLong(  HLIST list, const char *name, RMXLONG     value);
    void list_setParamNumber(HLIST list, const char *name, double      value);
    void list_setParamGuid(  HLIST list, const char *name, GUID        value);
    void list_setParamFourCC(HLIST list, const char *name, FOURCC      value);

    //does the list have a specific param?
    BOOL list_hasParam( HLIST list, HPARAM param) const;
    BOOL list_hasParam( HLIST list, const char *param) const;
    BOOL list_hasParams(HLIST list) const;

    //add params to a list
    HPARAM list_insertParam(      HLIST list, const char *name, HPARAM insert=NULL);
    HPARAM list_insertParamNoSave(HLIST list, const char *name, HPARAM insert=NULL);

    //get param data from a list
    BOOL   list_canEnumerateParams(HLIST list) const;
    int    list_getParamCount(     HLIST list) const;
    HPARAM list_getParamFirst(     HLIST list) const;
    HPARAM list_getParamLast(      HLIST list) const;
    HPARAM list_getParamPrev(      HLIST list, HPARAM param) const;
    HPARAM list_getParamNext(      HLIST list, HPARAM param) const;
    HPARAM list_getParamHandle(    HLIST list, const char *name) const;
    HPARAM list_getParamById(      HLIST list, __int64 id) const;

    //some other param stuff
    UINT   list_getParamFlags(  HLIST list, const char *param) const;
    BOOL   list_isParamSavable( HLIST list, const char *param) const;
    BOOL   list_isParamReadOnly(HLIST list, const char *param) const;
	BOOL   list_isParamUserData(HLIST list, HPARAM      param) const;
    void   list_setParamSavable(HLIST list, const char *param, BOOL save);
    HPARAM list_newParam(       HLIST list, const char *param, const char *value);
    void   list_removeParam(    HLIST list, HPARAM      param);
    void   list_removeParam(    HLIST list, const char *param);
    void   list_removeAllParams(HLIST list);

    //move param
    void   list_moveParam(HLIST list, HPARAM      param, HPARAM      insert);
    void   list_moveParam(HLIST list, const char *param, const char *insert);

    //list hierarchy (parent info, child info, etc)
    HLIST list_getParent(     HLIST list) const;
    HLIST list_getChild(      HLIST list, const char *name, const char *type) const;
    HLIST list_getChildById(  HLIST list, RMXLONG  id, BOOL recursive=FALSE)  const;
    BOOL  list_hasChild(      HLIST list, HLIST child, BOOL recursive=FALSE)  const;
    BOOL  list_hasChildCached(HLIST list, HLIST child) const;
    BOOL  list_hasChildren(   HLIST list) const;
    int   list_getChildCount( HLIST list) const;
    HLIST list_getFirstChild( HLIST list) const;
    HLIST list_getLastChild(  HLIST list) const;
    HLIST list_getPrevChild(  HLIST list, HLIST child) const;
    HLIST list_getNextChild(  HLIST list, HLIST child) const;

    HLIST list_newExternalChild(HLIST list, const char *name, const char *type);

    void list_moveUp(      HLIST list);
    void list_moveDown(    HLIST list);
    void list_moveBegining(HLIST list);
    void list_moveEnd(     HLIST list);
    void list_moveChild(   HLIST list, HLIST child, HLIST insert);


    void list_insertCallback(HLIST list, cbCore *callback);
    void list_removeCallback(HLIST list, cbCore *callback);


    //--------------------------------------------
    //HPARAM related functions
    //--------------------------------------------

    //get info about a specific param
    const char *param_getName(      HPARAM param) const;
    UINT        param_getFlags(     HPARAM param) const;
    UINT        param_getSecurity(  HPARAM param) const;
    UINT        param_getSecurityEx(HPARAM param) const;
	__int64     param_getId(        HPARAM param) const;
	void        param_setId(        HPARAM param, __int64 newid);

    //get value of a specific param
    const char *param_getValue(      HPARAM param) const;
    const char *param_getValueSafe(  HPARAM param, const char *safe="") const;
    int         param_getValueInt(   HPARAM param) const;
    RMXLONG     param_getValueLong(  HPARAM param) const;
    double      param_getValueNumber(HPARAM param) const;
    GUID        param_getValueGuid(  HPARAM param) const;
    FOURCC      param_getValueFourCC(HPARAM param) const;

    //set value of a specific param
    void param_setValue(      HPARAM param, const char *value);
    void param_setValueString(HPARAM param, const char *value);
    void param_setValueInt(   HPARAM param, int         value);
    void param_setValueLong(  HPARAM param, RMXLONG     value);
    void param_setValueNumber(HPARAM param, double      value);
    void param_setValueGuid(  HPARAM param, GUID        value);
    void param_setValueFourCC(HPARAM param, FOURCC      value);

    //check data type of specific param
    BOOL param_isNull(   HPARAM param) const;
    BOOL param_isString( HPARAM param) const;
    BOOL param_isInteger(HPARAM param) const;
    BOOL param_isLong(   HPARAM param) const;
    BOOL param_isNumber( HPARAM param) const;
    BOOL param_isGuid(   HPARAM param) const;
    BOOL param_isFourCC( HPARAM param) const;
    
    //is the param savable
    BOOL param_isSavable( HPARAM param) const;
    BOOL param_isReadOnly(HPARAM param) const;

    //some extras
    BOOL param_isNamed(HPARAM param, const char *value) const;
};


#endif //__HPLUGIN_H_
