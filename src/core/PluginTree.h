/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __PLUGINTREE_H__
#define __PLUGINTREE_H__


#include "../../sdk/rmxString.h"
#include "../../sdk/cfg/config.h"


class vPluginManager;
class vPlugin;


class vPluginTree : public cfgTree {
  public:
    vPluginTree(cfgBase *parent);
    virtual ~vPluginTree();

  protected:
    virtual cfgBase *onNewExternalChild(const char *name, const char *type, HPARAMLIST paramlist);

  public:
    vPluginManager *getManager(const char *filename);
    vPluginManager *getManager(vPlugin *plugin);
    vPluginManager *addPlugin(const char *filename, const char *name="Unknown Plugin");

    void loadAll(BOOL init=FALSE);
    void unloadAll();

  private:
    rmxList<vPluginManager*> managers;
};


//------------------------------------------------------------------------------


class vPluginManager : public cfgBool {
  public:
    vPluginManager(const char *name, const char *file, vPluginTree *parent);
    virtual ~vPluginManager();

    BOOL load();
    void unload();

    const char *getFilename() const { return getParam("filename"); }
    vPlugin    *getPlugin()   const { return plugin;               }

  protected:
//    virtual void onSetUserData(const prmBase *userdata);
		virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert=NULL);

//    virtual void onPreLoadSettings();
//    virtual void onLoadSettings();

  private:
    BOOL failedMessage(const char *message);

  private:
    vPlugin  *plugin;
    rmxString filename;
    cfgLabel *pathname;
    cfgLabel *sdkBuild;
    cfgLabel *libBuild;
};


//------------------------------------------------------------------------------


class vPluginScanBtn : public cfgButton {
  public:
    vPluginScanBtn(vPluginTree *parent);
    virtual ~vPluginScanBtn() {}

    virtual void onClick(HPLUGIN source);

  private:
    vPluginTree *par;
};


#endif //__PLUGINTREE_H__
