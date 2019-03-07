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
#include "../../sdk/func/funcSingle.h"
#include "../../sdk/func/funcGroup.h"

//#include <vBaDK/vScript/ScriptFunction.h>
//#include <vBaDK/vScript/ScriptTable.h>


class test : public plugBase {
  public:
    test();

  protected:
    virtual void onInit();
    virtual void onQuit();

  private:
    void dynamictest();
    void childrentest();
    void datatypetest();
    void movetest();
    void makeevents();
    void testmenu();
    void functiontest();
//    void scripttest();

  private:
    cfgTree *root;
    funcGroup *froot;
//    vScriptObject *script_table;
//    vScriptObject *script_function;
 //   vScriptObject *script_vargs;
};


class cfgTreeTitleChange : public cfgTree, public vTimer {
  public:
    cfgTreeTitleChange(const char *itemName, cfgTree *par)
      : cfgTree(itemName, par), vTimer(100, TRUE)  {}
    virtual ~cfgTreeTitleChange() {}
    
    virtual void onTimer(DWORD systemTime) {
      char str[16];
      VPRINTF(str, sizeof(str), "%d", systemTime);
      setTitle(str);
    }
};


class cfgTreeNameChange : public cfgTree, public vTimer {
  public:
    cfgTreeNameChange(const char *itemName, cfgTree *par)
      : cfgTree(itemName, par), vTimer(100, TRUE)  {}
    virtual ~cfgTreeNameChange() {}
    
    virtual void onTimer(DWORD systemTime) {
      char str[16];
      VPRINTF(str, sizeof(str), "%d", systemTime);
      setName(str);
    }
};


class cfgButtonNameChange : public cfgButton, public vTimer {
  public:
    cfgButtonNameChange(const char *itemName, cfgTree *par)
      : cfgButton(itemName, par), vTimer(100, TRUE)  {}
    virtual ~cfgButtonNameChange() {}
    
    virtual void onTimer(DWORD systemTime) {
      char str[16];
      VPRINTF(str, sizeof(str), "%d", systemTime);
      setName(str);
    }
};


class cfgButtonTextChange : public cfgButton, public vTimer {
  public:
    cfgButtonTextChange(const char *itemName, cfgTree *par)
      : cfgButton(itemName, par), vTimer(100, TRUE)  {}
    virtual ~cfgButtonTextChange() {}
    
    virtual void onTimer(DWORD systemTime) {
      char str[16];
      VPRINTF(str, sizeof(str), "%d", systemTime);
      setButtonText(str);
    }
};


class cfgButtonDescriptionChange : public cfgButton, public vTimer {
  public:
    cfgButtonDescriptionChange(const char *itemName, cfgTree *par)
      : cfgButton(itemName, par), vTimer(100, TRUE)  {}
    virtual ~cfgButtonDescriptionChange() {}
    
    virtual void onTimer(DWORD systemTime) {
      char str[16];
      VPRINTF(str, sizeof(str), "%d", systemTime);
      setButtonDescription(str);
    }
};


class cfgButtonEventChange : public cfgButton {
  public:
    cfgButtonEventChange(const char *itemName, cfgTree *par)
      : cfgButton(itemName, par) {}
    virtual ~cfgButtonEventChange() {}

    virtual void onClick(HPLUGIN source) {
      char str[16];
      VPRINTF(str, sizeof(str), "%d", GetTickCount());
      setButtonDescription(str);
    }
};


class cfgTextNameChange : public cfgText, public vTimer {
  public:
    cfgTextNameChange(const char *itemName, cfgTree *par)
      : cfgText(itemName, par), vTimer(100, TRUE)  {}
    virtual ~cfgTextNameChange() {}
    
    virtual void onTimer(DWORD systemTime) {
      char str[16];
      VPRINTF(str, sizeof(str), "%d", systemTime);
      setName(str);
    }
};


class cfgTextTextChange : public cfgText, public vTimer {
  public:
    cfgTextTextChange(const char *itemName, cfgTree *par)
      : cfgText(itemName, par), vTimer(100, TRUE) {}
    virtual ~cfgTextTextChange() {}
    
    virtual void onTimer(DWORD systemTime) {
      char str[16];
      VPRINTF(str, sizeof(str), "%d", systemTime);
      setText(str);
    }
};


class cfgTextFocusChange : public cfgText {
  public:
    cfgTextFocusChange(const char *itemName, cfgTree *par)
      : cfgText(itemName, par) { setText("untouched"); }
    virtual ~cfgTextFocusChange() {}
    virtual void onGainFocus(HPLUGIN source) { setText("onGainFocus()"); }
    virtual void onLostFocus(HPLUGIN source) { setText("onLostFocus()"); }
};


class cfgRemoveItem : public cfgButton {
  public:
    cfgRemoveItem(const char *initname, cfgBase *parent, cfgBase *insert=NULL)
      : cfgButton(initname, parent, insert) { setButtonText("Remove"); }
    virtual ~cfgRemoveItem() {}

    virtual void onClick(HPLUGIN source) {
      delete this;
    }
};


class cfgAddItem : public cfgButton {
  public:
    cfgAddItem(const char *initname, cfgBase *parent)
      : cfgButton(initname, parent) { setButtonText("Add"); }
    virtual ~cfgAddItem() {}

    virtual void onClick(HPLUGIN source) {
      char str[16];
      VPRINTF(str, sizeof(str), "%d", GetTickCount());
      new cfgRemoveItem(str, getParent());
    }
};


class cfgInsertItem : public cfgButton {
  public:
    cfgInsertItem(const char *initname, cfgBase *parent)
      : cfgButton(initname, parent) { setButtonText("Insert"); }
    virtual ~cfgInsertItem() {}

    virtual void onClick(HPLUGIN source) {
      char str[16];
      VPRINTF(str, sizeof(str), "%d", GetTickCount());
      new cfgRemoveItem(str, getParent(), this);
    }
};


class cfgAddTree : public cfgButton {
  public:
    cfgAddTree(const char *initname, cfgBase *parent)
      : cfgButton(initname, parent) { setButtonText("Add"); }
    virtual ~cfgAddTree() {}

    virtual void onClick(HPLUGIN source) {
      char str[16];
      VPRINTF(str, sizeof(str), "%d", GetTickCount());
      cfgTree *tree = new cfgTree(str, getParent());
      new cfgRemoveParent("Remove Tree", tree);
    }
};


class cfgHideTree : public cfgButton {
  public:
    cfgHideTree(const char *initname, cfgBase *parent)
      : cfgButton(initname, parent) {
        setButtonText("Show/Hide");
        child = new cfgTree("Visible", parent);
    }

    virtual ~cfgHideTree() {}

    virtual void onClick(HPLUGIN source) {
      child->setVisible( !child->isVisible() );
    }

  private:
    cfgBase *child;
};


class cfgAddChild : public cfgButton {
  public:
    cfgAddChild(const char *initname, cfgBase *parent)
      : cfgButton(initname, parent) { setButtonText("Add"); }
    virtual ~cfgAddChild() {}

    virtual void onClick(HPLUGIN source) {
      char str[16];
      VPRINTF(str, sizeof(str), "%d", GetTickCount());
      new cfgAddChild(str, this);
    }
};


class vSettingTest : public cfgButton {
  public:
    vSettingTest(evntSingle *e, cfgBase *parent) : cfgButton("Activate vSetting", parent) { event = e; }
    virtual ~vSettingTest() {}
    virtual void onClick(HPLUGIN source) { event->activate(); }
  private:
    evntSingle *event;
};


class cfgShowHideChild : public cfgButton {
  public:
    cfgShowHideChild(const char *initname, cfgBase *parent)
      : cfgButton(initname, parent) { setButtonText("Show/Hide"); }
    virtual ~cfgShowHideChild() {}

    virtual void onClick(HPLUGIN source) {
      cfgBase *list = getFirstChild();
      list = getNextChild(list);
      if (list) list->setVisible(!list->isVisible());
    }
};


class cfgMoveUp : public cfgButton {
  public:
    cfgMoveUp(const char *name, cfgBase *parent) : cfgButton(name, parent) { setButtonText("Move Up"); }
    virtual ~cfgMoveUp() {}
    virtual void onClick(HPLUGIN source) { moveListUp(); }
};

class cfgMoveDown : public cfgButton {
  public:
    cfgMoveDown(const char *name, cfgBase *parent) : cfgButton(name, parent) { setButtonText("Move Down"); }
    virtual ~cfgMoveDown() {}
    virtual void onClick(HPLUGIN source) { moveListDown(); }
};

class cfgMoveBegining : public cfgButton {
  public:
    cfgMoveBegining(const char *name, cfgBase *parent) : cfgButton(name, parent) { setButtonText("Move to Begining"); }
    virtual ~cfgMoveBegining() {}
    virtual void onClick(HPLUGIN source) { moveListToBegining(); }
};

class cfgMoveEnding : public cfgButton {
  public:
    cfgMoveEnding(const char *name, cfgBase *parent) : cfgButton(name, parent) { setButtonText("Move to Ending"); }
    virtual ~cfgMoveEnding() {}
    virtual void onClick(HPLUGIN source) { moveListToEnd(); }
};


class cfgTestMenu : public cfgMenu {
  public:
    cfgTestMenu(const char *name) : cfgMenu(name) {}
    virtual ~cfgTestMenu() {}
    virtual void onClick(HPLUGIN source) { MessageBox(0, L"cfgMenu", L"Test Plugin", 0); }
};


class funcTest : public funcSingle {
  public:
    funcTest(const char *function_name, const char *script_name, funcGroup *parent=NULL)
      : funcSingle(function_name, script_name, parent) {}
    virtual ~funcTest() {}
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {}
};

/*
class vTestScriptFunction : public vScriptFunction {
  public:
    vTestScriptFunction() : vScriptFunction("test") {}
    virtual ~vTestScriptFunction() {}
    virtual void onActivated(vScriptStack *stack) { MessageBox(0, "vScriptFunction", "Test Plugin", 0); }
};

class vTestScriptFunctionVargs : public vScriptFunction {
  public:
    vTestScriptFunctionVargs() : vScriptFunction("testvargs") {}
    virtual ~vTestScriptFunctionVargs() {}
    virtual void onActivated(vScriptStack *stack) {
      prmBase *param = getFirstParam();
      while (param) {
        MessageBox(0, param->getValue(), param->getName(), 0);
        param = getNextParam(param);
      }
    }
};

class vTestScriptTableFunction : public vScriptFunction {
  public:
    vTestScriptTableFunction(vScriptTable *parent) : vScriptFunction("subtest", parent) {}
    virtual ~vTestScriptTableFunction() {}
    virtual void onActivated(vScriptStack *stack) { MessageBox(0, "vScriptFunction inside of vScriptTable", "Test Plugin", 0); }
};


class vTestScriptTableChild : public vScriptTable {
  public:
    vTestScriptTableChild(vScriptTable *parent) : vScriptTable("childtable", parent) {}
    virtual ~vTestScriptTableChild() {}
};

class vTestScriptTable : public vScriptTable {
  public:
    vTestScriptTable() : vScriptTable("testtable") {
      new vTestScriptTableChild(this);
      new vTestScriptTableFunction(this);
      setTableValue("item1", "test");
      setTableValueInt("item2", 99);
    }
    virtual ~vTestScriptTable() {}
};
*/