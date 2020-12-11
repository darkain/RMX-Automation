/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include <RMX DLL.h>
#include <vXML.h>
#include <vConfigList/Config.h>
#include <Win32/Timer.h>

#include <stdlib.h>
#include <winsock2.h>
#include <../jnetlib/jnetlib.h>


#if defined _DEBUG || defined _BETA
#  define UPDATE_URL "http://iceberg.serveftp.org:31080/rmx/updates/"
#else
#  define UPDATE_URL "http://rmx.darkain.com/updates/"
#endif


char *UrlEncode(const char *url);
void UserAgent(JNL_HTTPGet *http);


class vUpdateTree;


class Updater : public vPlug {
  public:
    Updater();

  protected:
    virtual void onInit();
    virtual void onPostInit();

    virtual void onQuit();

  public:
    inline void setUpdateDate(int date) { lastupdate->setValue(date); }
    inline int getUpdateDate() const { return lastupdate->getValue(); }

    inline vUpdateTree *getTree() { return tree; }
    
    inline BOOL needRestart() const { return restart; }
    inline void setNeedRestart(BOOL res) { restart = res; }

  private:
    BOOL         restart;
    vUpdateTree *tree;
    vConfigInt  *lastcheck;
    vConfigInt  *lastupdate;
};


/////////////////////////////////////////////////////////////////////////


class vUpdateTree : public vConfigTree, public vTimer {
  public:
    vUpdateTree();
    virtual ~vUpdateTree();

  public:
    void update();
    void updateText(int num);

  protected:
    virtual void onTimer(DWORD systemTime);

  public:
    virtual void onClick(HPLUGIN source);

    inline vConfigButton *getButton() { return button; }

  private:
    JNL_HTTPGet    http;
    FILE          *file;
    rmxName        filename;

    vConfigButton *button;
    vConfigNull   *text;
};


/////////////////////////////////////////////////////////////////////////


class vUpdateButton : public vConfigButton {
  public:
    vUpdateButton(vUpdateTree *parent) : vConfigButton("Apply the following RM-X updates", parent) {
      setButtonText("Update");
      tree = parent;
    }

    virtual ~vUpdateButton() {
    }

  protected:
    virtual void onClick(HPLUGIN source) {
      tree->onClick(source);
    }

  private:
    vUpdateTree *tree;
};


/////////////////////////////////////////////////////////////////////////


class vUpdateItem : public vConfigBool, public vTimer {
  public:
    vUpdateItem(const char *name, vConfigTree *parent);
    virtual ~vUpdateItem();

  protected:
    virtual void onTimer(DWORD systemTime);
    virtual void onSetParam(const vConfigParam *param);

  public:
    virtual void onUpdateComplete();

    void update();
    void extract();

    inline void setCurrentVersion(int ver) { text_cur->setUserDataInt(ver); }
    inline void setUpdateVersion( int ver) { text_new->setUserDataInt(ver); }

    static void startUpdate();
    static int  getNumItems() { return items.getItemCount(); }

    void setStatus(const char *text) { status->setName(text); }

    inline const char *getOldName() const { return filename.getName(); }

  private:
    vConfigText *text_cur;
    vConfigText *text_new;
    vConfigNull *status;

    JNL_HTTPGet http;
    FILE       *file;
    rmxName     filename;
    int         filelen;

    unsigned char rmxhash[16];

    static rmxList<vUpdateItem*> items;
};


/////////////////////////////////////////////////////////////////////////


class vUpdateReader : public vXmlReader {
  public:
    vUpdateReader(const char *filename, vUpdateTree *parent);
    virtual ~vUpdateReader();

  protected:
    virtual void onElementStart(const char *xmlpath, const char *xmltag, vXmlParams *params);

  private:
    vUpdateTree *tree;
};


/////////////////////////////////////////////////////////////////////////


class vLoadPluginTimer : public vTimer {
  public:
    vLoadPluginTimer(vUpdateItem *update) : vTimer(10, TRUE), item(update) {}
    virtual ~vLoadPluginTimer() {}

  protected:
    virtual void onTimer(DWORD systemTime);

  private:
    vUpdateItem *item;
};
