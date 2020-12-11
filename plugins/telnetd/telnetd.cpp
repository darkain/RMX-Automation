/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


//TODO: PORT ME TO JNETLIB


//#define RAWOUT


#include "telnetd.h"


static vTelnetd *telnet = new vTelnetd();


vTelnetd::vTelnetd() : plugBase("Telnet Daemon") {
  bugfix   = NULL;
  tree     = NULL;
  port     = NULL;
  username = NULL;
  password = NULL;
}


void vTelnetd::onInit() {
  //the first instance of a timer must be created in the main thread

  bugfix = new vBugfix();

  tree     = new cfgTree(getName());
  port     = new cfgInt( "TCP Port", tree);
  new cfgLine(tree);
  new cfgNull("WARNING!! Password is stored as text with no encryption!", tree);
  username = new cfgText("Username", tree);
  password = new cfgText("Password", tree);

//  username->setNetSlave(FALSE);
//  username->setIpc(FALSE);
//  password->setNetSlave(FALSE);
//  password->setIpc(FALSE);

  WORD version = MAKEWORD(1,1);
  WSADATA wsaData;
  WSAStartup(version, &wsaData);

  port->insertCallback(this);
  port->setMinMax(1, 65535);
  port->setDefault(23);
  port->setValue(23);
}


void vTelnetd::onQuit() {
  port->removeCallback(this);
  vdelnull(bugfix);

  closeSocket();
  closeAllConnections(TRUE);
  WSACleanup();
}


void vTelnetd::cb_onSetUserData(const prmBase *userdata) {
  closeSocket();
  listenSocket( userdata->getValueInt() );
}


vConnect *vTelnetd::newConnection() {
  return new vTelnetClient("Telnet Client", this);
}



void vTelnetd::onClientDisconnected(vConnect *c) {
  vServer::onClientDisconnected(c);
}



//--------------------------------------------------------------


vTelnetClient::vTelnetClient(const char *name, vServer *server) : consBase(name), vTimer(1, FALSE) {
  setDeleteOnDisconnect(TRUE);
  ZeroMemory(linebuf, sizeof(linebuf));
  ZeroMemory(history, sizeof(history));
  linepos = 0;
  echo = false;
  prompt = PROMPT_NONE;
  ignore = 0;

  initConfig(); //TODO: temporary fix
}

vTelnetClient::~vTelnetClient() {
  End();
}

//253

void vTelnetClient::onConnected() {
  vConnect::onConnected();

  setThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);

  setName(getRemoteAddress());

  echo = true;

  //disable local echoing on client
  SendNoSep("\xff\xfb\x01");

  //force each character to be sent from client as they are typed
  SendNoSep("\xff\xfb\x03");

  prompt = PROMPT_USERNAME;
  echoLineBuffer();
}


int vTelnetClient::onRead() {
  int charsRead = recv(client, buffer, sizeof(buffer), 0);
  buffer[charsRead] = NULL;

  if (charsRead > 0) {
    for (int i=0; i<charsRead; i++) {
      if (ignore > 0) {
        ignore--;

      } else if (prompt==PROMPT_NONE  ||  prompt==PROMPT_HALT
         /*||  prompt==PROMPT_USERWAIT  ||  prompt==PROMPT_PASSWAIT*/) {
        //do nothing
      
      } else if (buffer[i] == 0x00) {  //NULL
        //do nothing

      } else if (buffer[i] == 0x03 || buffer[i] == 0x18) {  //CTRL+C
        closeSocket();
        return 0;

      } else if ( (buffer[i] == 0x08) || (buffer[i] == 0x7F) ) {  //BS - Backspace, DEL - Delete
        if (linepos > 0) {
          SendNoSep("\x08 \x08");
          linebuf[linepos] = NULL;
          linepos--;
        }

      } else if ( ((unsigned char)buffer[i]) == 0xFF ) {  //TELNET commands
        ignore = 2;

      } else if ( ((unsigned char)buffer[i]) == 0x1B ) {  //Escape Sequence
        ignore = 2;

        if (prompt == PROMPT_OK) {
          if (buffer[i]==0x1B && buffer[i+1]==0x4F) {  //function key was pressed
            if (buffer[i+2] == 0x52) {
              clearLine();
              VSTRCPY(linebuf, history, sizeof(linebuf));
              linepos = VSTRLEN(history);
              echoLineBuffer();
            }
          }
        }

#ifdef RAWOUT
        char hex[16];
        sprintf(hex, " %02X-%02X-%02X ", (char)buffer[i], (char)buffer[i+1], (char)buffer[i+2]);
        SendNoSep(hex);
#endif

      } else if ( (buffer[i] == 0x0A) || (buffer[i] == 0x0D) ) {
        if (linepos > 0) {
          linebuf[linepos] = NULL;
          linepos = 0;
          SendNoSep("\r\n");
          if      (prompt == PROMPT_USERNAME) prompt = PROMPT_USERWAIT;
          else if (prompt == PROMPT_PASSWORD) prompt = PROMPT_PASSWAIT;
          else                                prompt = PROMPT_HALT;
          startTimer();
        }

      } else {
        if (linepos < sizeof(linebuf)-1) {
          linebuf[linepos] = buffer[i];
          linepos++;
          linebuf[linepos] = NULL;
        }

        if (prompt == PROMPT_PASSWORD) {
          SendNoSep("*");

        } else if (echo) {
          char str[2] = {0, 0};
          str[0] = buffer[i];
          SendNoSep(str);

#ifdef RAWOUT
          char hex[8];
          sprintf(hex, " %02X ", (char)buffer[i]);
          SendNoSep(hex);
#endif
        }
      }
    }

    return 0;
  }
  return 10;
}


//we should thread jump to main thread
//or else we are gonna crashie crashie crashie
BOOL vTelnetClient::processCommand(unsigned char *command) {

  if (prompt == PROMPT_USERWAIT) {
    username.setName((const char*)command);
    prompt = PROMPT_PASSWORD;
    return TRUE;
  }

  else if (prompt == PROMPT_PASSWAIT) {
    password.setName((const char*)command);
/*
    HENCODER md5 = vHENCODER::getEncoderInterface("MD5");
    md5->encodeData( (const BYTE*)password.getName(), VSTRLEN(password.getName()) );
    const BYTE *data = md5->getEncodedData();
    UINT len = md5->getEncodedLength();
    char tmpstr[3];
    for (UINT i=0; i<len; i++) {
      VPRINTF(tmpstr, sizeof(tmpstr), "%02X", *data);
      SendNoSep(tmpstr);
      data++;
    }
    SendNoSep("\r\n");
    md5->releaseHandle();
*/
    if ( (VSTRCMP(username.getName(""), telnet->getUsername()) != 0) || (VSTRCMP(password.getName(""), telnet->getPassword()) != 0) ) {
      SendNoSep("Invalid User Name or Password\r\n\r\n");  //we need to flush the socket before disconnecting
      closeSocket();
      return TRUE;
    }

    clearConsole();
    char str[1024];
    VPRINTF(str, sizeof(str), "\033[1mWelcome to TELNET for RM-X General Purpose Control\r\nHost Version: %s\r\nServer Version: %s - (%d)\r\nType '\033[36mhelp\033[0;1m' for command reference\033[0m\r\n\r\n", "todo: FIX ME!!", telnet->getName(), telnet->getSdkVersion());
    SendRaw((unsigned char*)str, strlen(str));

    prompt = PROMPT_OK;
    return TRUE;
  }


  prompt = PROMPT_HALT;
  VSTRCPY(history, linebuf, sizeof(history));

  //parse to see if it is a local command
  const unsigned char *local = command;
  char *cmd = NULL;
  const unsigned char *params = NULL;
  while (*local) {
    if (*local == ' '  ||  *local == '(') {
      int len = local - command;
      cmd = (char*)malloc(len+1);
      memcpy(cmd, command, len);
      cmd[len] = NULL;
      params = local+1;
      break;
    }
    local++;
  }

  if (!cmd) {
    int len = local - command + 1;
    cmd = (char*) malloc(len);
    memcpy(cmd, command, len);
  }


  if (VSTRCMP((char*)cmd, "exit") == 0) {
    free(cmd);
    clearConsole();
    SendNoSep("Logging Out\r\n\r\n");  //todo: we need to flush the socket before disconnecting
    closeSocket(TRUE);
    return FALSE;
  }

  else if ( (VSTRCMP((char*)cmd, "help") == 0) || (VSTRCMP((char*)cmd, "?") == 0) ){
    SendNoSep("\r\n\033[1m /----------------------\\\r\n | RM-X TELNET COMMANDS |\r\n \\----------------------/\033[0m\r\n");

    if (params) {
      HFUNCTION object = getFunction((char*)params);
      if (object  &&  object->getType()==2) {

        SendNoSep(" \033[39m\033[1mFUNCTION OBJECTS FOR: \033[34m\033[1m");
        echoName(object);
        SendNoSep("\r\n\r\n");

        HFUNCTION script = object->child_getFirst();

        int count = 0;
        while (script) {
          const char *name = script->getScriptName();
          if (name && *name) {
            if (script->isVisible()) {
              count++;

              BOOL en = script->isEnabled();
              int type = script->getType();

              if (type == 1) {
                if (en) SendNoSep("\033[32m\033[1m "); else SendNoSep("\033[32m\033[0m ");
              } else if (type == 2) {
                if (en) SendNoSep("\033[34m\033[1m "); else SendNoSep("\033[34m\033[0m ");
              } else {
                if (en) SendNoSep("\033[31m\033[1m "); else SendNoSep("\033[31m\033[0m ");
              }
              SendNoSep(name);
              SendNoSep("\033[0m");
      
              const char *tip = script->getToolTip();
              if (tip) {
                if (VSTRLEN(name) < 7) SendNoSep("\t");
                if (en) SendNoSep("\t\033[1m"); else SendNoSep("\t\033[0m");
                SendNoSep(tip);
                SendNoSep("\033[0m");
              }

              SendNoSep("\r\n");
            }
          }
          script = object->child_getNext(script);
        }

        if (count == 0) {
          SendNoSep(" \033[31m\033[1mNO OBJECTS FOUND\r\n");
        } else {
          char str[32];
          VPRINTF(str, sizeof(str), "\r\n\033[39m\033[1m %d objects total\r\n", count);
          SendNoSep(str);
        }

      }

      else {
        SendNoSep("\033[31m\033[1m INVALID FUNCTION GROUP\r\n");
      }
    }

    else {

      SendNoSep("\033[32m\033[1m help\033[0m \t\t\033[1mThis help screen\033[0m\r\n");
      SendNoSep("\033[32m\033[1m exit\033[0m \t\t\033[1mCloses this TELNET session\033[0m\r\n");
    
      HFUNCTION script = vHFUNCTION::getFirst();
      while (script) {
        if (script->isVisible()) {
          const char *name = script->getScriptName();
          if (name && *name) {
            BOOL en = script->isEnabled();
            int type = script->getType();

            if (type == 1) {
              if (en) SendNoSep("\033[32m\033[1m "); else SendNoSep("\033[32m\033[0m ");
            } else if (type == 2) {
              if (en) SendNoSep("\033[34m\033[1m "); else SendNoSep("\033[34m\033[0m ");
            } else {
              if (en) SendNoSep("\033[31m\033[1m "); else SendNoSep("\033[31m\033[0m ");
            }
            SendNoSep(name);
            SendNoSep("\033[0m");
      
            const char *tip = script->getToolTip();
            if (tip) {
              if (VSTRLEN(name) < 7) SendNoSep("\t");
              if (en) SendNoSep("\t\033[1m"); else SendNoSep("\t\033[0m");
              SendNoSep(tip);
              SendNoSep("\033[0m");
            }

            SendNoSep("\r\n");
          }
        }
        script = vHFUNCTION::getNext(script);
      }
    }
    SendNoSep("\r\n");
  }

  else if (VSTRCMP((char*)cmd, "clear") == 0) {
    clearConsole();
  }

  else {
    telnet->consoleCommand((char*)command);
  }

  free(cmd);
  return TRUE;
}


void vTelnetClient::echoLineBuffer() {
  switch (prompt) {
    case PROMPT_OK:
      SendNoSep("\033[0;1m>\033[0m");
      if (linepos > 0) SendNoSep(linebuf);
      break;

    case PROMPT_USERNAME:
      SendNoSep("\033[1mUser Name: >\033[0m");
      break;

    case PROMPT_PASSWORD:
      SendNoSep("\033[1mPassword:  >\033[0m");
      prompt = PROMPT_PASSWORD;
      break;
  }
}


void vTelnetClient::print(const char *s) {
  if (prompt==PROMPT_OK || prompt==PROMPT_HALT) {
    clearLine();
    SendNoSep(s);
    SendNoSep("\r\n");
    echoLineBuffer();
  }
}


void vTelnetClient::clearLine() {
  SendNoSep("\r                                                                                \r");
}


void vTelnetClient::onTimer(DWORD systemTime) {
  stopTimer();

  grabActiveHandle();
  if (processCommand((unsigned char*)linebuf)) {
    releaseActiveHandle();

    if (prompt == PROMPT_HALT) prompt = PROMPT_OK;
    echoLineBuffer();
  }
}



//todo:  move this over to the RM-X core
HFUNCTION vTelnetClient::getFunction(char *name) {
  rmxList<char*> names;
  names.appendItem(name);

  char *pos = name;
  while (*pos) {
    if (*pos == '.') {
      *pos = NULL;
      names.appendItem(pos + 1);
    }
    pos++;
  }

  HFUNCTION script = vHFUNCTION::getFirst();
  while (script) {
    const char *script_name = script->getScriptName();
    if (script_name  &&  *script_name) {
      if (VSTRCMP(script_name, name) == 0) {
        if (names.getItemCount() == 1) return script;

        char *childname = names.getItem(1);
        HFUNCTION child = script;
        while (childname) {
          child = getFunctionChild(child, childname);
          if (!child) return NULL;
          childname = names.getNextItem(childname);
        }

        return child;
      }
    }
    script = vHFUNCTION::getNext(script);
  }

  return NULL;
}


HFUNCTION vTelnetClient::getFunctionChild(HFUNCTION function, const char *name) {
  HFUNCTION script = function->child_getFirst();
  while (script) {
    const char *script_name = script->getScriptName();
    if (script_name  &&  *script_name) {
      if (VSTRCMP(script_name, name) == 0) return script;
    }
    script = function->child_getNext(script);
  }
  return NULL;
}


void vTelnetClient::echoName(HFUNCTION function) {
  rmxList<HFUNCTION> items;
  
  while (function) {
    items.appendItem(function);
    function = function->getParent();
  }

  function = items.getLastItem();
  while (function) {
    HFUNCTION prev = items.getPrevItem(function);
    SendNoSep( function->getScriptName() );
    if (prev) SendNoSep(".");
    function = prev;
  }
}
