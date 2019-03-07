/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "../../sdk/cb/cbBase.h"
#include "../../sdk/xml/xmlread.h"
#include "../rmxlib/rmx_msg.h"
#include "Loader.h"
#include "Plugin.h"
#include "lua/luaHelper.h"
#include "callbacks/cbConsole.h"
#include "callbacks/cbFunction.h"
#include "callbacks/plugCallback.h"
#include "xml/xmlplugin.h"


extern "C" {
  void __declspec(dllexport) RMXCALL _RMX_CORE(rmxMsg *msg) {
    if (!msg) return;
  
    VMSG::MESSAGES thismsg = (VMSG::MESSAGES)msg->type;

    switch (thismsg) {

      case VMSG::SYSTEM: {
        if (plg == NULL) RETURN;

        switch (msg->msg) {
          case   2:  RETNUL( plg->removePlugin((HMODULE)msg->p0p) );
          case   3:  RETNUL( delete plg->getPlugin((HPLUGIN)msg->p0p) );
          case  11:  RETPTR( (plg->hasPlugin((vPlugin*)msg->p0p)) ? ( ((vPlugin*)msg->p0p)->list_getRoot()  ) : (NULL) );
          case  12:  RETPTR( (plg->hasPlugin((vPlugin*)msg->p0p)) ? ( ((vPlugin*)msg->p0p)->root_getIdent() ) : (NULL) );

          case  30:  RETINT( sizeof(rmxMsg) );

          case 200:  RETPTR( plg->getFirstPlugin() );
          case 201:  RETPTR( plg->getLastPlugin() );
          case 202:  RETPTR( plg->getNextPlugin((vPlugin*)msg->p0p) );
          case 203:  RETPTR( plg->getPrevPlugin((vPlugin*)msg->p0p) );
          case 204:  RETPTR( plg->getPlugin(msg->p0s) );
          case 205:  RETPTR( plg->getPluginByFilename(msg->p0s) );
          case 206:  RETPTR( plg->loadPlugin(msg->p0s) );
          case 207:  RETPTR( plg->getMasterHandle() );

          case 210:  RETINT( plg->hasPlugin(  (vPlugin*)msg->p0p) );
          case 211:  RETINT( plg->isMaster(   (vPlugin*)msg->p0p) );
          case 217:  RETINT( plg->unloadPlugin((HPLUGIN)msg->p0p) );

          case 218:  RETSTR( (plg->hasPlugin((vPlugin*)msg->p0p)) ? ( ((vPlugin*)msg->p0p)->getPath()     ) : (NULL) );
          case 219:  RETSTR( (plg->hasPlugin((vPlugin*)msg->p0p)) ? ( ((vPlugin*)msg->p0p)->getFilename() ) : (NULL) );

#ifdef _RMX_LUA_
          case 301:  RETNUL( lua_script(msg->p0s) );
#endif //_RMX_LUA_

          case 302:  RETNUL( cbCoreConsole::pushConsoleStack(cbCoreConsole::getConsoleCallback((HLIST)msg->p0p)) );
          case 303:  RETPTR( cbCoreConsole::popConsoleStack() );
          case 304:  RETNUL( cbCoreConsole::console_print(msg->p0s, (HCONSOLE)msg->p1p) );
          case 305:  RETNUL( cbCoreConsole::console_clear((HCONSOLE)msg->p0p) );
          case 310:  RETPTR( cbCoreConsole::getFirstCallback() );
          case 311:  RETPTR( cbCoreConsole::getLastCallback()  );
          case 312:  RETPTR( cbCoreConsole::getPrevCallback((cbCoreConsole*)msg->p0p) );
          case 313:  RETPTR( cbCoreConsole::getNextCallback((cbCoreConsole*)msg->p0p) );
          case 320:  RETPTR( cbCoreConsole::console_getPlugin((HCONSOLE)msg->p0p) );
          case 321:  RETPTR( cbCoreConsole::console_getList(  (HCONSOLE)msg->p0p) );

#ifdef _RMX_LUA_
          case 600:  RETINT( cbCoreFunction::global_isValueNull(    msg->p0s) );
          case 601:  RETINT( cbCoreFunction::global_isValueBoolean( msg->p0s) );
          case 602:  RETINT( cbCoreFunction::global_isValueInteger( msg->p0s) );
          case 603:  RETINT( cbCoreFunction::global_isValueNumber(  msg->p0s) );
          case 604:  RETINT( cbCoreFunction::global_isValueString(  msg->p0s) );
          case 611:  RETNUL( cbCoreFunction::global_setValueBoolean(msg->p0s, msg->p1i) );
          case 612:  RETNUL( cbCoreFunction::global_setValueInteger(msg->p0s, msg->p1i) );
          case 613:  RETNUL( cbCoreFunction::global_setValueNumber( msg->p0s, msg->p1d) );
          case 614:  RETNUL( cbCoreFunction::global_setValueString( msg->p0s, msg->p1s) );
          case 621:  RETINT( cbCoreFunction::global_getValueBoolean(msg->p0s) );
          case 622:  RETINT( cbCoreFunction::global_getValueInteger(msg->p0s) );
          case 623:  RETNUM( cbCoreFunction::global_getValueNumber( msg->p0s) );
          case 624:  RETSTR( cbCoreFunction::global_getValueString( msg->p0s) );
#endif //_RMX_LUA_
		} 
      } RETURN;


      case VMSG::CONFIG_BASE: {
        if (plg == NULL) RETURN;

        vPlugin *plugin = plg->getPlugin((HPLUGIN)msg->p0p);
        HLIST list = (HLIST) msg->p1p;
        if (!plugin) RETURN;
        if (!list)   RETURN;

        MSG_CONFIG listmsg = (MSG_CONFIG) msg->msg;
        switch (listmsg) {
          case CONFIGNOTHING:     RETURN;
          case DELETELIST:        RETNUL( plugin->list_delete(list) );
          case ONMESSAGE:         RETLNG( plugin->list_message(list, msg->plugin, msg->p2l, msg->p3l, msg->p4l) );

          // basic list information
          case GETNAME:           RETSTR( plugin->list_getName(list) );
          case SETNAME:           RETNUL( plugin->list_setName(list, msg->p2s) );
          case GETTYPE:           RETSTR( plugin->list_getType(list) );
          case GET_LIST_ID:       RETLNG( plugin->list_getId(  list) );
		  case SET_LIST_ID:       RETNUL( plugin->list_setId(  list, msg->p2l) );

          // list properties and access rights
          case GETLISTFLAGS:           RETINT( plugin->list_getFlags(          list) );
          case GETLISTSECURITY:        RETINT( plugin->list_getSecurity(       list) );
          case GETLISTSECURITYEX:      RETINT( plugin->list_getSecurityEx(     list) );
          case SETLISTSECURITYEX:      RETNUL( plugin->list_setSecurityEx(     list, msg->p2i) );

          // list properties and access rights - specifics
          case ISENABLED:              RETINT( plugin->list_isEnabled(             list) );
          case SETENABLED:             RETNUL( plugin->list_setEnabled(            list, msg->p2i) );
          case HASENABLEDCHILDREN:     RETINT( plugin->list_hasChildrenEnabled(    list, msg->p2i) );
          case ISVISIBLE:              RETINT( plugin->list_isVisible(             list) );
          case SETVISIBLE:             RETNUL( plugin->list_setVisible(            list, msg->p2i) );
          case HASVISIBLECHILDREN:     RETINT( plugin->list_hasChildrenVisible(    list, msg->p2i) );
          case ISEXPANDED:             RETINT( plugin->list_isExpanded(            list) );
          case SETEXPANDED:            RETNUL( plugin->list_setExpanded(           list, msg->p2i) );
          case HASEXPANDEDCHILDREN:    RETINT( plugin->list_hasChildrenExpanded(   list, msg->p2i) );
          case ISHIGHLIGHTED:          RETINT( plugin->list_isHighlighted(         list) );
          case SETHIGHLIGHTED:         RETNUL( plugin->list_setHighlighted(        list, msg->p2i) );
          case HASHIGHLIGHTEDCHILDREN: RETINT( plugin->list_hasChildrenHighlighted(list, msg->p2i) );
          case ISSAVABLE:              RETINT( plugin->list_isSavable(             list) );
          case SETSAVABLE:             RETNUL( plugin->list_setSavable(            list, msg->p2i) );
          case HASSAVABLECHILDREN:     RETINT( plugin->list_hasChildrenSavable(    list, msg->p2i) );
          case ISREADONLY:             RETINT( plugin->list_isReadOnly(            list) );
//        case SETREADONLY:            RETNUL( plugin->list_setReadOnly(           list, msg->p2i) );
          case HASREADONLYCHILDREN:    RETINT( plugin->list_hasChildrenReadOnly(   list, msg->p2i) );

		  case GET_LIST_PARAM:      RETPTR( plugin->list_getListParam(       list) );

          // get list params
          case GETPARAM:            RETSTR( plugin->list_getParam(           list, msg->p2s) );
          case GETPARAMSAFE:        RETSTR( plugin->list_getParamSafe(       list, msg->p2s, msg->p3s) );
          case GETPARAMINT:         RETINT( plugin->list_getParamInt(        list, msg->p2s) );
          case GETPARAMLONG:        RETLNG( plugin->list_getParamLong(       list, msg->p2s) );
          case GETPARAMNUMBER:      RETNUM( plugin->list_getParamNumber(     list, msg->p2s) );
          case GETPARAMGUID:        RETUID( plugin->list_getParamGuid(       list, msg->p2s) );
          case GETPARAMFOURCC:      RETFCC( plugin->list_getParamFourCC(     list, msg->p2s) );

          // set list params
          case SETPARAM:            RETNUL( plugin->list_setParam(           list, msg->p2s, msg->p3s ) );
          case SETPARAMSTRING:      RETNUL( plugin->list_setParamString(     list, msg->p2s, msg->p3s ) );
          case SETPARAMINT:         RETNUL( plugin->list_setParamInt(        list, msg->p2s, msg->p3i ) );
          case SETPARAMLONG:        RETNUL( plugin->list_setParamLong(       list, msg->p2s, msg->p3l ) );
          case SETPARAMNUMBER:      RETNUL( plugin->list_setParamNumber(     list, msg->p2s, msg->p3d ) );
          case SETPARAMGUID:        RETNUL( plugin->list_setParamGuid(       list, msg->p2s, msg->p45g) );
          case SETPARAMFOURCC:      RETNUL( plugin->list_setParamFourCC(     list, msg->p2s, msg->p3f ) );

          // list params everything elses
          case HASPARAM:            RETINT( plugin->list_hasParam(           list, (HPARAM)msg->p2p) );
          case HASPARAMSTR:         RETINT( plugin->list_hasParam(           list, msg->p2s) );
          case HASPARAMS:           RETINT( plugin->list_hasParams(          list) );
/*
          case INSERTPARAM:         RETPTR( plugin->list_insertParam(        list, msg->p2s, msg->p3s, (HPARAM)msg->p5p) );
          case INSERTPARAMINT:      RETPTR( plugin->list_insertParamInt(     list, msg->p2s, msg->p3i, (HPARAM)msg->p5p) );
          case INSERTPARAMNOCFG:    RETPTR( plugin->list_insertParamNoCfg(   list, msg->p2s, msg->p3s, (HPARAM)msg->p5p) );
          case INSERTPARAMINTNOCFG: RETPTR( plugin->list_insertParamIntNoCfg(list, msg->p2s, msg->p3i, (HPARAM)msg->p5p) );
*/          
          case CANENUMPARAMS:       RETINT( plugin->list_canEnumerateParams( list) );
          case GETPARAMCOUNT:       RETINT( plugin->list_getParamCount(      list) );
          case GETFIRSTPARAM:       RETPTR( plugin->list_getParamFirst(      list) );
          case GETLASTPARAM:        RETPTR( plugin->list_getParamLast(       list) );
          case GETPREVPARAM:        RETPTR( plugin->list_getParamPrev(       list, (HPARAM)msg->p2p) );
          case GETNEXTPARAM:        RETPTR( plugin->list_getParamNext(       list, (HPARAM)msg->p2p) );
          case GETPARAMDATA:        RETPTR( plugin->list_getParamHandle(     list, msg->p2s) );
		  case GET_PARAM_BY_ID:     RETPTR( plugin->list_getParamById(       list, msg->p2l) );

		  case GET_PARAM_ID:        RETLNG( plugin->list_getParamId(list, msg->p2s) );
          case SET_PARAM_ID:        RETNUL( plugin->list_setParamId(list, msg->p2s, msg->p3l) );

		  case GETPARAMFLAGS:       RETINT( plugin->list_getParamFlags(      list, msg->p2s) );
          case ISPARAMSAVABLE:      RETINT( plugin->list_isParamSavable(     list, msg->p2s) );
          case ISPARAMREADONLY:     RETINT( plugin->list_isParamReadOnly(    list, msg->p2s) );
          case ISPARAMUSERDATA:     RETINT( plugin->list_isParamUserData(    list, (HPARAM)msg->p2p) );
          case SETPARAMSAVABLE:     RETNUL( plugin->list_setParamSavable(    list, msg->p2s, msg->p3i) );
          case NEWEXTERNALPARAM:    RETPTR( plugin->list_newChild(           list, msg->p2s, msg->p2s, (xmlParamList*)msg->p4p) );
          case REMOVEPARAM:         RETNUL( plugin->list_removeParam(        list, (HPARAM)msg->p2p) );
          case REMOVEPARAMSTR:      RETNUL( plugin->list_removeParam(        list, msg->p2s) );
          case REMOVEALLPARAMS:     RETNUL( plugin->list_removeAllParams(    list) );
/*
          case MOVEPARAM:           RETNUL( plugin->list_moveParam(          list, (HPARAM)msg->p2p, (HPARAM)msg->p3p) );
          case MOVEPARAMSTR:        RETNUL( plugin->list_moveParam(          list, msg->p2s, msg->p3s) );
*/         
          //list hierarchy (parent info, child info, etc)
          case GETPARENT:           RETPTR( plugin->list_getParent(        list) );
          case GETCHILD:            RETPTR( plugin->list_getChild(         list, msg->p2s, msg->p3s, (xmlParamList*)msg->p4p) );
          case GETCHILDBYID:        RETPTR( plugin->list_getChildById(     list, msg->p2l, msg->p3i) );
          case HASCHILD:            RETINT( plugin->list_hasChild(         list, (HLIST)msg->p2p, msg->p3i) );
          case HASCACHEDCHILD:      RETINT( plugin->list_hasChildCached(   list, (HLIST)msg->p2p) );
          case HASCHILDREN:         RETINT( plugin->list_hasChildren(      list) );
          case GETCHILDCOUNT:       RETINT( plugin->list_getChildCount(    list) );
          case GETFIRSTCHILD:       RETPTR( plugin->list_getFirstChild(    list) );
          case GETLASTCHILD:        RETPTR( plugin->list_getLastChild(     list) );
          case GETPREVCHILD:        RETPTR( plugin->list_getPrevChild(     list, (HLIST)msg->p2p) );
          case GETNEXTCHILD:        RETPTR( plugin->list_getNextChild(     list, (HLIST)msg->p2p) );
          case NEWEXTERNALCHILD:    RETPTR( plugin->list_newChild(         list, msg->p2s, msg->p3s, (xmlParamList*)msg->p4p) );
          case REMOVEALLCHILDREN:   RETNUL( plugin->list_removeAllChildren(list) );

          //move list position
          case MOVELISTUP:          RETNUL( plugin->list_moveUp(      list) );
          case MOVELISTDOWN:        RETNUL( plugin->list_moveDown(    list) );
          case MOVELISTBEGINING:    RETNUL( plugin->list_moveBegining(list) );
          case MOVELISTEND:         RETNUL( plugin->list_moveEnd(     list) );
          case MOVECHILD:           RETNUL( plugin->list_moveChild(   list, (HLIST)msg->p2p, (HLIST)msg->p3p) );

          //XML/SQL related stuff
          case FIRSTLOAD:           RETNUL( plugin->list_onFirstLoad(list) );

          case INSERTCALLBACK:      RETNUL( plugin->insertCallback(list, msg->plugin, msg->p2p) );
          case REMOVECALLBACK:      RETNUL( plugin->removeCallback(msg->p2p) );
        }
      } RETURN;


      case VMSG::CALLBACK_CORE: {
        plugCallback *cb = (plugCallback*) msg->p0p;
        if (cb) cb->message(msg);
      } RETURN;


      //list params
      case VMSG::PARAM_BASE: {
        vPlugin *plugin = plg->getPlugin((HPLUGIN)msg->p0p);
        HPARAM param = (HPARAM)msg->p1p;
        if (!plugin) RETURN;
        if (!param)  RETURN;

        MSG_PARAM parammsg = (MSG_PARAM)msg->msg;
        switch (parammsg) {
        case PARAMNOTHING:        RETURN;
          case GETPRMNAME:        RETSTR( plugin->param_getName(       param) );

          case GET_PRM_ID:        RETLNG( plugin->param_getId(         param) );
          case SET_PRM_ID:        RETNUL( plugin->param_setId(         param, msg->p2l) );

          case GETPRMFLAGS:       RETINT( plugin->param_getFlags(      param) );
          case GETPRMSECURITY:    RETINT( plugin->param_getSecurity(   param) );
          case GETPRMSECURITYEX:  RETINT( plugin->param_getSecurityEx( param) );
          case SETPRMSECURITYEX:  RETNUL( plugin->param_setSecurityEx( param, msg->p2i) );

          case GETPRMVALUE:       RETSTR( plugin->param_getValue(      param) );
          case GETPRMVALUEINT:    RETINT( plugin->param_getValueInt(   param) );
          case GETPRMVALUELONG:   RETLNG( plugin->param_getValueLong(  param) );
          case GETPRMVALUENUMBER: RETNUM( plugin->param_getValueNumber(param) );
          case GETPRMVALUEGUID:   RETUID( plugin->param_getValueGuid(  param) );
          case GETPRMVALUEFOURCC: RETFCC( plugin->param_getValueFourCC(param) );

          case SETPRMVALUE:       RETNUL( plugin->param_setValue(      param, msg->p2s ) );
          case SETPRMVALUESTRING: RETNUL( plugin->param_setValueString(param, msg->p2s ) );
          case SETPRMVALUEINT:    RETNUL( plugin->param_setValueInt(   param, msg->p2i ) );
          case SETPRMVALUELONG:   RETNUL( plugin->param_setValueLong(  param, msg->p2l ) );
          case SETPRMVALUENUMBER: RETNUL( plugin->param_setValueNumber(param, msg->p2d ) );
          case SETPRMVALUEGUID:   RETNUL( plugin->param_setValueGuid(  param, msg->p23g) );
          case SETPRMVALUEFOURCC: RETNUL( plugin->param_setValueFourCC(param, msg->p2f ) );

          case ISPRMNULL:         RETINT( plugin->param_isNull(        param) );
          case ISPRMSTRING:       RETINT( plugin->param_isString(      param) );
          case ISPRMINT:          RETINT( plugin->param_isInteger(     param) );
          case ISPRMLONG:         RETINT( plugin->param_isLong(        param) );
          case ISPRMNUMBER:       RETINT( plugin->param_isNumber(      param) );
          case ISPRMGUID:         RETINT( plugin->param_isGuid(        param) );
          case ISPRMFOURCC:       RETINT( plugin->param_isFourCC(      param) );

          case ISPRMSAVABLE:      RETINT( plugin->param_isSavable(     param) );
          case ISPRMREADONLY:     RETINT( plugin->param_isReadOnly(    param) );
        }
      } RETURN;


      case VMSG::FUNCTION_BASE: {
        if (msg->msg == 90) RETPTR( cbCoreFunction::getFirstFunction() );
        if (msg->msg == 91) RETPTR( cbCoreFunction::getLastFunction()  );
        if (msg->msg == 92) RETPTR( cbCoreFunction::getPrevFunction((cbCoreFunction*)msg->p0p) );
        if (msg->msg == 93) RETPTR( cbCoreFunction::getNextFunction((cbCoreFunction*)msg->p0p) );

        cbCoreFunction *cb = NULL;
        if (msg->msg >= 100) {
          cb = cbCoreFunction::getFunction(msg->plugin, (HLIST)msg->p0p, 1);
        } else {
          cb = (cbCoreFunction*) msg->p0p;
        }

        if (!cb) RETURN;
        switch (msg->msg) {
          case   3: RETSTR( cb->getScriptName() );
          case   4: RETINT( cb->getParamCount() );
          case   5: RETLNG( cb->getId() );
          case   6: RETINT( cb->isEnabled() );
          case   7: RETINT( cb->isVisible() );
          case   8: RETINT( cb->getFunctionType() );
          case   9: RETPTR( cb->getFunctionParent() );
          case  10: RETPTR( cb->getFirstChildFunction() );
          case  11: RETPTR( cb->getLastChildFunction()  );
          case  12: RETPTR( cb->getPrevChildFunction((cbCoreFunction*)msg->p1i) );
          case  13: RETPTR( cb->getNextChildFunction((cbCoreFunction*)msg->p1i) );
          case  20: RETSTR( cb->getFunctionName() );
          case  21: RETPTR( cb->getRemotePlugin() );
          case  22: RETPTR( cb->getRemoteConfig() );

#ifdef _RMX_LUA_
		  case 100: RETINT( cb->script_isValueNull(    msg->p1s) );
          case 101: RETINT( cb->script_isValueBoolean( msg->p1s) );
          case 102: RETINT( cb->script_isValueInteger( msg->p1s) );
          case 103: RETINT( cb->script_isValueNumber(  msg->p1s) );
          case 104: RETINT( cb->script_isValueString(  msg->p1s) );
          case 111: RETNUL( cb->script_setValueBoolean(msg->p1s, msg->p2i) );
          case 112: RETNUL( cb->script_setValueInteger(msg->p1s, msg->p2i) );
          case 113: RETNUL( cb->script_setValueNumber( msg->p1s, msg->p2d) );
          case 114: RETNUL( cb->script_setValueString( msg->p1s, msg->p2s) );
          case 121: RETINT( cb->script_getValueBoolean(msg->p1s) );
          case 122: RETINT( cb->script_getValueInteger(msg->p1s) );
          case 123: RETNUM( cb->script_getValueNumber( msg->p1s) );
          case 124: RETSTR( cb->script_getValueString( msg->p1s) );
#endif //_RMX_LUA_
		}
      } RETURN;



      case VMSG::XML_PARAM_LIST: {
        VXMLPARAMLIST xmlmsg  = (VXMLPARAMLIST) msg->msg;
        xmlParamList *xmllist = (xmlParamList*) msg->p0p;
        if (!xmllist) RETURN;

        switch (xmlmsg) {
          case XMLPRMGETITEM:  RETSTR( xmllist->getItemValue(msg->p1s) );

          case XMLPRMGETFIRST: RETSTR( xmllist->getFirstName() );
          case XMLPRMGETLAST:  RETSTR( xmllist->getLastName( ) );
          case XMLPRMGETPREV:  RETSTR( xmllist->getPrevName(msg->p1s) );
          case XMLPRMGETNEXT:  RETSTR( xmllist->getNextName(msg->p1s) );
        }
      } RETURN;


/*  TODO:  reimplement these!!
      case VMSG::XML_READER: {
        if (msg->msg == XMLCREATE)  RETPTR( new xmlReader(msg->p0s, msg->p1p, plg->getPlugin(msg->plugin)) );
        xmlReader *reader = (xmlReader*) msg->p0p;
        if (!reader) RETURN;
        switch (msg->msg) {
          case XMLDESTROY:     RETNUL( delete reader );
          case XMLNOTHING:     RETURN;
          case XMLGETFILELINE: RETINT( reader->xmlGetFileLine() );
          case XMLPARSEFILE:   RETNUL( reader->loadFile(msg->p1s) );
        }
      } RETURN;


      case VMSG::XML_PARAMS: {
        XmlReaderParams *param = (XmlReaderParams*) msg->p0p;
        if (!param) RETURN;
        switch (msg->msg) {
          case XPRMNOTHING:     RETURN;
          case XPRMGETNAME:     RETSTR( param->getItemName(    msg->p1i) );
          case XPRMGETVALUE:    RETSTR( param->getItemValue(   msg->p1s) );
          case XPRMGETVALUEID:  RETSTR( param->getItemValue(   msg->p1i) );
          case XPRMGETVALUEINT: RETINT( param->getItemValueInt(msg->p1s) );
          case XPRMGETCOUNT:    RETINT( param->getNbItems() );
        }
      } RETURN;
*/
    }
    RETURN;
  }
}
