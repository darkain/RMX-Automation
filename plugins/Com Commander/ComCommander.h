/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/plugBase.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/evnt/evntPress.h"
#include "../../sdk/Win32/MessageWnd.h"
#include "../../sdk/Win32/Thread.h"


class ComTree;


class ComCommander : public plugBase {
  public:
    ComCommander();

  protected:
    virtual void onInit();
    virtual void onQuit();

  private:
    ComTree *root;
};


class ComTree : public cfgTree, public vMessageWnd, public rmxThread {
  public:
    ComTree(const char *name);
    virtual ~ComTree() {}

  protected:
    virtual int threadProc();
    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);

  private:
    evntPress *btn[4];
    HANDLE hcom;
};
