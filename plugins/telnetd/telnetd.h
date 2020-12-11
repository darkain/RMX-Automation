/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/plugBase.h"
#include "../../sdk/Win32/Timer.h"
#include "../../sdk/cons/consBase.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/cb/cbBase.h"
#include "connect.h"


class vBugfix;


enum PROMPT {
  PROMPT_NONE,
  PROMPT_USERNAME,
  PROMPT_USERWAIT,
  PROMPT_PASSWORD,
  PROMPT_PASSWAIT,
  PROMPT_HALT,
  PROMPT_OK,
};


class vTelnetd : public plugBase, public vServer {
  public:
    vTelnetd();
    virtual ~vTelnetd() {}

    void getList();

  protected:
    virtual void onInit();
    virtual void onQuit();

    virtual void cb_onDelete() {}  //do nothing
    virtual void cb_onSetUserData(const prmBase *userdata);

    virtual vConnect *newConnection();
    virtual void onClientDisconnected(vConnect *c);

  public:
    const char *getUsername() const { return username->getText(); }
    const char *getPassword() const { return password->getText(); }

  private:
    vBugfix *bugfix;
    cfgBase *tree;
    cfgInt  *port;
    cfgText *username;
    cfgText *password;
};


class vTelnetClient : public vConnect, public consBase, public vTimer {
  public:
    vTelnetClient(const char *name, vServer *server);
    virtual ~vTelnetClient();

    virtual int onRead();
    virtual void onConnected();

    virtual BOOL processCommand(unsigned char *command);

    void echoLineBuffer();

    virtual void print(const char *s);

    void clearLine();
    inline void clearConsole() { SendNoSep("\033[2J"); }

    virtual void onTimer(DWORD systemTime);


    HFUNCTION getFunction(char *name);
    HFUNCTION getFunctionChild(HFUNCTION function, const char *name);
    void echoName(HFUNCTION function);

  private:
    char *buffer;
    PROMPT prompt;
    bool echo;
    int ignore;
    char linepos;
    char linebuf[256];
    char history[256];

    rmxName username;
    rmxName password;
};


class vBugfix : public vTimer {};
