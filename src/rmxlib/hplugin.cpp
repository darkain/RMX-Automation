/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#define RMX_LIB


#include "../../sdk/rmxBase.h"
#include "../../sdk/handles/hplugin.h"
#include "rmx_msg.h"


//--------------------------------------------
//HPLUGIN info related functions
//--------------------------------------------
HPLUGIN vHPLUGIN::getFirst()                          { RmxPtr( HPLUGIN, (VMSG::SYSTEM, 200          ) ); }
HPLUGIN vHPLUGIN::getLast()                           { RmxPtr( HPLUGIN, (VMSG::SYSTEM, 201          ) ); }
HPLUGIN vHPLUGIN::getPrev(HPLUGIN plugin)             { RmxPtr( HPLUGIN, (VMSG::SYSTEM, 203, plugin  ) ); }
HPLUGIN vHPLUGIN::getNext(HPLUGIN plugin)             { RmxPtr( HPLUGIN, (VMSG::SYSTEM, 202, plugin  ) ); }
HPLUGIN vHPLUGIN::getMaster()                         { RmxPtr( HPLUGIN, (VMSG::SYSTEM, 207          ) ); }
HPLUGIN vHPLUGIN::getByName(    const char *name)     { RmxPtr( HPLUGIN, (VMSG::SYSTEM, 204, name    ) ); }
HPLUGIN vHPLUGIN::getByFilename(const char *filename) { RmxPtr( HPLUGIN, (VMSG::SYSTEM, 205, filename) ); }
HPLUGIN vHPLUGIN::loadPlugin(   const char *filename) { RmxPtr( HPLUGIN, (VMSG::SYSTEM, 206, filename) ); }

BOOL vHPLUGIN::isValid()    const { RmxInt( (VMSG::SYSTEM, 210, const_cast<vHPLUGIN*>(this)) ); }
BOOL vHPLUGIN::isMaster()   const { RmxInt( (VMSG::SYSTEM, 211, const_cast<vHPLUGIN*>(this)) ); }
BOOL vHPLUGIN::unload()           { RmxInt( (VMSG::SYSTEM, 217, const_cast<vHPLUGIN*>(this)) ); }

const char *vHPLUGIN::getPath()     const { RmxStr( (VMSG::SYSTEM, 218, const_cast<vHPLUGIN*>(this)) ); }
const char *vHPLUGIN::getFilename() const { RmxStr( (VMSG::SYSTEM, 219, const_cast<vHPLUGIN*>(this)) ); }


//--------------------------------------------
//HLIST related functions
//--------------------------------------------

HLIST vHPLUGIN::list_getRoot() const { RmxPtr( HLIST, (VMSG::SYSTEM, 11, const_cast<vHPLUGIN*>(this)) ); }

RMXLONG vHPLUGIN::list_message(HLIST list, RMXLONG message, RMXLONG param1, RMXLONG param2) { RmxLng( (VMSG::CONFIG_BASE, ONMESSAGE, const_cast<vHPLUGIN*>(this), list, message, param1, param2) ); }


    //primary list information
const char *vHPLUGIN::list_getName(   HLIST list) const                { RmxStr((VMSG::CONFIG_BASE, GETNAME,     const_cast<vHPLUGIN*>(this), list)); }
void        vHPLUGIN::list_setName(   HLIST list, const char *name)    { RmxNul((VMSG::CONFIG_BASE, SETNAME,     const_cast<vHPLUGIN*>(this), list, name)); }
const char *vHPLUGIN::list_getType(   HLIST list) const                { RmxStr((VMSG::CONFIG_BASE, GETTYPE,     const_cast<vHPLUGIN*>(this), list)); }
__int64     vHPLUGIN::list_getId(     HLIST list) const                { RmxLng((VMSG::CONFIG_BASE, GET_LIST_ID, const_cast<vHPLUGIN*>(this), list)); }
void        vHPLUGIN::list_setId(     HLIST list, __int64 newid)       { RmxNul((VMSG::CONFIG_BASE, SET_LIST_ID, const_cast<vHPLUGIN*>(this), list, newid)); }

void vHPLUGIN::list_onFirstLoad(HLIST list) {RmxNul((VMSG::CONFIG_BASE, FIRSTLOAD, const_cast<vHPLUGIN*>(this), list));  }


    // list properties and access rights
UINT vHPLUGIN::list_getFlags(              HLIST list)                       const { RmxInt((VMSG::CONFIG_BASE, GETLISTFLAGS,           const_cast<vHPLUGIN*>(this), list));            }
UINT vHPLUGIN::list_getSecurity(           HLIST list)                       const { RmxInt((VMSG::CONFIG_BASE, GETLISTSECURITY,        const_cast<vHPLUGIN*>(this), list));            }
UINT vHPLUGIN::list_getSecurityEx(         HLIST list)                       const { RmxInt((VMSG::CONFIG_BASE, GETLISTSECURITYEX,      const_cast<vHPLUGIN*>(this), list));            }
    

    // list properties and access rights - specifics
BOOL vHPLUGIN::list_isVisible(             HLIST list)                 const { RmxInt((VMSG::CONFIG_BASE, ISVISIBLE,              const_cast<vHPLUGIN*>(this), list));            }
void vHPLUGIN::list_setVisible(            HLIST list, BOOL visible)         { RmxNul((VMSG::CONFIG_BASE, SETVISIBLE,             const_cast<vHPLUGIN*>(this), list, visible));   }
BOOL vHPLUGIN::list_hasChildrenVisible(    HLIST list, BOOL recursive) const { RmxInt((VMSG::CONFIG_BASE, HASVISIBLECHILDREN,     const_cast<vHPLUGIN*>(this), list, recursive)); }
BOOL vHPLUGIN::list_isEnabled(             HLIST list)                 const { RmxInt((VMSG::CONFIG_BASE, ISENABLED,              const_cast<vHPLUGIN*>(this), list));            }
void vHPLUGIN::list_setEnabled(            HLIST list, BOOL enable)          { RmxNul((VMSG::CONFIG_BASE, SETENABLED,             const_cast<vHPLUGIN*>(this), list, enable));    }
BOOL vHPLUGIN::list_hasChildrenEnabled(    HLIST list, BOOL recursive) const { RmxInt((VMSG::CONFIG_BASE, HASENABLEDCHILDREN,     const_cast<vHPLUGIN*>(this), list, recursive)); }
BOOL vHPLUGIN::list_isHighlighted(         HLIST list)                 const { RmxInt((VMSG::CONFIG_BASE, ISHIGHLIGHTED,          const_cast<vHPLUGIN*>(this), list));            }
void vHPLUGIN::list_setHighlighted(        HLIST list, BOOL highlight)       { RmxNul((VMSG::CONFIG_BASE, SETHIGHLIGHTED,         const_cast<vHPLUGIN*>(this), list, highlight)); }
BOOL vHPLUGIN::list_hasChildrenHighlighted(HLIST list, BOOL recursive) const { RmxInt((VMSG::CONFIG_BASE, HASHIGHLIGHTEDCHILDREN, const_cast<vHPLUGIN*>(this), list, recursive)); }
BOOL vHPLUGIN::list_isExpanded(            HLIST list)                 const { RmxInt((VMSG::CONFIG_BASE, ISEXPANDED,             const_cast<vHPLUGIN*>(this), list));            }
void vHPLUGIN::list_setExpanded(           HLIST list, BOOL expanded)        { RmxNul((VMSG::CONFIG_BASE, SETEXPANDED,            const_cast<vHPLUGIN*>(this), list, expanded));  }
BOOL vHPLUGIN::list_hasChildrenExpanded(   HLIST list, BOOL recursive) const { RmxInt((VMSG::CONFIG_BASE, HASEXPANDEDCHILDREN,    const_cast<vHPLUGIN*>(this), list, recursive)); }
BOOL vHPLUGIN::list_isSavable(             HLIST list)                 const { RmxInt((VMSG::CONFIG_BASE, ISSAVABLE,              const_cast<vHPLUGIN*>(this), list));            }
void vHPLUGIN::list_setSavable(            HLIST list, BOOL savable)         { RmxNul((VMSG::CONFIG_BASE, SETSAVABLE,             const_cast<vHPLUGIN*>(this), list, savable));   }
BOOL vHPLUGIN::list_hasChildrenSavable(    HLIST list, BOOL recursive) const { RmxInt((VMSG::CONFIG_BASE, HASSAVABLECHILDREN,     const_cast<vHPLUGIN*>(this), list, recursive)); }
BOOL vHPLUGIN::list_isReadOnly(            HLIST list)                 const { RmxInt((VMSG::CONFIG_BASE, ISREADONLY,             const_cast<vHPLUGIN*>(this), list));            }
BOOL vHPLUGIN::list_hasChildrenReadOnly(   HLIST list, BOOL recursive) const { RmxInt((VMSG::CONFIG_BASE, HASREADONLYCHILDREN,    const_cast<vHPLUGIN*>(this), list, recursive)); }


HPARAM vHPLUGIN::list_getListParam(HLIST list) const { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, GET_LIST_PARAM, const_cast<vHPLUGIN*>(this), list)); }

    //get param value from a list
const char *vHPLUGIN::list_getParam(      HLIST list, const char *name)                   const { RmxStr((VMSG::CONFIG_BASE, GETPARAM,       const_cast<vHPLUGIN*>(this), list, name));        }
const char *vHPLUGIN::list_getParamSafe(  HLIST list, const char *name, const char *safe) const { RmxSaf((VMSG::CONFIG_BASE, GETPARAMSAFE,   const_cast<vHPLUGIN*>(this), list, name, safe));  }
int         vHPLUGIN::list_getParamInt(   HLIST list, const char *name)                   const { RmxInt((VMSG::CONFIG_BASE, GETPARAMINT,    const_cast<vHPLUGIN*>(this), list, name));        }
RMXLONG     vHPLUGIN::list_getParamLong(  HLIST list, const char *name)                   const { RmxLng((VMSG::CONFIG_BASE, GETPARAMLONG,   const_cast<vHPLUGIN*>(this), list, name));        }
double      vHPLUGIN::list_getParamNumber(HLIST list, const char *name)                   const { RmxNum((VMSG::CONFIG_BASE, GETPARAMNUMBER, const_cast<vHPLUGIN*>(this), list, name));        }
GUID        vHPLUGIN::list_getParamGuid(  HLIST list, const char *name)                   const { RmxUid((VMSG::CONFIG_BASE, GETPARAMGUID,   const_cast<vHPLUGIN*>(this), list, name));        }
FOURCC      vHPLUGIN::list_getParamFourCC(HLIST list, const char *name)                   const { RmxFcc((VMSG::CONFIG_BASE, GETPARAMFOURCC, const_cast<vHPLUGIN*>(this), list, name));        }

    //set param value from a list
void vHPLUGIN::list_setParam(      HLIST list, const char *name, const char *value) { RmxNul((VMSG::CONFIG_BASE, SETPARAM,       const_cast<vHPLUGIN*>(this), list, name, value)); }
void vHPLUGIN::list_setParamString(HLIST list, const char *name, const char *value) { RmxNul((VMSG::CONFIG_BASE, SETPARAMSTRING, const_cast<vHPLUGIN*>(this), list, name, value)); }
void vHPLUGIN::list_setParamInt(   HLIST list, const char *name, int         value) { RmxNul((VMSG::CONFIG_BASE, SETPARAMINT,    const_cast<vHPLUGIN*>(this), list, name, value)); }
void vHPLUGIN::list_setParamLong(  HLIST list, const char *name, RMXLONG     value) { RmxNul((VMSG::CONFIG_BASE, SETPARAMLONG,   const_cast<vHPLUGIN*>(this), list, name, value)); }
void vHPLUGIN::list_setParamNumber(HLIST list, const char *name, double      value) { RmxNul((VMSG::CONFIG_BASE, SETPARAMNUMBER, const_cast<vHPLUGIN*>(this), list, name, value)); }
void vHPLUGIN::list_setParamGuid(  HLIST list, const char *name, GUID        value) { RmxNul((VMSG::CONFIG_BASE, SETPARAMGUID,   const_cast<vHPLUGIN*>(this), list, name, value)); }
void vHPLUGIN::list_setParamFourCC(HLIST list, const char *name, FOURCC      value) { RmxNul((VMSG::CONFIG_BASE, SETPARAMFOURCC, const_cast<vHPLUGIN*>(this), list, name, value)); }

    //does the list have a specific param?
BOOL vHPLUGIN::list_hasParam( HLIST list, HPARAM param)      const { RmxInt((VMSG::CONFIG_BASE, HASPARAM,    const_cast<vHPLUGIN*>(this), list, param)); }
BOOL vHPLUGIN::list_hasParam( HLIST list, const char *param) const { RmxInt((VMSG::CONFIG_BASE, HASPARAMSTR, const_cast<vHPLUGIN*>(this), list, param)); }
BOOL vHPLUGIN::list_hasParams(HLIST list)                    const { RmxInt((VMSG::CONFIG_BASE, HASPARAMS,   const_cast<vHPLUGIN*>(this), list));        }

    //add params to a list
//HPARAM vHPLUGIN::list_insertParam(      HLIST list, const char *name, HPARAM insert) { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, INSERTPARAM,         const_cast<vHPLUGIN*>(this), list, name, insert)); }
//HPARAM vHPLUGIN::list_insertParamNoSave(HLIST list, const char *name, HPARAM insert) { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, INSERTPARAMNOCFG,    const_cast<vHPLUGIN*>(this), list, name, insert)); }

    //get param data from a list
BOOL   vHPLUGIN::list_canEnumerateParams(HLIST list)                   const { RmxInt(        (VMSG::CONFIG_BASE, CANENUMPARAMS,   const_cast<vHPLUGIN*>(this), list));        }
int    vHPLUGIN::list_getParamCount(     HLIST list)                   const { RmxInt(        (VMSG::CONFIG_BASE, GETPARAMCOUNT,   const_cast<vHPLUGIN*>(this), list));        }
HPARAM vHPLUGIN::list_getParamFirst(     HLIST list)                   const { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, GETFIRSTPARAM,   const_cast<vHPLUGIN*>(this), list));        }
HPARAM vHPLUGIN::list_getParamLast(      HLIST list)                   const { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, GETLASTPARAM,    const_cast<vHPLUGIN*>(this), list));        }
HPARAM vHPLUGIN::list_getParamPrev(      HLIST list, HPARAM param)     const { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, GETPREVPARAM,    const_cast<vHPLUGIN*>(this), list, param)); }
HPARAM vHPLUGIN::list_getParamNext(      HLIST list, HPARAM param)     const { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, GETNEXTPARAM,    const_cast<vHPLUGIN*>(this), list, param)); }
HPARAM vHPLUGIN::list_getParamHandle(    HLIST list, const char *name) const { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, GETPARAMDATA,    const_cast<vHPLUGIN*>(this), list, name )); }
HPARAM vHPLUGIN::list_getParamById(      HLIST list, __int64 id)       const { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, GET_PARAM_BY_ID, const_cast<vHPLUGIN*>(this), list, id   )); }

    //some other param stuff
UINT   vHPLUGIN::list_getParamFlags(  HLIST list, const char *param)       const { RmxInt(        (VMSG::CONFIG_BASE, GETPARAMFLAGS,    const_cast<vHPLUGIN*>(this), list, param));        }
BOOL   vHPLUGIN::list_isParamSavable( HLIST list, const char *param)       const { RmxInt(        (VMSG::CONFIG_BASE, ISPARAMSAVABLE,   const_cast<vHPLUGIN*>(this), list, param));        }
BOOL   vHPLUGIN::list_isParamReadOnly(HLIST list, const char *param)       const { RmxInt(        (VMSG::CONFIG_BASE, ISPARAMREADONLY,  const_cast<vHPLUGIN*>(this), list, param));        }
BOOL   vHPLUGIN::list_isParamUserData(HLIST list, HPARAM      param)       const { RmxInt(        (VMSG::CONFIG_BASE, ISPARAMUSERDATA,  const_cast<vHPLUGIN*>(this), list, param));        }
void   vHPLUGIN::list_setParamSavable(HLIST list, const char *param, BOOL save)  { RmxNul(        (VMSG::CONFIG_BASE, SETPARAMSAVABLE,  const_cast<vHPLUGIN*>(this), list, param, save));  }
HPARAM vHPLUGIN::list_newParam(HLIST list, const char *param, const char *value) { RmxPtr(HPARAM, (VMSG::CONFIG_BASE, NEWEXTERNALPARAM, const_cast<vHPLUGIN*>(this), list, param, value)); }
void   vHPLUGIN::list_removeParam(    HLIST list, HPARAM      param)             { RmxNul(        (VMSG::CONFIG_BASE, REMOVEPARAM,      const_cast<vHPLUGIN*>(this), list, param));        }
void   vHPLUGIN::list_removeParam(    HLIST list, const char *param)             { RmxNul(        (VMSG::CONFIG_BASE, REMOVEPARAMSTR,   const_cast<vHPLUGIN*>(this), list, param));        }
void   vHPLUGIN::list_removeAllParams(HLIST list)                                { RmxNul(        (VMSG::CONFIG_BASE, REMOVEALLPARAMS,  const_cast<vHPLUGIN*>(this), list));               }

    //move param
//void   vHPLUGIN::list_moveParam(HLIST list, HPARAM      param, HPARAM      insert) { RmxNul((VMSG::CONFIG_BASE, MOVEPARAM,    const_cast<vHPLUGIN*>(this), list, param, insert)); }
//void   vHPLUGIN::list_moveParam(HLIST list, const char *param, const char *insert) { RmxNul((VMSG::CONFIG_BASE, MOVEPARAMSTR, const_cast<vHPLUGIN*>(this), list, param, insert)); }

    //list hierarchy (parent info, child info, etc)
HLIST vHPLUGIN::list_getParent(     HLIST list)                                     const { RmxPtr(HLIST, (VMSG::CONFIG_BASE, GETPARENT,         const_cast<vHPLUGIN*>(this), list));                   }
//TODO: (void*)NULL << const_cast<vHPLUGIN*>(this) will be filled in later.  keep as NULL for the time being for backwrd/forward compatibility
HLIST vHPLUGIN::list_getChild(      HLIST list, const char *name, const char *type) const { RmxPtr(HLIST, (VMSG::CONFIG_BASE, GETCHILD,          const_cast<vHPLUGIN*>(this), list, name, type, (void*)NULL));      }
HLIST vHPLUGIN::list_getChildById(  HLIST list, RMXLONG id, BOOL recursive)         const { RmxPtr(HLIST, (VMSG::CONFIG_BASE, GETCHILDBYID,      const_cast<vHPLUGIN*>(this), list, id,    recursive)); }
BOOL  vHPLUGIN::list_hasChild(      HLIST list, HLIST child, BOOL recursive)        const { RmxInt(       (VMSG::CONFIG_BASE, HASCHILD,          const_cast<vHPLUGIN*>(this), list, child, recursive)); }
BOOL  vHPLUGIN::list_hasChildCached(HLIST list, HLIST child)                        const { RmxInt(       (VMSG::CONFIG_BASE, HASCACHEDCHILD,    const_cast<vHPLUGIN*>(this), list, child));            }
BOOL  vHPLUGIN::list_hasChildren(   HLIST list)                                     const { RmxInt(       (VMSG::CONFIG_BASE, HASCHILDREN,       const_cast<vHPLUGIN*>(this), list));                   }
int   vHPLUGIN::list_getChildCount( HLIST list)                                     const { RmxInt(       (VMSG::CONFIG_BASE, GETCHILDCOUNT,     const_cast<vHPLUGIN*>(this), list));                   }
HLIST vHPLUGIN::list_getFirstChild( HLIST list)                                     const { RmxPtr(HLIST, (VMSG::CONFIG_BASE, GETFIRSTCHILD,     const_cast<vHPLUGIN*>(this), list));                   }
HLIST vHPLUGIN::list_getLastChild(  HLIST list)                                     const { RmxPtr(HLIST, (VMSG::CONFIG_BASE, GETLASTCHILD,      const_cast<vHPLUGIN*>(this), list));                   }
HLIST vHPLUGIN::list_getPrevChild(  HLIST list, HLIST child)                        const { RmxPtr(HLIST, (VMSG::CONFIG_BASE, GETPREVCHILD,      const_cast<vHPLUGIN*>(this), list, child));            }
HLIST vHPLUGIN::list_getNextChild(  HLIST list, HLIST child)                        const { RmxPtr(HLIST, (VMSG::CONFIG_BASE, GETNEXTCHILD,      const_cast<vHPLUGIN*>(this), list, child));            }

//TODO: (void*)NULL << const_cast<vHPLUGIN*>(this) will be filled in later.  keep as NULL for the time being for backwrd/forward compatibility
HLIST vHPLUGIN::list_newExternalChild(HLIST list, const char *name, const char *type) { RmxPtr(HLIST, (VMSG::CONFIG_BASE, NEWEXTERNALCHILD, const_cast<vHPLUGIN*>(this), list, name, type, (void*)NULL) ); }

//TODO: why dont these have const_cast<vHPLUGIN*>(this) ???
void vHPLUGIN::list_moveUp(      HLIST list) { RmxNul((VMSG::CONFIG_BASE, MOVELISTUP,       list)); }
void vHPLUGIN::list_moveDown(    HLIST list) { RmxNul((VMSG::CONFIG_BASE, MOVELISTDOWN,     list)); }
void vHPLUGIN::list_moveBegining(HLIST list) { RmxNul((VMSG::CONFIG_BASE, MOVELISTBEGINING, list)); }
void vHPLUGIN::list_moveEnd(     HLIST list) { RmxNul((VMSG::CONFIG_BASE, MOVELISTEND,      list)); }
void vHPLUGIN::list_moveChild(   HLIST list, HLIST child, HLIST insert) { RmxNul((VMSG::CONFIG_BASE, MOVECHILD, const_cast<vHPLUGIN*>(this), list, child, insert)); }

void vHPLUGIN::list_insertCallback(HLIST list, cbCore *callback) { RmxNul((VMSG::CONFIG_BASE, INSERTCALLBACK, const_cast<vHPLUGIN*>(this), list, callback)); }
void vHPLUGIN::list_removeCallback(HLIST list, cbCore *callback) { RmxNul((VMSG::CONFIG_BASE, REMOVECALLBACK, const_cast<vHPLUGIN*>(this), list, callback)); }


//--------------------------------------------
//HPARAM related functions
//--------------------------------------------

    //get info about a specific param
const char *vHPLUGIN::param_getName(      HPARAM param)          const { RmxStr((VMSG::PARAM_BASE, GETPRMNAME,       const_cast<vHPLUGIN*>(this), param)       ); }
UINT        vHPLUGIN::param_getFlags(     HPARAM param)          const { RmxInt((VMSG::PARAM_BASE, GETPRMFLAGS,      const_cast<vHPLUGIN*>(this), param)       ); }
UINT        vHPLUGIN::param_getSecurity(  HPARAM param)          const { RmxInt((VMSG::PARAM_BASE, GETPRMSECURITY,   const_cast<vHPLUGIN*>(this), param)       ); }
UINT        vHPLUGIN::param_getSecurityEx(HPARAM param)          const { RmxInt((VMSG::PARAM_BASE, GETPRMSECURITYEX, const_cast<vHPLUGIN*>(this), param)       ); }
__int64     vHPLUGIN::param_getId(        HPARAM param)          const { RmxLng((VMSG::PARAM_BASE, GET_PRM_ID,       const_cast<vHPLUGIN*>(this), param)       ); }
void        vHPLUGIN::param_setId(        HPARAM param, __int64 newid) { RmxNul((VMSG::PARAM_BASE, SET_PRM_ID,       const_cast<vHPLUGIN*>(this), param, newid)); }

    //get value of a specific param
const char *vHPLUGIN::param_getValue(      HPARAM param) const { RmxStr((VMSG::PARAM_BASE, GETPRMVALUE,       const_cast<vHPLUGIN*>(this), param)); }
int         vHPLUGIN::param_getValueInt(   HPARAM param) const { RmxInt((VMSG::PARAM_BASE, GETPRMVALUEINT,    const_cast<vHPLUGIN*>(this), param)); }
RMXLONG     vHPLUGIN::param_getValueLong(  HPARAM param) const { RmxLng((VMSG::PARAM_BASE, GETPRMVALUELONG,   const_cast<vHPLUGIN*>(this), param)); }
double      vHPLUGIN::param_getValueNumber(HPARAM param) const { RmxNum((VMSG::PARAM_BASE, GETPRMVALUENUMBER, const_cast<vHPLUGIN*>(this), param)); }
GUID        vHPLUGIN::param_getValueGuid(  HPARAM param) const { RmxUid((VMSG::PARAM_BASE, GETPRMVALUEGUID,   const_cast<vHPLUGIN*>(this), param)); }
FOURCC      vHPLUGIN::param_getValueFourCC(HPARAM param) const { RmxFcc((VMSG::PARAM_BASE, GETPRMVALUEFOURCC, const_cast<vHPLUGIN*>(this), param)); }

const char *vHPLUGIN::param_getValueSafe(  HPARAM param, const char *safe) const { 
	const char *value = param_getValue(param);
	return (value ? value : safe);
}

    //set value of a specific param
void vHPLUGIN::param_setValue(      HPARAM param, const char *value) { RmxNul((VMSG::PARAM_BASE, SETPRMVALUE,       const_cast<vHPLUGIN*>(this), param, value)); }
void vHPLUGIN::param_setValueString(HPARAM param, const char *value) { RmxNul((VMSG::PARAM_BASE, SETPRMVALUESTRING, const_cast<vHPLUGIN*>(this), param, value)); }
void vHPLUGIN::param_setValueInt(   HPARAM param, int         value) { RmxNul((VMSG::PARAM_BASE, SETPRMVALUEINT,    const_cast<vHPLUGIN*>(this), param, value)); }
void vHPLUGIN::param_setValueLong(  HPARAM param, RMXLONG     value) { RmxNul((VMSG::PARAM_BASE, SETPRMVALUELONG,   const_cast<vHPLUGIN*>(this), param, value)); }
void vHPLUGIN::param_setValueNumber(HPARAM param, double      value) { RmxNul((VMSG::PARAM_BASE, SETPRMVALUENUMBER, const_cast<vHPLUGIN*>(this), param, value)); }
void vHPLUGIN::param_setValueGuid(  HPARAM param, GUID        value) { RmxNul((VMSG::PARAM_BASE, SETPRMVALUEGUID,   const_cast<vHPLUGIN*>(this), param, value)); }
void vHPLUGIN::param_setValueFourCC(HPARAM param, FOURCC      value) { RmxNul((VMSG::PARAM_BASE, SETPRMVALUEFOURCC, const_cast<vHPLUGIN*>(this), param, value)); }

    //check data type of specific param
BOOL vHPLUGIN::param_isNull(   HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMNULL,   const_cast<vHPLUGIN*>(this), param)); }
BOOL vHPLUGIN::param_isString( HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMSTRING, const_cast<vHPLUGIN*>(this), param)); }
BOOL vHPLUGIN::param_isInteger(HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMINT,    const_cast<vHPLUGIN*>(this), param)); }
BOOL vHPLUGIN::param_isLong(   HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMLONG,   const_cast<vHPLUGIN*>(this), param)); }
BOOL vHPLUGIN::param_isNumber( HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMNUMBER, const_cast<vHPLUGIN*>(this), param)); }
BOOL vHPLUGIN::param_isGuid(   HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMGUID,   const_cast<vHPLUGIN*>(this), param)); }
BOOL vHPLUGIN::param_isFourCC( HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMFOURCC, const_cast<vHPLUGIN*>(this), param)); }
    
    //is the param savable
BOOL vHPLUGIN::param_isSavable( HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMSAVABLE,  const_cast<vHPLUGIN*>(this), param)); }
BOOL vHPLUGIN::param_isReadOnly(HPARAM param) const { RmxInt((VMSG::PARAM_BASE, ISPRMREADONLY, const_cast<vHPLUGIN*>(this), param)); }


    //some extras
BOOL vHPLUGIN::param_isNamed(HPARAM param, const char *value) const {
  const char *name = param_getName(param);
  if (name) return (!VSTRCMP(name, value));
  return FALSE;
}
