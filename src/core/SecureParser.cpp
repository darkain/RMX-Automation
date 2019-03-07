/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/

#if 0

#include <rmxBase.h>

#include "SecureLoader.h"
#include "SecureParser.h"
#include "LocalPlugin.h"
#include <winsock.h>

#ifdef RMX_IPCLOADER
#include "ipc/ipcConfigCallback.h"
#include "ipc/ipcRootListCallback.h"
#endif


#pragma comment(lib, "WS2_32.lib")


vSecureParser::vSecureParser(vPlugin *plug, UINT client) {
  plugin = plug;
  remoteclient = client;
}

vSecureParser::~vSecureParser() {
}


void vSecureParser::addString(const char *string) {
  if (!string) return;
  stringTable.appendItem(string);
}


UINT vSecureParser::beginParser(BYTE *data, int len) {
  if (!data) return 0;

#ifdef _DEBUG
  BYTE tmp[4096];
  memcpy(tmp, data, min(len, sizeof(tmp)));
#endif

  const char *begin = (const char*) data;
  const char *pos = begin;
  int dlen = ntohl(*((int*)pos));
  pos += 4;
  vSecureParser *parser = (vSecureParser*)ntohl(*((int*)pos));
  pos += 4;

  rmxList<RMXSTACK*> stack;
  while ((pos - begin) < (int)len) {
    RMXSTACK *x = (RMXSTACK*)pos;
    x->format = ntohl(x->format);
    x->len = ntohl(x->len);

    if (x->format == 1) {
      x->data = ntohl(x->data);
      pos += sizeof(RMXSTACK);

    } else if (x->format == 2) {
      pos += sizeof(RMXSTACK)-4;
      while (*pos) pos++;
      pos++;

    } else if (x->format == 3) {
      x->data = ntohl(x->data);
      pos += sizeof(RMXSTACK);
    }
    stack.appendItem(x);
  }

  if (stack.getItemCount() < 2) return 0;
  return parser->parseCommand(&stack);
}


//------------------------------------------------------------------------------------------------------------------------------


UINT vSecureParser::parseCommand(rmxList<RMXSTACK*> *stack) {
  return 0;
}
/*
  switch (stack->getItem(0)->data) {
    case 1: 
      switch (stack->getItem(1)->data) {
        case   4: return (UINT) plugin->getPluginID();
        case   9: return (UINT) sendString(plugin->getName());

        case  16: return (UINT) plg->processMessage(stack->getItem(2)->str);
        case  18: return (UINT) sendString(plg->getMessageName(stack->getItem(2)->str));

        case 100: return (UINT) plg->getAppVersionNumber();
        case 101: return (UINT) sendString(plg->getAppName());
        case 102: return (UINT) sendString(plg->getAppVersion());
        case 103: return (UINT) plg->getAppWnd();

        case 200: return (UINT) plg->getFirstPluginHandle();
        case 201: return (UINT) plg->getLastPluginHandle();
        case 202: return (UINT) plg->getNextPluginHandle((HPLUGIN)stack->getItem(2)->data);
        case 203: return (UINT) plg->getPrevPluginHandle((HPLUGIN)stack->getItem(2)->data);
        case 204: return (UINT) plg->getPluginHandle(stack->getItem(2)->str);

        case 210: return (UINT) sendString(plg->getPluginName((HPLUGIN)stack->getItem(2)->data));
        case 211: return (UINT) plg->getPluginVersion((HPLUGIN)stack->getItem(2)->data);
        case 212: return (UINT) plg->pluginExists((HPLUGIN)stack->getItem(2)->data);
        case 213: return (UINT) sendString(plg->getPluginFilename((HPLUGIN)stack->getItem(2)->data));
//        case 220: return (UINT) sendPluginMessage(stack->getItem(2)->str);
//        case 221: return (UINT) sendPluginMessagePtr(stack->getItem(2)->str);

//        case 300: plg->consoleMessage(stack->getItem(2)->str, stack->getItem(3)->data); break;
        case 301: return (UINT) plg->consoleCommand(stack->getItem(2)->str);

        case 400: return (UINT) plg->getMasterPlugin()->getPluginID();  //application ID maybe?
        case 410: remoteclient = stack->getItem(2)->data; break;
      }
      break;


    case 2: {
//      vConfigList *list = (vConfigList*) stack->getItem(2)->data;
      HLIST list = (HLIST) stack->getItem(2)->data;
      if (!list) return 0;
      if (!valid(list)) return 0;
      VCONFIGLIST listmsg = (VCONFIGLIST) stack->getItem(1)->data;
      switch (listmsg) {
          case CONFIGNOTHING: break;

          case GETNAME:     return (UINT) sendString(plugin->getConfigListName(list)); //sendString(list->getName());
          //case SETNAME:     //list->setName(stack->getItem(3)->str);    break;  //not supported by vPlugin* API  (update)
          case GETTYPE:    return (UINT) sendString(plugin->getConfigListType(list)); //sendString(list->getType());
          //case SETTYPE:    break;
          case GETUNIQUE:  return (UINT) sendString(plugin->getConfigListUnique(list)); //sendString(list->getUnique());
          //case SETUNIQUE:  break;
          case GETTOOLTIP: return (UINT) sendString(plugin->getConfigListTooltip(list)); //sendString(list->getTooltip());
          //case SETTOOLTIP: plugin->setConfigListTool //list->setTooltip(stack->getItem(3)->str); break;  //not supported by vPlugin* API  (update)

          case GETENABLED:     return plugin->getConfigListEnabled(list);
          case SETENABLED:     plugin->getConfigListEnabled(list); break;
          case GETVISIBLE:     return plugin->getConfigListVisible(list);
          case SETVISIBLE:     plugin->getConfigListVisible(list); break;
          case GETEXPANDED:    return plugin->getConfigListExpanded(list);
          case SETEXPANDED:    plugin->getConfigListExpanded(list); break;
          case GETHIGHLIGHTED: return plugin->getConfigListHighlighted(list);
          case SETHIGHLIGHTED: plugin->getConfigListHighlighted(list); break;

          case GETPARAM: return (UINT) sendString(plugin->getConfigListParam(list, stack->getItem(3)->str));
          case GETPARAMSAFE: return (UINT) sendString(plugin->getConfigListParamSafe(list, stack->getItem(3)->str, stack->getItem(4)->str)); //list->getParamSafe((const char*)msg->param1, (const char*)msg->param2);
          case GETPARAMINT: return (UINT) plugin->getConfigListParamInt(list, stack->getItem(3)->str);
          case SETPARAM: plugin->setConfigListParam(list, stack->getItem(3)->str, stack->getItem(4)->str); break;
          case SETPARAMINT: plugin->setConfigListParamInt(list, stack->getItem(3)->str, (int)stack->getItem(4)->data); break;
//          case INSERTPARAM: return (UINT) plugin->insertConfigListParam(stack->getItem(3)->str, stack->getItem(4)->str, (vConfigParam*)stack->getItem(5)->data);
//          case INSERTPARAMINT: return (UINT) plugin->insertConfigListParamInt(stack->getItem(3)->str, (int)stack->getItem(4)->data, (vConfigParam*)stack->getItem(5)->data);
//          case INSERTPARAMNOCFG: return (UINT) plugin->insertConfigListParamNoCfg(stack->getItem(3)->str, stack->getItem(4)->str, (vConfigParam*)stack->getItem(5)->data);
//          case INSERTPARAMINTNOCFG: return (UINT) plugin->insertConfigListParamIntNoCfg(stack->getItem(3)->str, (int)stack->getItem(4)->data, (vConfigParam*)stack->getItem(5)->data);
          case GETFIRSTPARAM: return (UINT) plugin->getConfigListParamFirst(list);
          case GETNEXTPARAM: return (UINT) plugin->getConfigListParamNext(list, (HPARAM)stack->getItem(3)->data);
          case GETPARAMDATA: return (UINT) plugin->getConfigListParamData(list, stack->getItem(3)->str);
          //case GETPARAMFLAGS: return (UINT) plugin->getConfigParamFlags(stack->getItem(3)->str);
//          case REMOVEPARAM: plugin->removeConfigListParam((vConfigParam*)stack->getItem(3)->data); break;
//          case REMOVEPARAMSTR: plugin->removeConfigListParam(stack->getItem(3)->str); break;

          case GETPARENT: return (UINT) plugin->getConfigListParent(list);
//          case GETROOT: return (UINT) plugin->getConfigListRoot(list);

//          case INSERTCHILD: list->insertChild((vConfigList*)msg->param1, (vConfigList*)msg->param2); break;
//          case REMOVECHILD: list->removeChild((vConfigList*)msg->param1); break;
//          case DELETECHILD: list->deleteChild((vConfigList*)msg->param1); break;
          case GETCHILD: return (UINT) plugin->getConfigChild(list, stack->getItem(3)->str, stack->getItem(4)->str, stack->getItem(5)->str);
//          case HASCHILDREN: return (UINT) plugin->hasChildren();
          

          case GETFIRSTCHILD: {
            HLIST child = plugin->getFirstConfigChild(list);
            while (child) {
              LISTSECURITY sec;
              sec.flags = plugin->getConfigListFlags(list);
              if (sec.ipc) return (UINT) child;
              child = plugin->getNextConfigChild(list, child);
            }
          } return NULL;
/ *
          case GETLASTCHILD: {
            HLIST child = plugin->getLastConfigChild(list);
            while (child) {
              LISTSECURITY sec;
              sec.flags = plugin->getConfigListFlags(list);
              if (sec.ipc) return (UINT) child;
              child = plugin->getPrevConfigChild(list, child);
            }
          } return NULL;

          case GETPREVCHILD: { // return (UINT) list->getPrevChild((vConfigList*)stack->getItem(3)->data);
            HLIST child = plugin->getPrevConfigChild(list, (HLIST)stack->getItem(3)->data);
            while (child) {
              LISTSECURITY sec;
              sec.flags = plugin->getConfigListFlags(list);
              if (sec.ipc) return (UINT) child;
              child = plugin->getPrevConfigChild(list, child);
            }
          } return NULL;
* /
          case GETNEXTCHILD: { // return (UINT) list->getNextChild((vConfigList*)stack->getItem(3)->data);
            HLIST child = plugin->getNextConfigChild(list, (HLIST)stack->getItem(3)->data);
            while (child) {
              LISTSECURITY sec;
              sec.flags = plugin->getConfigListFlags(list);
              if (sec.ipc) return (UINT) child;
              child = plugin->getNextConfigChild(list, child);
            }
          } return NULL;


//          case ONACTIVATED: return (UINT) static_cast<vFunction*>(list)->onActivated((vFunctionCall*)msg->param1);
//          case GETMESSAGENAME: return (UINT) static_cast<vFunction*>(list)->getMessageName((vFunctionCall*)msg->param1);
//          case ONACTIVATED: return (UINT) static_cast<vFunction*>(list)->onActivated(&vFunctionCall(stack->getItem(3)->str));
//          case GETMESSAGENAME: return (UINT) sendString(static_cast<vFunction*>(list)->getMessageName(&vFunctionCall(stack->getItem(3)->str)));
          case ONACTIVATED: return (UINT) plg->processMessage(stack->getItem(2)->str);  //this needs to be updated
          case GETMESSAGENAME: return (UINT) sendString(plg->getMessageName(stack->getItem(2)->str));  //this needs to be updated

          case GETUSERDATA: return (UINT) sendString(plugin->getConfigListUserData(list));
          case GETUSERDATASAFE: return (UINT) sendString(plugin->getConfigListUserDataSafe(list, stack->getItem(3)->str)); //list->getUserDataSafe((const char*)msg->param1, (const char*)msg->param2);
          case SETUSERDATA: plugin->setConfigListUserData(list, stack->getItem(3)->str); break;
          case GETUSERDATAINT: return (UINT) plugin->getConfigListUserDataInt(list);
          case SETUSERDATAINT: plugin->setConfigListUserDataInt(list, (int)stack->getItem(3)->data); break;

          case GETLISTFLAGS: return plugin->getConfigListFlags(list);

//          case HASPARAM: return list->hasParam(stack->getItem(3)->str);

//          case ACTIVATEMENU: (static_cast<vConfigMenu*>(list))->onClick(); break;

          case GETSAVABLE: return plugin->getConfigListSavable(list);
          case SETSAVABLE: plugin->setConfigListSavable(list, stack->getItem(3)->data); break;
//          case GETPROFILABLE: return plugin->getConfigListProfilable(list);
//          case SETPROFILABLE: plugin->setConfigListProfilable(list, stack->getItem(3)->data); break;

          case GETCHILDCOUNT: return plugin->getConfigChildCount(list); //list->getChildCount();
          case GETPARAMCOUNT: return plugin->getConfigListParamCount(list); //list->getParamCount();

//          case GETENCODERINTERFACE: return (UINT) (static_cast<vEncoder*>(list)->getEncoderInterface());
//          case ENCODEDATA: (static_cast<vEncoderHandle*>(list))->encodeData((BYTE*)msg->param1, msg->param2); break;
//          case GETENCODERDATA:   return (UINT) (static_cast<vEncoderHandle*>(list))->getData();
//          case GETENCODERLENGTH: return (UINT) (static_cast<vEncoderHandle*>(list))->getLength();
//          case RELEASEDECODERHANDLE: (static_cast<vEncoderHandle*>(list))->releaseEncoderHandle(); break;

//          case DELETELIST: delete list; break;

//          case ACTIVATESCRIPT: plugin->activateScriptFunction(list); break;
      }
    } break;

#ifdef RMX_IPCLOADER  //this needs to be removed eventually
    //list callbacks
    case 3: {
      //this needs to be reworked into taking a plugin* as well
      if ((int)stack->getItem(1)->data == -1) return (UINT) new ipcConfigCallback((HLIST)stack->getItem(3)->data, plugin, stack->getItem(4)->data, (vSecureParser*)stack->getItem(5)->data);
      rmxListCallback *cb = (rmxListCallback*) stack->getItem(2)->data;
      if (!cb) return 0;
      VCONFIGCALLBACK cbmsg = (VCONFIGCALLBACK)stack->getItem(1)->data;
      switch (cbmsg) {
        case CALLBACKNOTHING: break;
        case DELETECALLBACK:  delete cb; break;
        
        case ONDELETE: {
#ifdef _DEBUG
  char str[1024];
  VPRINTF(str, sizeof(str), "\033[1;35m[IPC LIST CALLBACK] onDelete: THIS=%08X", cb);
  plg->printConsole(str, CONSOLE_MAIN);
#endif
          cb->onDelete(); 
        } break;

        case ONNAMECHANGE:     cb->onNameChange((const char*)stack->getItem(3)->data); break;
        case ONSETUSERDATA:    cb->onSetUserData((const char*)stack->getItem(3)->data); break;
        case ONSETTOOLTIP:     cb->onSetTooltip((const char *)stack->getItem(3)->data); break;
        case ONINSERTCHILD:    cb->onInsertChild((vConfigList*)stack->getItem(3)->data, (vConfigList*)stack->getItem(4)->data); break;
        case ONREMOVECHILD:    cb->onRemoveChild((vConfigList*)stack->getItem(3)->data); break;
        case ONMOVECHILD:      cb->onMoveChild((vConfigList*)stack->getItem(3)->data, (vConfigList*)stack->getItem(4)->data); break;
        case ONINSERTPARAM:    cb->onInsertParam((vConfigParam*)stack->getItem(3)->data, (vConfigParam*)stack->getItem(4)->data); break;
        case ONREMOVEPARAM:    cb->onRemoveParam((vConfigParam*)stack->getItem(3)->data); break;
        case ONSETPARAM:       cb->onSetParam((vConfigParam*)stack->getItem(3)->data, (vConfigParam*)stack->getItem(4)->data); break;
        case ONSETVISIBLE:     cb->onSetVisible(stack->getItem(3)->data); break;
        case ONSETEXPANDED:    cb->onSetExpanded(stack->getItem(3)->data); break;
        case ONSETHIghLIGHTED: cb->onSetHilighted(stack->getItem(3)->data); break;
        case ONSETENABLED:     cb->onSetEnabled(stack->getItem(3)->data); break;
        case ONSETSAVABLE:     cb->onSetSavable(stack->getItem(3)->data); break;
      }
    } break;
#endif //RMX_IPCLOADER

    case 4: {
      HPARAM param = (HPARAM) stack->getItem(2)->data;
      if (!param) return 0;
      if (!valid(param)) return 0;
      VCONFIGPARAM parammsg = (VCONFIGPARAM) stack->getItem(1)->data;
      switch (parammsg) {
        case PARAMNOTHING:   break;
        case GETPRMNAME:     return (UINT) sendString(plugin->getListParamName(param));
//      case SETPRMNAME:     param->setName((const char*)msg->param1); break;
        case SETPRMNAME:     break; //reserved
        case GETPRMVALUE:    return (UINT) sendString(plugin->getListParamValue(param));
        case SETPRMVALUE:    plugin->setListParamValue(param, stack->getItem(3)->str); break;
        case GETPRMVALUEINT: return (UINT) plugin->getListParamValueInt(param);
        case SETPRMVALUEINT: plugin->setListParamValueInt(param, (int)stack->getItem(3)->data); break;
        case GETPRMFLAGS:    return (UINT) plugin->getListParamFlags(param);
        case SETPRMFLAGS:    plugin->setListParamFlags(param, stack->getItem(3)->data); break;
      }
    } break;
    

#ifdef RMX_IPCLOADER //this needs to be removed eventually
    case 5: {
      if ((int)stack->getItem(1)->data == -1) return (UINT) new ipcRootListCallback(plugin, stack->getItem(3)->data, (vSecureParser*)stack->getItem(4)->data);
      
      VCONFIGROOT rootmsg = (VCONFIGROOT) stack->getItem(1)->data;
      switch (rootmsg) {
        case ROOTNOTHING:   break;

        case ROOTGETFIRST: {
            HLIST list = plugin->getFirstConfigList();
            while (list) {
              if (valid(list)) return (UINT) list;
              list = plugin->getNextConfigList(list);
            }
          } return NULL;

//        case ROOTGETLAST:   return (UINT) plg->getMasterPlugin()->getLastConfigList(); //configListList.getLastItem();
//        case ROOTGETPREV:   return (UINT) plg->getMasterPlugin()->getPrevConfigList((HLIST)stack->getItem(2)->data); //configListList.getPrevItem((vConfigList*)msg->param0);
        case ROOTGETNEXT: {
            HLIST list = plugin->getNextConfigList((HLIST)stack->getItem(2)->data);
            while (list) {
              if (valid(list)) return (UINT) list;
              list = plugin->getNextConfigList(list);
            }
          } return NULL;
          
        //case ROOTGETTOTAL:  return (UINT) plg->getMasterPlugin()->getFirstConfigList(); //configListList.getItemCount();
        //case ROOTITEMEXIST: return (UINT) plg->getMasterPlugin()->getFirstConfigList(); //configListList.itemExists((vConfigList*)msg->param0);
        case ROOTGETLIST:   return (UINT) plg->getMasterPlugin()->getConfigList(stack->getItem(2)->str, stack->getItem(3)->str, stack->getItem(4)->str); //configListList.getConfigList((const char *)msg->param0, (const char*)msg->param1, (const char*)msg->param2);

//        case ONNEWROOTLIST: if (plugin) plugin->onInsertRootList((HLIST)stack->getItem(2)->data, (HLIST)stack->getItem(3)->data);  break;
      }

      vRootListCallback *cb = (vRootListCallback*) stack->getItem(2)->data;
      if (!cb) break;
      switch (rootmsg) {
        case -2:  delete cb;  break;
        case ONNEWROOTLIST: cb->onInsertRootList((HLIST)stack->getItem(3)->data, (HLIST)stack->getItem(4)->data);  break;
      }
    } break;
#endif //RMX_IPCLOADER

  }


  return 0;
}
*/

//----------------------------------------------------------------------------------------------------------------


UINT vSecureParser::RmxMsg(UINT type, UINT message) const {
  vRmxStack stack;
  stack.appendItem(type);
  stack.appendItem(message);
  return stack.build(this);
}

UINT vSecureParser::RmxMsg(UINT type, UINT message, UINT param1) const {
  vRmxStack stack;
  stack.appendItem(type);
  stack.appendItem(message);
  stack.appendItem(param1);
  return stack.build(this);
}

UINT vSecureParser::RmxMsg(UINT type, UINT message, UINT param1, UINT param2) const {
  vRmxStack stack;
  stack.appendItem(type);
  stack.appendItem(message);
  stack.appendItem(param1);
  stack.appendItem(param2);
  return stack.build(this);
}

UINT vSecureParser::RmxMsg(UINT type, UINT message, UINT param1, UINT param2, UINT param3) const {
  vRmxStack stack;
  stack.appendItem(type);
  stack.appendItem(message);
  stack.appendItem(param1);
  stack.appendItem(param2);
  stack.appendItem(param3);
  return stack.build(this);
}

UINT vSecureParser::RmxMsg(UINT type, UINT message, UINT param1, UINT param2, UINT param3, UINT param4) const {
  vRmxStack stack;
  stack.appendItem(type);
  stack.appendItem(message);
  stack.appendItem(param1);
  stack.appendItem(param2);
  stack.appendItem(param3);
  stack.appendItem(param4);
  return stack.build(this);
}

UINT vSecureParser::RmxMsg(UINT type, UINT message, UINT param1, UINT param2, UINT param3, UINT param4, UINT param5) const {
  vRmxStack stack;
  stack.appendItem(type);
  stack.appendItem(message);
  stack.appendItem(param1);
  stack.appendItem(param2);
  stack.appendItem(param3);
  stack.appendItem(param4);
  stack.appendItem(param5);
  return stack.build(this);
}

UINT vSecureParser::RmxMsg(UINT type, UINT message, UINT param1, UINT param2, UINT param3, UINT param4, UINT param5, UINT param6) const {
  vRmxStack stack;
  stack.appendItem(type);
  stack.appendItem(message);
  stack.appendItem(param1);
  stack.appendItem(param2);
  stack.appendItem(param3);
  stack.appendItem(param4);
  stack.appendItem(param5);
  stack.appendItem(param6);
  return stack.build(this);
}



UINT vSecureParser::RmxMsg(UINT type, UINT message, const char *param1) const {
  vRmxStack stack;
  stack.appendItem(type);
  stack.appendItem(message);
  stack.appendItem(param1);
  return stack.build(this);
}

UINT vSecureParser::RmxMsg(UINT type, UINT message, UINT param1, const char *param2) const {
  vRmxStack stack;
  stack.appendItem(type);
  stack.appendItem(message);
  stack.appendItem(param1);
  stack.appendItem(param2);
  return stack.build(this);
}

UINT vSecureParser::RmxMsg(UINT type, UINT message, UINT param1, UINT param2, const char *param3) const {
  vRmxStack stack;
  stack.appendItem(type);
  stack.appendItem(message);
  stack.appendItem(param1);
  stack.appendItem(param2);
  stack.appendItem(param3);
  return stack.build(this);
}

UINT vSecureParser::RmxMsg(UINT type, UINT message, UINT param1, const char *param2, UINT param3) const {
  vRmxStack stack;
  stack.appendItem(type);
  stack.appendItem(message);
  stack.appendItem(param1);
  stack.appendItem(param2);
  stack.appendItem(param3);
  return stack.build(this);
}

UINT vSecureParser::RmxMsg(UINT type, UINT message, UINT param1, const char *param2, const char *param3) const {
  vRmxStack stack;
  stack.appendItem(type);
  stack.appendItem(message);
  stack.appendItem(param1);
  stack.appendItem(param2);
  stack.appendItem(param3);
  return stack.build(this);
}

UINT vSecureParser::RmxMsg(UINT type, UINT message, const char *param1, const char *param2, const char *param3) const {
  vRmxStack stack;
  stack.appendItem(type);
  stack.appendItem(message);
  stack.appendItem(param1);
  stack.appendItem(param2);
  stack.appendItem(param3);
  return stack.build(this);
}

UINT vSecureParser::RmxMsg(UINT type, UINT message, UINT param1, const char *param2, const char *param3, const char *param4) const {
  vRmxStack stack;
  stack.appendItem(type);
  stack.appendItem(message);
  stack.appendItem(param1);
  stack.appendItem(param2);
  stack.appendItem(param3);
  stack.appendItem(param4);
  return stack.build(this);
}


#endif //0
