/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/plugBase.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/cfg/cfgHelper.h"
#include "../../sdk/cfg/cfgAutolist.h"
#include "../../sdk/cfg/cfgWndHelper.h"
#include "../../sdk/evnt/evntSingle.h"
#include "../../sdk/wnd/wndSubclass.h"
#include "../../sdk/whk/whkCommand.h"


class ToolbarParentWnd;
class ToolbarWnd;
class ToolbarButton;

#define ToolbarTreeParent tcfgAutoEventList<cfgTree, ToolbarButton>


class Toolbar : public plugBase {
  public:
    Toolbar();

  protected:
    virtual void onInit();
    virtual void onQuit();

  public:
    inline ToolbarParentWnd *getWindow() { return wnd; }
    inline ToolbarTreeParent *getTree() { return tree; }

  private:
    ToolbarTreeParent *tree;
    ToolbarParentWnd *wnd;
};


class vToolbarOnTop : public cfgBool {
  public:
    vToolbarOnTop(cfgBase *parent, wndBase *window) : cfgBool("Always On Top", parent), wnd(window) { setValueInt(1); }
    virtual ~vToolbarOnTop() {}

    virtual void onSetUserData(const prmBase *newdata) {
      cfgBool::onSetUserData(newdata);
      if (getValue()) {
        SetWindowPos(wnd->getWnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
      } else {
        SetWindowPos(wnd->getWnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
      }
    }

  private:
    wndBase *wnd;
};


class vToolbarRoot : public ToolbarTreeParent {
  public:
    vToolbarRoot(const char *initname, cfgBase *parent) : ToolbarTreeParent(initname, parent) {}
    virtual ~vToolbarRoot() {}

  protected:
    virtual void onLoadSettings();
    virtual void onInsertChild(cfgBase *child, const cfgBase *insert);
    virtual void onRemoveChild(const cfgBase *child);
    virtual void onPreMoveChild(const cfgBase *child, const cfgBase *insert);
};


class ToolbarParentWnd : public wndBase, public whkBase {
  public:
    ToolbarParentWnd(const char *name);
    virtual ~ToolbarParentWnd();

  protected:
    virtual void onInit();
    virtual void onResize(int width, int height);

  public:
    inline ToolbarWnd *getToolbar() { return toolbar; }

  protected:
    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);
    virtual BOOL wndHookProc(whkInfo *info);


  private:
    ToolbarWnd *toolbar;
    wndBase *rebar;
};


class ToolbarWnd : public wndBase, public whkCommand {
  friend class ToolbarButton;

  public:
    ToolbarWnd(wndBase *parent);
    virtual ~ToolbarWnd();

  protected:
    virtual void onInit();

    virtual BOOL onCommand(whkInfo *info);
};


class ToolbarButton : public evntSingle {
  public:
    ToolbarButton(const char *name, cfgBase *parent, HPARAMLIST paramlist=NULL);
    virtual ~ToolbarButton();

  public:
    void setToolbarButtonText(const char *text);

  private:
    ToolbarWnd *toolbar;
    cfgText    *edit;
    static int btnCount;
};


class ToolbarText : public cfgText {
  public:
    ToolbarText(ToolbarButton *parent) : cfgText("Button Text", parent) { par=parent; }
    virtual ~ToolbarText() {}

    virtual void onSetUserData(const prmBase *userdata) {
      cfgText::onSetUserData(userdata);
      par->setToolbarButtonText(getText());
    }

    virtual void onLoadSettings() {
      cfgText::onLoadSettings();
      par->setToolbarButtonText(getText());
    }

  private:
    ToolbarButton *par;
};
