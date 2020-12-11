/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/plugBase.h"
#include "../../sdk/Win32/Timer.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/cfg/cfgHelper.h"
#include "../../sdk/evnt/evntSingle.h"
#include "../../sdk/cons/consBase.h"
#include "../../libraries/jnetlib/webserver.h"


class vWebServer : public plugBase {
  public:
    vWebServer();

  protected:
    virtual void onInit();
    virtual void onQuit();

  public:
    inline cfgTree *getTree() { return tree; }

  private:
    vTimer *server;
    cfgTree *tree;
};


class vWebTree : public cfgTree {
  public:
    vWebTree(const char *name, cfgBase *parent);
    virtual ~vWebTree();

  protected:
    virtual cfgBase *onNewExternalChild(const char *name, const char *type, HPARAMLIST paramlist);

  public:
    evntSingle *addEvent(const char *unique=NULL);
};

/*
class vQuickLink : public vEvent {
  public:
    vQuickLink(cfgBase *parent);
    virtual ~vQuickLink();
};
*/

class vAddWebButton : public cfgButton {
  public:
    vAddWebButton(vWebTree *parent) : cfgButton("Add Quick Link", parent) {
      setButtonText("Add");
      setSavable(FALSE);
      par = parent;
    }

    virtual ~vAddWebButton() {}

  protected:
    virtual void onClick(HPLUGIN source) {
      cfgButton::onClick(source);
      par->addEvent();
    }

  private:
    vWebTree *par;
};


class vMyServer : public WebServerBaseClass, public vTimer {
  public:
    vMyServer() : vTimer(1, TRUE) { addListenPort(8080); }
    virtual ~vMyServer() {}

  protected:
    virtual void onTimer(DWORD systemTime) { run(); }
    virtual IPageGenerator *onConnection(JNL_HTTPServ *serv, int port);
};


class vWebFile : public IPageGenerator {
  public:
    vWebFile(JNL_HTTPServ *server, const char *filename, FILE *handle=NULL);
    virtual ~vWebFile();

  public:
    inline JNL_HTTPServ *getServer() const { return serv; }
    inline FILE *getFile() const { return file; }

  protected:
    virtual int GetData(char *buf, int size);

  protected:
    rmxName name;
    FILE *file;
    JNL_HTTPServ *serv;
};


class vHome : public IPageGenerator {
  public:
    vHome(JNL_HTTPServ *serv);
    virtual ~vHome();

  protected:
    virtual int GetData(char *buf, int size);

    void enumList(HPLUGIN plugin, HLIST list, int depth=0);
    BOOL hasVisibleTree(HPLUGIN plugin, HLIST list);

    void generateLinks();
    void generatePane(HPLUGIN plugin, HLIST list);

    void printExpand(HPLUGIN plugin, HLIST list);
    void printButton(HPLUGIN plugin, HLIST list);
    void printLink(HPLUGIN plugin, HLIST list);
    void printString(HPLUGIN plugin, HLIST list);
    void printInt(HPLUGIN plugin, HLIST list);
    void printBool(HPLUGIN plugin, HLIST list);
    void printText(HPLUGIN plugin, HLIST list);

    void processCommand(JNL_HTTPServ *serv);
    void processSubmit(JNL_HTTPServ *serv);

    inline const char *prm() const { return params.getName(); }

  private:
    HPLUGIN vplugin;
    HLIST   vlist;
    rmxName name;
    rmxName params;
    FILE   *file;
};


struct postparam {
  char *param;
  char *value;
};


class vPostParam : public rmxFreeList<postparam*> {
  public:
    vPostParam() {}
    ~vPostParam() {}

  public:
    char *getParam(const char *name) {
      postparam *item = getFirstItem();
      while (item) {
        if (item->param  &&  VSTRCMP(item->param, name) == 0) return item->value;
        item = getNextItem(item);
      }
      return NULL;
    }

    virtual void onDelete(postparam *data) {
      free(data->value);
      rmxFreeList<postparam*>::onDelete(data);
    }
};


class vLuaFile : public vWebFile, public consBase {
  public:
    vLuaFile(JNL_HTTPServ *server, const char *luafile, FILE *tempfile);
    virtual ~vLuaFile();


  private:
    void parse(char *buffer, int len);
    void runScript(const char *s);

  protected:
    virtual void print(const char *s);
};