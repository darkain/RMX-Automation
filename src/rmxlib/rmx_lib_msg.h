/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RMX_LIB_MSG_
#define _RMX_LIB_MSG_


namespace VMSG {
  enum MESSAGES {
    MSG_ERROR,
    SYSTEM,
    CONFIG_BASE,
    CALLBACK_CORE,
    CALLBACK_REMOTE,
    PARAM_BASE,
    FUNCTION_BASE,

    XML_PARAM_LIST,

//    CONSOLE_CALLBACK,  //obsolete this
//    ENCODER_CALLBACK,
//    XML_READER,
//    XML_PARAMS,
//    XML_WRITER,
  };
};


enum MSG_CONFIG {
  CONFIGNOTHING,
  DELETELIST,

  ONMESSAGE,

  GETNAME,
  SETNAME,
  GETTYPE,
  GET_LIST_ID,
  SET_LIST_ID,
  SETTOOLTIP_OBSOLETE,

  GETLISTFLAGS,
  GETLISTSECURITY,
  GETLISTSECURITYEX,
  SETLISTSECURITYEX,

  ISENABLED,
  SETENABLED,
  HASENABLEDCHILDREN,
  ISVISIBLE,
  SETVISIBLE,
  HASVISIBLECHILDREN,
  ISEXPANDED,
  SETEXPANDED,
  HASEXPANDEDCHILDREN,
  ISHIGHLIGHTED,
  SETHIGHLIGHTED,
  HASHIGHLIGHTEDCHILDREN,
  ISSAVABLE,
  SETSAVABLE,
  HASSAVABLECHILDREN,
  ISREADONLY,
  SETREADONLY,
  HASREADONLYCHILDREN,

  GET_LIST_PARAM,
  
  GETPARAM,
  GETPARAMSAFE,
  GETPARAMINT,
  GETPARAMLONG,
  GETPARAMNUMBER,
  GETPARAMGUID,
  GETPARAMFOURCC,

  SETPARAM,
  SETPARAMSTRING,
  SETPARAMINT,
  SETPARAMLONG,
  SETPARAMNUMBER,
  SETPARAMGUID,
  SETPARAMFOURCC,

  HASPARAM,
  HASPARAMSTR,
  HASPARAMS,
/*
  INSERTPARAM,
  INSERTPARAMINT,
  INSERTPARAMNOCFG,
  INSERTPARAMINTNOCFG,
*/
  CANENUMPARAMS,
  GETPARAMCOUNT,
  GETFIRSTPARAM,
  GETLASTPARAM,
  GETPREVPARAM,
  GETNEXTPARAM,
  GETPARAMDATA,
  GET_PARAM_BY_ID,
  GET_PARAM_ID,
  SET_PARAM_ID,
  GETPARAMFLAGS,
  ISPARAMSAVABLE,
  ISPARAMREADONLY,
  ISPARAMUSERDATA,
  SETPARAMSAVABLE,
  NEWEXTERNALPARAM,
  REMOVEPARAM,
  REMOVEPARAMSTR,
  REMOVEALLPARAMS,
/*
  MOVEPARAM,
  MOVEPARAMSTR,
*/
  GETPARENT,
  GETCHILD,
  GETCHILDBYID,
  HASCHILD,
  HASCACHEDCHILD,
  HASCHILDREN,
  GETCHILDCOUNT,
  GETFIRSTCHILD,
  GETLASTCHILD,
  GETNEXTCHILD,
  GETPREVCHILD,
  NEWEXTERNALCHILD,
  REMOVEALLCHILDREN,

  MOVELISTUP,
  MOVELISTDOWN,
  MOVELISTBEGINING,
  MOVELISTEND,
  MOVECHILD,

  FIRSTLOAD,

  INSERTCALLBACK,
  REMOVECALLBACK,
};


enum MSG_CALLBACK_CORE {
  MSG_CB_CORE_NOTHING,

  MSG_CB_ON_DELETE,

  MSG_CB_MESSAGE_TEXT,
  MSG_CB_MESSAGE_INT,
  MSG_CB_MESSAGE_CONFIG,
  MSG_CB_MESSAGE_PARAM,
};


enum MSG_CALLBACK_CONFIG {
  CBCONFIGNOTHING,
  CBCFGINSERT,
  CBCFGREMOVE,
};


enum MSG_PARAM {
  PARAMNOTHING,
  ISPRMNAMED,
  GETPRMNAME,

  GET_PRM_ID,
  SET_PRM_ID,

  GETPRMFLAGS,
  GETPRMSECURITY,
  GETPRMSECURITYEX,
  SETPRMSECURITYEX,

  GETPRMVALUE,
  GETPRMVALUEINT,
  GETPRMVALUELONG,
  GETPRMVALUENUMBER,
  GETPRMVALUEGUID,
  GETPRMVALUEFOURCC,

  SETPRMVALUE,
  SETPRMVALUESTRING,
  SETPRMVALUEINT,
  SETPRMVALUELONG,
  SETPRMVALUENUMBER,
  SETPRMVALUEGUID,
  SETPRMVALUEFOURCC,

  ISPRMNULL,
  ISPRMSTRING,
  ISPRMINT,
  ISPRMLONG,
  ISPRMNUMBER,
  ISPRMGUID,
  ISPRMFOURCC,

  ISPRMSAVABLE,
  ISPRMREADONLY,
};


enum VXMLPARAMLIST {
  XMLPRMNOTHING,
  XMLPRMGETITEM,

  XMLPRMGETFIRST,
  XMLPRMGETLAST,
  XMLPRMGETPREV,
  XMLPRMGETNEXT,
};


/*
enum VXMLPARSER {
  XMLNOTHING,
  XMLCREATE,
  XMLDESTROY,
  XMLELEMENTSTART,
  XMLELEMENTEND,
  XMLCHARACTERDATA,
  XMLREADERROR,
  XMLGETFILELINE,
  XMLPARSEFILE,
};


enum VXMLPARAM {
  XPRMNOTHING,
  XPRMGETNAME,
  XPRMGETVALUE,
  XPRMGETVALUEID,
  XPRMGETVALUEINT,
  XPRMGETCOUNT,
};
*/

#endif //_RMX_LIB_MSG_H_
