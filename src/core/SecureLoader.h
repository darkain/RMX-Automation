/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/

#if 0

#ifndef __SECUREPLUGIN_H__
#define __SECUREPLUGIN_H__


#include <Core/RMX DLL LIB/DllEnums.h>
#include "Plugin.h"
#include "SecureParser.h"
#include "FunctionCall.h"


class vSecurePlugin : public vPlugin, public vSecureParser {
  friend class vCopyData;

  protected:
    vSecurePlugin(UINT client);
  public:
    virtual ~vSecurePlugin();

  public:
//    virtual void onInit() {}
//    virtual void onPostInit() {}

    virtual const char *getName() const { return (const char*) RmxMsg(1, 9); }
//    const char *getFilename() const { return filename.getName(); }
    virtual const char *getPathRootName() { return "IPC"; }
//    HMODULE gethMod() const { return hMod; }
    virtual UINT getStatus() { return 1; }  //will be updated with status stuff later, maybe?

    virtual BOOL removable() { return TRUE; }
    virtual BOOL external()  { return TRUE; }

    virtual UINT getSdkVersion()    const { return (UINT) RmxMsg(1, 400); }
    virtual UINT getLibVersion()    const { return (UINT) RmxMsg(1, 401); }
    virtual UINT getPluginVersion() const { return (UINT) RmxMsg(1, 402); }

//    virtual void onFirstLoad();
//    virtual void onPreSaveSettings();
//    virtual void onPreLoadSettings();
//    virtual void onSaveSettings();
//    virtual void onLoadSettings();

//    virtual HRMX getApplicationID()      { return (HRMX)    RmxMsg(1, 0x08); }
//    virtual HPLUGIN getPluginID() const  { return (HPLUGIN) RmxMsg(1, 0x05); }
//    virtual void setPluginID(HPLUGIN id) { RmxMsg(1, 0x04, (UINT)id);  vPlugin::setPluginID(id); }

#ifdef RMX_STANDALONE
    virtual UINT onProcessMessage(vFunctionCall *function) { return RmxMsg(1, 16, function->getFunction()); }
    virtual const char *getMessageName(vFunctionCall *function) { return (const char*) RmxMsg(1, 18, function->getFunction()); }
#endif
//    virtual void activateMenu(HLIST list) { RmxMsg(VMSG::CONFIGLIST, ACTIVATEMENU, (UINT)list); }

//    virtual void activateScriptFunction(HLIST list, vScriptStack *stack) { RmxMsg(VMSG::CONFIGLIST, ACTIVATESCRIPT, (UINT)list, (UINT)stack); }
//    virtual void setScriptTableCallback(HLIST list, vScriptTableCallback *callback) {}

//    virtual void onInsertRootList(HLIST list, HLIST insert) { vPlugin::onInsertRootList(list, insert); } //specifically skip vSecureParser

//    virtual UINT onPluginMessage(HPLUGIN sender, UINT msg, UINT param0, UINT param1) { return 0; }
//    virtual UINT onPluginMessagePtr(HPLUGIN sender, UINT msg, void *data, UINT size) { return 0; }


//this is the full API that must be inherited from this class.  i may make them pure virtuals at some point
//---------------------------------------------------------------------------------------------------------------
/*
    //get root lists
    virtual HLIST getConfigList(const char *name, const char *type="*", const char *unique=NULL) const { return (HLIST) RmxMsg(5, ROOTGETLIST, name, type, unique); }
    virtual HLIST getFirstConfigList() const { return (HLIST) RmxMsg(5, ROOTGETFIRST); }
    virtual HLIST getNextConfigList(HLIST list) const { return (HLIST) RmxMsg(5, ROOTGETNEXT, (UINT)list); }
*/
    //get the root list
    virtual HLIST getConfigListRoot() const { return (HLIST) RmxMsg(VMSG::SYSTEM, 11); }

    //get children of a list
    virtual int getConfigChildCount(HLIST list) { return (int) RmxMsg(VMSG::CONFIGLIST, GETCHILDCOUNT, (UINT)list); }
    virtual HLIST getFirstConfigChild(HLIST list) const { return (HLIST) RmxMsg(VMSG::CONFIGLIST, GETFIRSTCHILD, (UINT)list); }
    virtual HLIST getNextConfigChild(HLIST list, HLIST child) const { return (HLIST) RmxMsg(VMSG::CONFIGLIST, GETNEXTCHILD, (UINT)list, (UINT)child); }
    virtual HLIST getConfigChild(HLIST list, const char *name, const char *type="*", const char *unique=NULL) const { return (HLIST) RmxMsg(VMSG::CONFIGLIST, GETCHILD, (UINT)list, name, type, unique); }
    virtual HLIST newConfigChild(HLIST list, const char *name, const char *type, const char *unique) { return (HLIST) RmxMsg(VMSG::CONFIGLIST, NEWEXTERNALCHILD, (UINT)list, name, type, unique); }
    virtual BOOL  hasConfigChild(HLIST list, HLIST child, BOOL recursive=FALSE) { return RmxMsg(VMSG::CONFIGLIST, HASCHILD, (UINT)list, (UINT)child, recursive); }

    //get parents of a list
//    virtual HLIST getConfigListRoot(HLIST list) const { return (HLIST) RmxMsg(VMSG::CONFIGLIST, GETROOT, (UINT)list); }
    virtual HLIST getConfigListParent(HLIST list) const { return (HLIST) RmxMsg(VMSG::CONFIGLIST, GETPARENT, (UINT)list); }

    //primary list information
    virtual const char *getConfigListName(HLIST list) const { return (const char*) RmxMsg(VMSG::CONFIGLIST, GETNAME, (UINT)list); }
    virtual const char *getConfigListType(HLIST list) const { return (const char*) RmxMsg(VMSG::CONFIGLIST, GETTYPE, (UINT)list); }
    virtual const char *getConfigListUnique(HLIST list) const { return (const char*) RmxMsg(VMSG::CONFIGLIST, GETUNIQUE, (UINT)list); }
    virtual const char *getConfigListTooltip(HLIST list) const { return (const char*) RmxMsg(VMSG::CONFIGLIST, GETTOOLTIP, (UINT)list); }
    virtual UINT getConfigListFlags(HLIST list) { return RmxMsg(VMSG::CONFIGLIST, GETLISTFLAGS, (UINT)list); }
    virtual void deleteConfigList(HLIST list) { RmxMsg(VMSG::CONFIGLIST, DELETELIST, (UINT)list); }

    //list attributes
    virtual BOOL getConfigListVisible(HLIST list)    const     { return RmxMsg(VMSG::CONFIGLIST, ISVISIBLE,     (UINT)list); }
    virtual void setConfigListVisible(HLIST list, BOOL visible)       { RmxMsg(VMSG::CONFIGLIST, SETVISIBLE,     (UINT)list, visible); }
    virtual BOOL getConfigListEnabled(HLIST list)    const     { return RmxMsg(VMSG::CONFIGLIST, ISENABLED,     (UINT)list); }
    virtual void setConfigListEnabled(HLIST list, BOOL enable)        { RmxMsg(VMSG::CONFIGLIST, SETENABLED,     (UINT)list, enable); }
    virtual BOOL getConfigListHighlighted(HLIST list)  const   { return RmxMsg(VMSG::CONFIGLIST, ISHIGHLIGHTED, (UINT)list); }
    virtual void setConfigListHighlighted(HLIST list, BOOL highlight) { RmxMsg(VMSG::CONFIGLIST, SETHIGHLIGHTED, (UINT)list, highlight); }
    virtual BOOL getConfigListExpanded(HLIST list)   const     { return RmxMsg(VMSG::CONFIGLIST, ISEXPANDED,    (UINT)list); }
    virtual void setConfigListExpanded(HLIST list, BOOL expanded)     { RmxMsg(VMSG::CONFIGLIST, SETEXPANDED,    (UINT)list, expanded); }
    virtual BOOL getConfigListSavable(HLIST list)    const     { return RmxMsg(VMSG::CONFIGLIST, ISSAVABLE,     (UINT)list); }
    virtual void setConfigListSavable(HLIST list, BOOL savable)       { RmxMsg(VMSG::CONFIGLIST, SETSAVABLE,     (UINT)list, savable); }

    //list user data
    virtual const char *getConfigListUserData(HLIST list) { return (const char*) RmxMsg(VMSG::CONFIGLIST, GETUSERDATA, (UINT)list); }
    virtual const char *getConfigListUserDataSafe(HLIST list, const char *safe="") { return (const char*) RmxMsg(VMSG::CONFIGLIST, GETUSERDATASAFE, (UINT)list, safe); }
    virtual const char *setConfigListUserData(HLIST list, const char *data) { return (const char*) RmxMsg(VMSG::CONFIGLIST, SETUSERDATA, (UINT)list, data); }
    virtual int getConfigListUserDataInt(HLIST list) { return (int) RmxMsg(VMSG::CONFIGLIST, GETUSERDATAINT, (UINT)list); }
    virtual int setConfigListUserDataInt(HLIST list, int data) { return (int) RmxMsg(VMSG::CONFIGLIST, SETUSERDATAINT, (UINT)list, data); }

    //get and set param value from a list
    virtual const char *getConfigListParam(HLIST list, const char *name) const { return (const char*) RmxMsg(VMSG::CONFIGLIST, GETPARAM, (UINT)list, name); }
    virtual const char *getConfigListParamSafe(HLIST list, const char *name, const char *safe="") const { return (const char*) RmxMsg(VMSG::CONFIGLIST, GETPARAMSAFE, (UINT)list, name, safe); }
    virtual int getConfigListParamInt(HLIST list, const char *name) const { return (int) RmxMsg(VMSG::CONFIGLIST, GETPARAMINT, (UINT)list, name); }
    virtual void setConfigListParam(HLIST list, const char *name, const char *value) { RmxMsg(VMSG::CONFIGLIST, SETPARAM, (UINT)list, name, value); }
    virtual void setConfigListParamInt(HLIST list, const char *name, int value) { RmxMsg(VMSG::CONFIGLIST, SETPARAMINT, (UINT)list, name, value); }
/*  gotta implement this section
    //add and remove params from a list
    virtual HPARAM insertConfigListParam(HLIST list, const char *name, const char *value, HPARAM insert=NULL) { return NULL; }
    virtual HPARAM insertConfigListParamInt(HLIST list, const char *name, int value, HPARAM insert=NULL) { return NULL; }
    virtual HPARAM insertConfigListParamNoCfg(HLIST list, const char *name, const char *value, HPARAM insert=NULL) { return NULL; }
    virtual HPARAM insertConfigListParamIntNoCfg(HLIST list, const char *name, int value, HPARAM insert=NULL) { return NULL; }
    virtual void removeConfigListParam(HLIST list, HPARAM param) {}
    virtual void removeConfigListParam(HLIST list, const char *name) {}
*/
    //get param data from a list
    virtual int getConfigListParamCount(HLIST list) { return (int) RmxMsg(VMSG::CONFIGLIST, GETPARAMCOUNT, (UINT)list); }
    virtual HPARAM getConfigListParamFirst(HLIST list) const { return (HPARAM) RmxMsg(VMSG::CONFIGLIST, GETFIRSTPARAM, (UINT)list); }
    virtual HPARAM getConfigListParamNext(HLIST list, HPARAM param) const { return (HPARAM) RmxMsg(VMSG::CONFIGLIST, GETNEXTPARAM, (UINT)list, (UINT)param); }
    virtual HPARAM getConfigListParamData(HLIST list, const char *name) const { return (HPARAM) RmxMsg(VMSG::CONFIGLIST, GETPARAMDATA, (UINT)list, name); }
    
    //get info about a specific param
    virtual const char *getListParamName(HPARAM param) const { return (const char*) RmxMsg(4, GETPRMNAME, (UINT)param); }
    virtual const char *getListParamValue(HPARAM param) const { return (const char*) RmxMsg(4, GETPRMVALUE, (UINT)param); }
    virtual int getListParamValueInt(HPARAM param) const { return (int) RmxMsg(4, GETPRMVALUEINT, (UINT)param); }
    virtual UINT getListParamFlags(HPARAM param) const  { return (UINT) RmxMsg(4, GETPRMFLAGS, (UINT)param); }
    virtual BOOL isListParamNULL(HPARAM param) const    { return RmxMsg(4, ISPRMNULL,    (UINT)param); }
    virtual BOOL isListParamGUID(HPARAM param) const    { return RmxMsg(4, ISPRMGUID,    (UINT)param); }
    virtual BOOL isListParamInteger(HPARAM param) const { return RmxMsg(4, ISPRMINT,     (UINT)param); }

    //set info about a specific param
//    virtual void setListParamName(HPARAM param, const char *name) {}
    virtual void setListParamValue(HPARAM param, const char *value) { RmxMsg(4, SETPRMVALUE, (UINT)param, value); }
    virtual void setListParamValueInt(HPARAM param, int value) { RmxMsg(4, SETPRMVALUEINT, (UINT)param, value); }

    //encoder interface
    virtual HLIST encoderGetInterface(HLIST list) { return (HLIST) RmxMsg(VMSG::CONFIGLIST, GETENCODERINTERFACE, (UINT)list); }
    virtual void encoderEncodeData(HLIST list, const BYTE *data, UINT len) { RmxMsg(VMSG::CONFIGLIST, ENCODEDATA, (UINT)list, (UINT)data, len); }
    virtual const BYTE *encoderGetData(HLIST list) const { return (const BYTE*) RmxMsg(VMSG::CONFIGLIST, GETENCODERDATA, (UINT)list); }
    virtual UINT encoderGetLength(HLIST list) const { return RmxMsg(VMSG::CONFIGLIST, GETENCODERLENGTH, (UINT)list); }

    //register a callback with a list
    virtual void *registerListCallback(  rmxListCallback *callback);
    virtual void  deregisterListCallback(rmxListCallback *callback);
};


#endif //__SECUREPLUGIN_H__

#endif //0
