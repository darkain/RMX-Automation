/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __PLUGIN_H__
#define __PLUGIN_H__



//#include "../../sdk/wnd/wndBase.h"
#include "../../sdk/rmxMsg.h"
#include "../../sdk/rmxList.h"
#include "../../sdk/cfg/cfgButton.h"
#include "../../sdk/xml/xmlRead.h"

#include "../rmxlib/rmx_msg.h"

#include "Loader.h"



class xmlWrite;
class XmlReaderParams;
class vLuaStack;
class vScriptTableCallback;
class plugCallback;


class vPlugin {
  friend class vPluginLoader;
  friend class vPluginManager;
  friend class plugCallback;

  public:
    vPlugin(const char *fname, HMODULE hmod=NULL);
    virtual ~vPlugin();

  public:
    virtual void onInitPlugin() {}
    virtual void onPostInitPlugin() {}

    inline  const char *getPath()         const { return path; }
    inline  const char *getFilename()     const { return filename; }
    virtual const char *getPathRootName() const { return ""; }
    inline  HMODULE gethMod()             const { return hMod; }
	inline  HPLUGIN getHandle()           const { return (HPLUGIN)(this); }
	virtual UINT getStatus() { return 0; }

    virtual BOOL removable() const { return TRUE;  }
    virtual BOOL external()  const { return FALSE; }

    inline  UINT getSdkVersion()    const { return list_getParamInt(root_getIdent(), "sdk"); }
    inline  UINT getLibVersion()    const { return list_getParamInt(root_getIdent(), "lib"); }
    inline  UINT getPluginVersion() const { return list_getParamInt(root_getIdent(), "ver"); }


    inline __int64     getPluginId() const { return list_getId(  list_getRoot()); }
    inline const char *getName()     const { return list_getName(list_getRoot()); }



//---------------------------------------------------------------------------------------------------------------



    inline HLIST root_getFirstChild()                                                      const { return list_getFirstChild(list_getRoot());                        }
    inline HLIST root_getLastChild()                                                       const { return list_getLastChild( list_getRoot());                        }
    inline HLIST root_getPrevChild(HLIST child)                                            const { return list_getPrevChild( list_getRoot(), child);                 }
    inline HLIST root_getNextChild(HLIST child)                                            const { return list_getNextChild( list_getRoot(), child);                 }
    inline HLIST root_getList(const char *name, const char *type, xmlParamList *paramlist) const { return list_getChild(     list_getRoot(), name, type, paramlist); }
    inline HLIST root_getList(RMXLONG id)                                                  const { return list_getChildById( list_getRoot(), id,   TRUE);            }
    inline HLIST root_getIdent()                                                           const { return root_getList("Identification", "info", NULL);              }
    BOOL         root_hasList(HLIST list, BOOL recursive=FALSE);

    //some special stuff
    virtual HLIST list_getRoot() const { return NULL; }
    inline void list_delete(HLIST list) { RmxNul((VMSG::CONFIG_BASE, DELETELIST, list)); }

    inline RMXLONG list_message(HLIST list, HPLUGIN source, RMXLONG message, RMXLONG param1=0, RMXLONG param2=0) {
      if (blockMessage(message)) return 0;
      RmxLng((VMSG::CONFIG_BASE, ONMESSAGE, list, source, message, param1, param2));
    }

#ifdef RMX_STANDALONE
    inline BOOL blockMessage(RMXLONG message) const { return FALSE; }
#else
    BOOL blockMessage(RMXLONG message) const;
#endif

    //primary list information
    inline const char *list_getName(HLIST list) const                { RmxStr((VMSG::CONFIG_BASE, GETNAME,     list)); }
    inline void        list_setName(HLIST list, const char *name)    { RmxNul((VMSG::CONFIG_BASE, SETNAME,     list, name)); }
    inline const char *list_getType(HLIST list) const                { RmxStr((VMSG::CONFIG_BASE, GETTYPE,     list)); }
    inline __int64     list_getId(  HLIST list) const                { RmxLng((VMSG::CONFIG_BASE, GET_LIST_ID, list)); }
    inline void        list_setId(  HLIST list, __int64 newid)       { RmxNul((VMSG::CONFIG_BASE, SET_LIST_ID, list, newid)); }

	inline void list_onFirstLoad(HLIST list) { RmxNul((VMSG::CONFIG_BASE, FIRSTLOAD, list)); }


    // list properties and access rights
    inline UINT list_getFlags(              HLIST list)                       const { RmxInt((VMSG::CONFIG_BASE, GETLISTFLAGS,           list));            }
    inline UINT list_getSecurity(           HLIST list)                       const { RmxInt((VMSG::CONFIG_BASE, GETLISTSECURITY,        list));            }
    inline UINT list_getSecurityEx(         HLIST list)                       const { RmxInt((VMSG::CONFIG_BASE, GETLISTSECURITYEX,      list));            }
    inline void list_setSecurityEx(         HLIST list, int ex)                     { RmxNul((VMSG::CONFIG_BASE, SETLISTSECURITYEX,      list, ex));            }
    

    // list properties and access rights - specifics
    inline BOOL list_isVisible(             HLIST list)                       const { RmxInt((VMSG::CONFIG_BASE, ISVISIBLE,              list));            }
    inline void list_setVisible(            HLIST list, BOOL visible)               { RmxNul((VMSG::CONFIG_BASE, SETVISIBLE,             list, visible));   }
    inline BOOL list_hasChildrenVisible(    HLIST list, BOOL recursive=FALSE) const { RmxInt((VMSG::CONFIG_BASE, HASVISIBLECHILDREN,     list, recursive)); }
    inline BOOL list_isEnabled(             HLIST list)                       const { RmxInt((VMSG::CONFIG_BASE, ISENABLED,              list));            }
    inline void list_setEnabled(            HLIST list, BOOL enable)                { RmxNul((VMSG::CONFIG_BASE, SETENABLED,             list, enable));    }
    inline BOOL list_hasChildrenEnabled(    HLIST list, BOOL recursive=FALSE) const { RmxInt((VMSG::CONFIG_BASE, HASENABLEDCHILDREN,     list, recursive)); }
    inline BOOL list_isHighlighted(         HLIST list)                       const { RmxInt((VMSG::CONFIG_BASE, ISHIGHLIGHTED,          list));            }
    inline void list_setHighlighted(        HLIST list, BOOL highlight)             { RmxNul((VMSG::CONFIG_BASE, SETHIGHLIGHTED,         list, highlight)); }
    inline BOOL list_hasChildrenHighlighted(HLIST list, BOOL recursive=FALSE) const { RmxInt((VMSG::CONFIG_BASE, HASHIGHLIGHTEDCHILDREN, list, recursive)); }
    inline BOOL list_isExpanded(            HLIST list)                       const { RmxInt((VMSG::CONFIG_BASE, ISEXPANDED,             list));            }
    inline void list_setExpanded(           HLIST list, BOOL expanded)              { RmxNul((VMSG::CONFIG_BASE, SETEXPANDED,            list, expanded));  }
    inline BOOL list_hasChildrenExpanded(   HLIST list, BOOL recursive=FALSE) const { RmxInt((VMSG::CONFIG_BASE, HASEXPANDEDCHILDREN,    list, recursive)); }
    inline BOOL list_isSavable(             HLIST list)                       const { RmxInt((VMSG::CONFIG_BASE, ISSAVABLE,              list));            }
    inline void list_setSavable(            HLIST list, BOOL savable)               { RmxNul((VMSG::CONFIG_BASE, SETSAVABLE,             list, savable));   }
    inline BOOL list_hasChildrenSavable(    HLIST list, BOOL recursive=FALSE) const { RmxInt((VMSG::CONFIG_BASE, HASSAVABLECHILDREN,     list, recursive)); }
    inline BOOL list_isReadOnly(            HLIST list)                       const { RmxInt((VMSG::CONFIG_BASE, ISREADONLY,             list));            }
//  inline void list_setReadOnly(           HLIST list, BOOL readonly)              { RmxNul((VMSG::CONFIG_BASE, SETREADONLY,            list, readonly));  }
    inline BOOL list_hasChildrenReadOnly(   HLIST list, BOOL recursive=FALSE) const { RmxInt((VMSG::CONFIG_BASE, HASREADONLYCHILDREN,    list, recursive)); }

	inline HPARAM list_getListParam(HLIST list) const { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, GET_LIST_PARAM, list)); }

    //get param value from a list
    inline const char *list_getParam(      HLIST list, const char *name)                      const { RmxStr((VMSG::CONFIG_BASE, GETPARAM,       list, name));        }
    inline const char *list_getParamSafe(  HLIST list, const char *name, const char *safe="") const { RmxSaf((VMSG::CONFIG_BASE, GETPARAMSAFE,   list, name, safe));  }
    inline int         list_getParamInt(   HLIST list, const char *name)                      const { RmxInt((VMSG::CONFIG_BASE, GETPARAMINT,    list, name));        }
    inline RMXLONG     list_getParamLong(  HLIST list, const char *name)                      const { RmxLng((VMSG::CONFIG_BASE, GETPARAMLONG,   list, name));        }
    inline double      list_getParamNumber(HLIST list, const char *name)                      const { RmxNum((VMSG::CONFIG_BASE, GETPARAMNUMBER, list, name));        }
    inline GUID        list_getParamGuid(  HLIST list, const char *name)                      const { RmxUid((VMSG::CONFIG_BASE, GETPARAMGUID,   list, name));        }
    inline FOURCC      list_getParamFourCC(HLIST list, const char *name)                      const { RmxFcc((VMSG::CONFIG_BASE, GETPARAMFOURCC, list, name));        }

    //set param value from a list
    inline void list_setParam(      HLIST list, const char *name, const char *value) { RmxNul((VMSG::CONFIG_BASE, SETPARAM,       list, name, value)); }
    inline void list_setParamString(HLIST list, const char *name, const char *value) { RmxNul((VMSG::CONFIG_BASE, SETPARAMSTRING, list, name, value)); }
    inline void list_setParamInt(   HLIST list, const char *name, int         value) { RmxNul((VMSG::CONFIG_BASE, SETPARAMINT,    list, name, value)); }
    inline void list_setParamLong(  HLIST list, const char *name, RMXLONG     value) { RmxNul((VMSG::CONFIG_BASE, SETPARAMLONG,   list, name, value)); }
    inline void list_setParamNumber(HLIST list, const char *name, double      value) { RmxNul((VMSG::CONFIG_BASE, SETPARAMNUMBER, list, name, value)); }
    inline void list_setParamGuid(  HLIST list, const char *name, GUID        value) { RmxNul((VMSG::CONFIG_BASE, SETPARAMGUID,   list, name, value)); }
    inline void list_setParamFourCC(HLIST list, const char *name, FOURCC      value) { RmxNul((VMSG::CONFIG_BASE, SETPARAMFOURCC, list, name, value)); }

    //does the list have a specific param?
    inline BOOL list_hasParam( HLIST list, HPARAM param)      const { RmxInt((VMSG::CONFIG_BASE, HASPARAM,    list, param)); }
    inline BOOL list_hasParam( HLIST list, const char *param) const { RmxInt((VMSG::CONFIG_BASE, HASPARAMSTR, list, param)); }
    inline BOOL list_hasParams(HLIST list)                    const { RmxInt((VMSG::CONFIG_BASE, HASPARAMS,   list));        }
/*
    //add params to a list
    inline HPARAM list_insertParam(        HLIST list, const char *name, const char *value, HPARAM insert=NULL) { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, INSERTPARAM,         list, name, value, insert)); }
    inline HPARAM list_insertParamInt(     HLIST list, const char *name, int         value, HPARAM insert=NULL) { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, INSERTPARAMINT,      list, name, value, insert)); }
    inline HPARAM list_insertParamNoCfg(   HLIST list, const char *name, const char *value, HPARAM insert=NULL) { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, INSERTPARAMNOCFG,    list, name, value, insert)); }
    inline HPARAM list_insertParamIntNoCfg(HLIST list, const char *name, int         value, HPARAM insert=NULL) { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, INSERTPARAMINTNOCFG, list, name, value, insert)); }
*/
    //get param data from a list
    inline BOOL   list_canEnumerateParams(HLIST list)                   const { RmxInt(        (VMSG::CONFIG_BASE, CANENUMPARAMS,   list));        }
    inline int    list_getParamCount(     HLIST list)                   const { RmxInt(        (VMSG::CONFIG_BASE, GETPARAMCOUNT,   list));        }
    inline HPARAM list_getParamFirst(     HLIST list)                   const { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, GETFIRSTPARAM,   list));        }
    inline HPARAM list_getParamLast(      HLIST list)                   const { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, GETLASTPARAM,    list));        }
    inline HPARAM list_getParamPrev(      HLIST list, HPARAM param)     const { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, GETPREVPARAM,    list, param)); }
    inline HPARAM list_getParamNext(      HLIST list, HPARAM param)     const { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, GETNEXTPARAM,    list, param)); }
    inline HPARAM list_getParamHandle(    HLIST list, const char *name) const { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, GETPARAMDATA,    list, name )); }
    inline HPARAM list_getParamById(      HLIST list, __int64 id)       const { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, GET_PARAM_BY_ID, list, id   )); }

	inline __int64 list_getParamId(HLIST list, const char *param) const          { RmxLng((VMSG::CONFIG_BASE, GET_PARAM_ID, list       )); } 
	inline void    list_setParamId(HLIST list, const char *param, __int64 newid) { RmxNul((VMSG::CONFIG_BASE, GET_PARAM_ID, list, newid)); } 

    //some other param stuff
    inline UINT   list_getParamFlags(  HLIST list, const char *param)       const { RmxInt(        (VMSG::CONFIG_BASE, GETPARAMFLAGS,    list, param));        }
    inline BOOL   list_isParamSavable( HLIST list, const char *param)       const { RmxInt(        (VMSG::CONFIG_BASE, ISPARAMSAVABLE,   list, param));        }
    inline BOOL   list_isParamReadOnly(HLIST list, const char *param)       const { RmxInt(        (VMSG::CONFIG_BASE, ISPARAMREADONLY,  list, param));        }
    inline BOOL   list_isParamUserData(HLIST list, HPARAM      param)       const { RmxInt(        (VMSG::CONFIG_BASE, ISPARAMUSERDATA,  list, param));        }
    inline void   list_setParamSavable(HLIST list, const char *param, BOOL save)  { RmxNul(        (VMSG::CONFIG_BASE, SETPARAMSAVABLE,  list, param, save));  }
    inline HPARAM list_newParam(HLIST list, const char *param, const char *value) { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, NEWEXTERNALPARAM, list, param, value)); }
    inline void   list_removeParam(    HLIST list, HPARAM      param)             { RmxNul(        (VMSG::CONFIG_BASE, REMOVEPARAM,      list, param));        }
    inline void   list_removeParam(    HLIST list, const char *param)             { RmxNul(        (VMSG::CONFIG_BASE, REMOVEPARAMSTR,   list, param));        }
    inline void   list_removeAllParams(HLIST list)                                { RmxNul(        (VMSG::CONFIG_BASE, REMOVEALLPARAMS,  list));               }
/*
    //move param
    inline void   list_moveParam(HLIST list, HPARAM      param, HPARAM      insert) { RmxNul((VMSG::CONFIG_BASE, MOVEPARAM,    list, param, insert)); }
    inline void   list_moveParam(HLIST list, const char *param, const char *insert) { RmxNul((VMSG::CONFIG_BASE, MOVEPARAMSTR, list, param, insert)); }
*/
    //list hierarchy (parent info, child info, etc)
    inline HLIST list_getParent(     HLIST list)                                                              const { RmxPtr(HLIST, (VMSG::CONFIG_BASE, GETPARENT,         list));                        }
    inline HLIST list_getChild(      HLIST list, const char *name, const char *type, xmlParamList *paramlist) const { RmxPtr(HLIST, (VMSG::CONFIG_BASE, GETCHILD,          list, name, type, paramlist)); }
    inline HLIST list_getChildById(  HLIST list, RMXLONG id, BOOL recursive=FALSE)                            const { RmxPtr(HLIST, (VMSG::CONFIG_BASE, GETCHILDBYID,      list, id,    recursive));      }
    inline BOOL  list_hasChild(      HLIST list, HLIST child, BOOL recursive=FALSE)                           const { RmxInt(       (VMSG::CONFIG_BASE, HASCHILD,          list, child, recursive));      }
    inline BOOL  list_hasChildCached(HLIST list, HLIST child)                                                 const { RmxInt(       (VMSG::CONFIG_BASE, HASCACHEDCHILD,    list, child));                 }
    inline BOOL  list_hasChildren(   HLIST list)                                                              const { RmxInt(       (VMSG::CONFIG_BASE, HASCHILDREN,       list));                        }
    inline int   list_getChildCount( HLIST list)                                                              const { RmxInt(       (VMSG::CONFIG_BASE, GETCHILDCOUNT,     list));                        }
    inline HLIST list_getFirstChild( HLIST list)                                                              const { RmxPtr(HLIST, (VMSG::CONFIG_BASE, GETFIRSTCHILD,     list));                        }
    inline HLIST list_getLastChild(  HLIST list)                                                              const { RmxPtr(HLIST, (VMSG::CONFIG_BASE, GETLASTCHILD,      list));                        }
    inline HLIST list_getPrevChild(  HLIST list, HLIST child)                                                 const { RmxPtr(HLIST, (VMSG::CONFIG_BASE, GETPREVCHILD,      list, child));                 }
    inline HLIST list_getNextChild(  HLIST list, HLIST child)                                                 const { RmxPtr(HLIST, (VMSG::CONFIG_BASE, GETNEXTCHILD,      list, child));                 }
    inline HLIST list_newChild(      HLIST list, const char *name, const char *type, xmlParamList *paramlist)       { RmxPtr(HLIST, (VMSG::CONFIG_BASE, NEWEXTERNALCHILD,  list, name, type, paramlist)); }
    inline void  list_removeAllChildren(HLIST list)                                                                 { RmxNul(       (VMSG::CONFIG_BASE, REMOVEALLCHILDREN, list));                        }

    inline void list_moveUp(      HLIST list) { RmxNul((VMSG::CONFIG_BASE, MOVELISTUP,       list)); }
    inline void list_moveDown(    HLIST list) { RmxNul((VMSG::CONFIG_BASE, MOVELISTDOWN,     list)); }
    inline void list_moveBegining(HLIST list) { RmxNul((VMSG::CONFIG_BASE, MOVELISTBEGINING, list)); }
    inline void list_moveEnd(     HLIST list) { RmxNul((VMSG::CONFIG_BASE, MOVELISTEND,      list)); }
    inline void list_moveChild(   HLIST list, HLIST child, HLIST insert) { RmxNul((VMSG::CONFIG_BASE, MOVECHILD, list, child, insert)); }


//---------------------------------------------------------------------------------------------------------------


    //todo: change "set security ex" to "append" and "remove" security, as to not over-write pending operations
    //get info about a specific param
    inline const char *param_getName(      HPARAM param)         const  { RmxStr((VMSG::PARAM_BASE, GETPRMNAME,       param)       ); }
    inline UINT        param_getFlags(     HPARAM param)         const  { RmxInt((VMSG::PARAM_BASE, GETPRMFLAGS,      param)       ); }
	inline __int64     param_getId(        HPARAM param)         const  { RmxLng((VMSG::PARAM_BASE, GET_PRM_ID,       param)       ); }
	inline void        param_setId(        HPARAM param, __int64 newid) { RmxNul((VMSG::PARAM_BASE, SET_PRM_ID,       param, newid)); }
    inline UINT        param_getSecurity(  HPARAM param)         const  { RmxInt((VMSG::PARAM_BASE, GETPRMSECURITY,   param)       ); }
    inline UINT        param_getSecurityEx(HPARAM param)         const  { RmxInt((VMSG::PARAM_BASE, GETPRMSECURITYEX, param)       ); }
    inline void        param_setSecurityEx(HPARAM param, int ex)        { RmxNul((VMSG::PARAM_BASE, SETPRMSECURITYEX, param, ex)   ); }

    //get value of a specific param
    inline const char *param_getValue(      HPARAM param) const { RmxStr((VMSG::PARAM_BASE, GETPRMVALUE,       param)); }
    inline int         param_getValueInt(   HPARAM param) const { RmxInt((VMSG::PARAM_BASE, GETPRMVALUEINT,    param)); }
    inline RMXLONG     param_getValueLong(  HPARAM param) const { RmxLng((VMSG::PARAM_BASE, GETPRMVALUELONG,   param)); }
    inline double      param_getValueNumber(HPARAM param) const { RmxNum((VMSG::PARAM_BASE, GETPRMVALUENUMBER, param)); }
    inline GUID        param_getValueGuid(  HPARAM param) const { RmxUid((VMSG::PARAM_BASE, GETPRMVALUEGUID,   param)); }
    inline FOURCC      param_getValueFourCC(HPARAM param) const { RmxFcc((VMSG::PARAM_BASE, GETPRMVALUEFOURCC, param)); }

    //set value of a specific param
    inline void param_setValue(      HPARAM param, const char *value) { RmxNul((VMSG::PARAM_BASE, SETPRMVALUE,       param, value)); }
    inline void param_setValueString(HPARAM param, const char *value) { RmxNul((VMSG::PARAM_BASE, SETPRMVALUESTRING, param, value)); }
    inline void param_setValueInt(   HPARAM param, int         value) { RmxNul((VMSG::PARAM_BASE, SETPRMVALUEINT,    param, value)); }
    inline void param_setValueLong(  HPARAM param, RMXLONG     value) { RmxNul((VMSG::PARAM_BASE, SETPRMVALUELONG,   param, value)); }
    inline void param_setValueNumber(HPARAM param, double      value) { RmxNul((VMSG::PARAM_BASE, SETPRMVALUENUMBER, param, value)); }
    inline void param_setValueGuid(  HPARAM param, GUID        value) { RmxNul((VMSG::PARAM_BASE, SETPRMVALUEGUID,   param, value)); }
    inline void param_setValueFourCC(HPARAM param, FOURCC      value) { RmxNul((VMSG::PARAM_BASE, SETPRMVALUEFOURCC, param, value)); }

    //check data type of specific param
    inline BOOL param_isNull(   HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMNULL,   param)); }
    inline BOOL param_isString( HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMSTRING, param)); }
    inline BOOL param_isInteger(HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMINT,    param)); }
    inline BOOL param_isLong(   HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMLONG,   param)); }
    inline BOOL param_isNumber( HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMNUMBER, param)); }
    inline BOOL param_isGuid(   HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMGUID,   param)); }
    inline BOOL param_isFourCC( HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMFOURCC, param)); }
    
    //is the param savable
    inline BOOL param_isSavable( HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMSAVABLE,  param)); }
    inline BOOL param_isReadOnly(HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMREADONLY, param)); }



    //register a callback with a list
    void insertCallback(HLIST list, HPLUGIN src, void *callback);
    void removeCallback(void *callback);
    plugCallback *getCallback(void *callback);


//---------------------------------------------------------------------------------------------------------------



  protected:
    virtual void RmxMsg(vMSG *msg) const;
    virtual void RmxMsg(UINT type, UINT msg) const;
    virtual void RmxMsg(UINT type, UINT msg, int p0) const;

    virtual void RmxMsg(UINT type, UINT msg, const void *p0) const;
    virtual void RmxMsg(UINT type, UINT msg, const void *p0, const void *p1) const;
    virtual void RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, const void *p2) const;
    virtual void RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, const void *p2, const void *p3) const;

    virtual void RmxMsg(UINT type, UINT msg, const void *p0, int p1) const;
    virtual void RmxMsg(UINT type, UINT msg, const void *p0, RMXLONG p1) const;
    virtual void RmxMsg(UINT type, UINT msg, const void *p0, double p1) const;

    virtual void RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, RMXLONG p2) const;
    virtual void RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, double p2) const;
    virtual void RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, int p2) const;
    virtual void RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, GUID p23) const;

    virtual void RmxMsg(UINT type, UINT msg, const void *p0, int p1, int p2, const void *p3) const;
    virtual void RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, int p2, const void *p3) const;
    virtual void RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, RMXLONG p2, RMXLONG p3, RMXLONG p4) const;

    virtual void RmxMsg(UINT type, UINT msg, const void *p0, RMXLONG p1, int p2) const;

    virtual void RmxMsg(UINT type, UINT msg, const void *p0, FOURCC p1) const;
    virtual void RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, FOURCC p2) const;


    //to downsize the total number of messages needed, some we just inline that are similar
    inline  void RmxMsg(UINT type, UINT msg, const void *p0, GUID p23)   const { RmxMsg(type, msg, p0, NULL, p23); }

    inline void setEntry(RMX_ENTRY newentry) { entry = newentry; }

  protected:
    mutable vMSG extrmxmsg;

    rmxString path;
    rmxString filename;

    HMODULE hMod;
    RMX_ENTRY entry;
    rmxList<plugCallback*> cblist;
};


#endif //__PLUGIN_H__
