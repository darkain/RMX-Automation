/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#define RMX_LIB


#include "../../sdk/rmxBase.h"
#include "../../sdk/plugBase.h"
#include "../../sdk/cb/cbCore.h"
#include "../../sdk/Win32/Timer.h"
#include "../../sdk/Win32/Win32.h"

#include "rmx_entry.h"
#include "rmx_msg.h"
#include "cbRemote.h"


extern plugBase *plugin;
extern RMX_ENTRY rmx;


//TODO: move this over to plugBase
UINT xmlStatus = 0;


extern "C" {
  void __declspec(dllexport) RMXCALL _RMX_ENTRY(rmxMsg *msg) {
    if (!msg) RETURN;

    VMSG::MESSAGES thismsg = (VMSG::MESSAGES)LOWORD(msg->type);

    switch (thismsg) {

      //system commands
      case VMSG::SYSTEM:
        switch (msg->msg) {
          case   1:  //create;
            if (!plugin) RETINT(0);
            rmx = (RMX_ENTRY)msg->p0p;

            ZeroMemory(&extrmxmsg, sizeof(extrmxmsg));
            extrmxmsg.plugin = (HPLUGIN)msg->p1p;

            RMXOS::setInstance((HINSTANCE)msg->p2p);

            RETINT(1);

          case   2:  //destroy
            if (!plugin) RETINT(0);
            plugin->onQuit();
            vdelnull(plugin);
            RETINT(1);
            

          case   6:  RETNUL( plugin->onPostInit() );
          case   7:  RETPTR( plugin );
//        case   8:  return appId;  //reserved for secureLoader
          case   9:  RETSTR( plugin->getName() );  //should be replaced with cfgRoot stuff
          case  10:  RETNUL( plugin->initConfig() );
          case  11:  RETPTR( cfgBase::getListRoot() );
		  case  12:  RETLNG( cfgBase::getRootId() );

          case  30:  RETINT( sizeof(rmxMsg) );

          case 300:  RETNUL( xmlStatus = msg->p0i );
          case 301:  RETINT( xmlStatus );
        }
        RETURN;


      //primary configuration class
      case VMSG::CONFIG_BASE: {
        MSG_CONFIG cfgmsg = (MSG_CONFIG) msg->msg;
        cfgBase *list = (cfgBase*) msg->p0p;
        if (!list) RETURN;

        switch (cfgmsg) {
          case CONFIGNOTHING:     RETURN;
          case DELETELIST:        RETNUL( delete list );
          case ONMESSAGE:         RETLNG( list->onMessage((HPLUGIN)msg->p1p, msg->p2l, msg->p3l, msg->p4l) );

          // basic list information
          case GETNAME:           RETSTR( list->getName() );
          case SETNAME:           RETNUL( list->setName(msg->p1s) );
          case GETTYPE:           RETSTR( list->getType() );
		  case GET_LIST_ID:       RETLNG( list->getId() );
		  case SET_LIST_ID:       RETNUL( list->setId(msg->p1l) );

          // list properties and access rights
          case GETLISTFLAGS:      RETINT( list->getFlags() );
          case GETLISTSECURITY:   RETINT( list->getSecurity() );
          case GETLISTSECURITYEX: RETINT( list->getSecurityEx() );
          case SETLISTSECURITYEX: RETNUL( list->setSecurityEx(msg->p1i) );

          // list properties and access rights - specifics
          case ISENABLED:              RETINT( list->isEnabled() );
          case SETENABLED:             RETNUL( list->setEnabled(msg->p1i) );
          case HASENABLEDCHILDREN:     RETINT( list->hasChildrenEnabled(msg->p1i) );
          case ISVISIBLE:              RETINT( list->isVisible() );
          case SETVISIBLE:             RETNUL( list->setVisible(msg->p1i) );
          case HASVISIBLECHILDREN:     RETINT( list->hasChildrenVisible(msg->p1i) );
          case ISEXPANDED:             RETINT( list->isExpanded() );
          case SETEXPANDED:            RETNUL( list->setExpanded(msg->p1i) );
          case HASEXPANDEDCHILDREN:    RETINT( list->hasChildrenExpanded(msg->p1i) );
          case ISHIGHLIGHTED:          RETINT( list->isHighlighted() );
          case SETHIGHLIGHTED:         RETNUL( list->setHighlighted(msg->p1i) );
          case HASHIGHLIGHTEDCHILDREN: RETINT( list->hasChildrenHighlighted(msg->p1i) );
          case ISSAVABLE:              RETINT( list->isSavable() );
          case SETSAVABLE:             RETNUL( list->setSavable(msg->p1i) );
          case HASSAVABLECHILDREN:     RETINT( list->hasChildrenSavable(msg->p1i) );
          case ISREADONLY:             RETINT( list->isReadOnly() );
//        case SETREADONLY:            RETNUL( list->setReadOnly(msg->p1i) );
          case HASREADONLYCHILDREN:    RETINT( list->hasChildrenReadOnly(msg->p1i) );

          // get list params
          case GETPARAM:            RETSTR( list->getParam(      msg->p1s) );
          case GETPARAMSAFE:        RETSTR( list->getParamSafe(  msg->p1s, msg->p2s) );
          case GETPARAMINT:         RETINT( list->getParamInt(   msg->p1s) );
          case GETPARAMLONG:        RETLNG( list->getParamLong(  msg->p1s) );
          case GETPARAMNUMBER:      RETNUM( list->getParamNumber(msg->p1s) );
          case GETPARAMGUID:        RETUID( list->getParamGuid(  msg->p1s) );

		  case GET_LIST_PARAM:      RETPTR( list->getParam() );

          // set list params
          case SETPARAM:            RETNUL( list->setParam(      msg->p1s, msg->p2s ) );
          case SETPARAMSTRING:      RETNUL( list->setParamString(msg->p1s, msg->p2s ) );
          case SETPARAMINT:         RETNUL( list->setParamInt(   msg->p1s, msg->p2i ) );
          case SETPARAMLONG:        RETNUL( list->setParamLong(  msg->p1s, msg->p2l ) );
          case SETPARAMNUMBER:      RETNUL( list->setParamNumber(msg->p1s, msg->p2d ) );
          case SETPARAMGUID:        RETNUL( list->setParamGuid(  msg->p1s, msg->p23g) );

          // list params everything elses
          case HASPARAM:            RETINT( list->hasParam((prmBase*)msg->p1p) );
          case HASPARAMSTR:         RETINT( list->hasParam(msg->p1s) );
          case HASPARAMS:           RETINT( list->hasParams() );
/*
          case INSERTPARAM:         RETPTR( list->insertParam(msg->p1s, msg->p2s, (prmBase*)msg->p3p) );
          case INSERTPARAMINT:      RETPTR( list->insertParamInt(msg->p1s, msg->p2i, (prmBase*)msg->p3p) );
          case INSERTPARAMNOCFG:    RETPTR( list->insertParamNoSave(msg->p1s, msg->p2s, (prmBase*)msg->p3p) );
          case INSERTPARAMINTNOCFG: RETPTR( list->insertParamIntNoSave(msg->p1s, msg->p2i, (prmBase*)msg->p3p) );
*/
          case CANENUMPARAMS:       RETINT( list->canEnumerateParams() );
          case GETPARAMCOUNT:       RETINT( list->getParamCount() );
          case GETFIRSTPARAM:       RETPTR( list->getFirstParam() );
          case GETLASTPARAM:        RETPTR( list->getLastParam() );
          case GETPREVPARAM:        RETPTR( list->getPrevParam((prmBase*)msg->p1p) );
          case GETNEXTPARAM:        RETPTR( list->getNextParam((prmBase*)msg->p1p) );
          case GETPARAMDATA:        RETPTR( list->getParamData(msg->p1s) );
		  case GET_PARAM_BY_ID:     RETPTR( list->getParamById(msg->p1l) );

		  case GET_PARAM_ID:        RETLNG( list->getParamId(msg->p1s) );
          case SET_PARAM_ID:        RETNUL( list->setParamId(msg->p1s, msg->p2l) );

          case GETPARAMFLAGS:       RETINT( list->getParamFlags(msg->p1s) );
          case ISPARAMSAVABLE:      RETINT( list->isParamSavable(msg->p1s) );
          case ISPARAMREADONLY:     RETINT( list->isParamReadOnly(msg->p1s) );
          case ISPARAMUSERDATA:     RETINT( list->isParamUserData((prmBase*)msg->p1p) );
          case SETPARAMSAVABLE:     RETNUL( list->setParamSavable(msg->p1s, msg->p2i) );
          case NEWEXTERNALPARAM:    RETPTR( list->onNewExternalParam(msg->p1s, msg->p2s) );
          case REMOVEPARAM:         RETNUL( list->removeParam((prmBase*)msg->p1p) );
          case REMOVEPARAMSTR:      RETNUL( list->removeParam(msg->p1s) );
          case REMOVEALLPARAMS:     RETNUL( list->removeAllParams() );
/*
          case MOVEPARAM:           RETNUL( list->moveParam((prmBase*)msg->p1p, (prmBase*)msg->p2p) );
          case MOVEPARAMSTR:        RETNUL( list->moveParam(msg->p1s, msg->p2s) );
*/
          //list hierarchy (parent info, child info, etc)
          case GETPARENT:           RETPTR( list->getParent() );
          case GETCHILD:            RETPTR( list->getChild(msg->p1s, msg->p2s, (HPARAMLIST)msg->p3p) );
          case GETCHILDBYID:        RETPTR( list->getChildById(msg->p1l, msg->p2i) );
//          case GETCHILDBYDATA:      RETPTR( list->getChildByUserData(msg->p1s, msg->p2i) );
          case HASCHILD:            RETINT( list->hasChild((cfgBase*)msg->p1p, msg->p2i) );
          case HASCACHEDCHILD:      RETINT( list->hasChildCached((cfgBase*)msg->p1p) );
          case HASCHILDREN:         RETINT( list->hasChildren() );
          case GETCHILDCOUNT:       RETINT( list->getChildCount() );
          case GETFIRSTCHILD:       RETPTR( list->getFirstChild() );
          case GETLASTCHILD:        RETPTR( list->getLastChild() );
          case GETPREVCHILD:        RETPTR( list->getPrevChild((cfgBase*)msg->p1p) );
          case GETNEXTCHILD:        RETPTR( list->getNextChild((cfgBase*)msg->p1p) );
          case NEWEXTERNALCHILD:    RETPTR( list->onNewExternalChild(msg->p1s, msg->p2s, (HPARAMLIST)msg->p3p) );
          case REMOVEALLCHILDREN:   RETNUL( list->removeAllChildren() );

          //move list position
          case MOVELISTUP:          RETNUL( list->moveListUp() );
          case MOVELISTDOWN:        RETNUL( list->moveListDown() );
          case MOVELISTBEGINING:    RETNUL( list->moveListToBegining() );
          case MOVELISTEND:         RETNUL( list->moveListToEnd() );
          case MOVECHILD:           RETNUL( list->moveChild((cfgBase*)msg->p1p, (cfgBase*)msg->p2p) );

          case FIRSTLOAD:           RETNUL( list->onFirstLoad() );
        }
      } RETURN;


      //list callbacks
      case VMSG::CALLBACK_CORE: {
        MSG_CALLBACK_CORE cbmsg = (MSG_CALLBACK_CORE)msg->msg;
        cbCore *cb = (cbCore*) msg->p0p;
        if (!cb) RETURN;

		//TODO: reimpliment cbCore
        switch (cbmsg) {
          case MSG_CB_CORE_NOTHING:   RETURN;

          case MSG_CB_ON_DELETE:      RETNUL( cb->cb_onDelete( (HLIST)msg->p1p ) );

		  case MSG_CB_MESSAGE_TEXT:   RETNUL( cb->cb_cfgMessage( (cbMessage)msg->p1i, msg->p2s ) );
		  case MSG_CB_MESSAGE_INT:    RETNUL( cb->cb_cfgMessage( (cbMessage)msg->p1i, msg->p2l ) );
//	NOTE: We are now passing an extra param (HLIST)(list) which dispatched the message
//		  case MSG_CB_MESSAGE_CONFIG: RETNUL( cb->cb_cfgMessage( (cbMessage)msg->p1i, (HLIST) msg->p2p, (HLIST) msg->p3p ) );
//		  case MSG_CB_MESSAGE_PARAM:  RETNUL( cb->cb_cfgMessage( (cbMessage)msg->p1i, (HPARAM)msg->p2p, (HPARAM)msg->p3p ) );
		  case MSG_CB_MESSAGE_CONFIG: RETNUL( cb->cb_cfgMessage( (cbMessage)msg->p1i, (HLIST) msg->p3p, (HLIST) msg->p4p ) );
		  case MSG_CB_MESSAGE_PARAM:  RETNUL( cb->cb_cfgMessage( (cbMessage)msg->p1i, (HPARAM)msg->p3p, (HPARAM)msg->p4p ) );
			  
/*
          case ONNAMECHANGE:     RETNUL( cb->cb_onNameChange(    msg->p1s) );
          case ONSETTOOLTIP:     RETNUL( cb->cb_onSetTooltip(    msg->p1s) );
          case ONSETUSERDATA:    RETNUL( cb->cb_onSetUserData(  (HPARAM)msg->p1p) );

          case ONINSERTPARAM:    RETNUL( cb->cb_onInsertParam(  (HPARAM)msg->p1p, (HPARAM)msg->p2p) );
          case ONREMOVEPARAM:    RETNUL( cb->cb_onRemoveParam(  (HPARAM)msg->p1p) );
          case ONSETPARAM:       RETNUL( cb->cb_onSetParam(     (HPARAM)msg->p1p) );
          case ONMOVEPARAM:      RETNUL( cb->cb_onMoveParam(    (HPARAM)msg->p1p, (HPARAM)msg->p2p) );

          case ONINSERTCHILD:    RETNUL( cb->cb_onInsertChild(  (HLIST)msg->p1p, (HLIST)msg->p2p) );
          case ONREMOVECHILD:    RETNUL( cb->cb_onRemoveChild(  (HLIST)msg->p1p) );
          case ONMOVECHILD:      RETNUL( cb->cb_onMoveChild(    (HLIST)msg->p1p, (HLIST)msg->p2p) );

          case ONSETVISIBLE:     RETNUL( cb->cb_onSetVisible(    msg->p1i) );
          case ONSETENABLED:     RETNUL( cb->cb_onSetEnabled(    msg->p1i) );
          case ONSETEXPANDED:    RETNUL( cb->cb_onSetExpanded(   msg->p1i) );
          case ONSETHIGHLIGHTED: RETNUL( cb->cb_onSetHighlighted(msg->p1i) );
          case ONSETSAVABLE:     RETNUL( cb->cb_onSetSavable(    msg->p1i) );
          case ONSETREADONLY:    RETNUL( cb->cb_onSetReadOnly(   msg->p1i) );
*/
        }
      } RETURN;


      case VMSG::CALLBACK_REMOTE: {
        MSG_CALLBACK_CORE cbmsg = (MSG_CALLBACK_CORE)msg->msg;
        if (msg->msg == CBCFGINSERT) RETPTR( new cbRemote((cfgBase*)msg->p0p, msg->p1p) );
        if (msg->msg == CBCFGREMOVE) delete ((cbRemote*)(msg->p0p));
      } RETURN;


      //list params
      case VMSG::PARAM_BASE: {
        MSG_PARAM parammsg = (MSG_PARAM)msg->msg;
        prmBase   *param   = (prmBase*) msg->p0p;
        if (!param) RETURN;

        switch (parammsg) {
          case PARAMNOTHING:      RETURN;
          case ISPRMNAMED:        RETINT( param->isNamed(msg->p1s) );
          case GETPRMNAME:        RETSTR( param->getName() );

		  case GET_PRM_ID:        RETLNG( param->getId() );
		  case SET_PRM_ID:        RETNUL( param->setId(msg->p1l) );

          case GETPRMFLAGS:       RETINT( param->getFlags() );
          case GETPRMSECURITY:    RETINT( param->getSecurity() );
          case GETPRMSECURITYEX:  RETINT( param->getSecurityEx() );
          case SETPRMSECURITYEX:  RETNUL( param->_setSecurityEx(msg->p1i) );

          case GETPRMVALUE:       RETSTR( param->getValue()       );
          case GETPRMVALUEINT:    RETINT( param->getValueInt()    );
          case GETPRMVALUELONG:   RETLNG( param->getValueLong()   );
          case GETPRMVALUENUMBER: RETNUM( param->getValueNumber() );
          case GETPRMVALUEGUID:   RETUID( param->getValueGuid()   );
          case GETPRMVALUEFOURCC: RETFCC( param->getValueFourCC() );

          case SETPRMVALUE:       RETNUL( param->setValue(      msg->p1s ) );
          case SETPRMVALUESTRING: RETNUL( param->setValueString(msg->p1s ) );
          case SETPRMVALUEINT:    RETNUL( param->setValueInt(   msg->p1i ) );
          case SETPRMVALUELONG:   RETNUL( param->setValueLong(  msg->p1l ) );
          case SETPRMVALUENUMBER: RETNUL( param->setValueNumber(msg->p1d ) );
          case SETPRMVALUEGUID:   RETNUL( param->setValueGuid(  msg->p23g) );
          case SETPRMVALUEFOURCC: RETNUL( param->setValueFourCC(msg->p1f)  );

          case ISPRMNULL:         RETINT( param->isNull()    );
          case ISPRMSTRING:       RETINT( param->isString()  );
          case ISPRMINT:          RETINT( param->isInteger() );
          case ISPRMLONG:         RETINT( param->isLong()    );
          case ISPRMNUMBER:       RETINT( param->isNumber()  );
          case ISPRMGUID:         RETINT( param->isGuid()    );
          case ISPRMFOURCC:       RETINT( param->isFourCC()  );

          case ISPRMSAVABLE:      RETINT( param->isSavable() );
          case ISPRMREADONLY:     RETINT( param->isReadOnly() );
        }
      } RETURN;
    
/*     
      case VMSG::XML_READER: {
        vXmlReader *reader = (vXmlReader*) msg->p0p;
        if (!reader) RETURN;

        switch (msg->msg) {
          case XMLELEMENTSTART:  RETNUL( reader->onElementStart(msg->p1s, msg->p2s, (vXmlParams*)msg->p3p) );
          case XMLELEMENTEND:    RETNUL( reader->onElementEnd(msg->p1s, msg->p2s) );
          case XMLCHARACTERDATA: RETNUL( reader->onCharacterData(msg->p1s, msg->p2s, msg->p3s) );
          case XMLREADERROR:     RETNUL( reader->onReaderError(msg->p1i, msg->p2i, msg->p3s) );
        };
      } RETURN;
*/
    }

    RETURN;
  }
}
