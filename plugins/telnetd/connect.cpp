/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#pragma comment(lib, "WS2_32.lib")

#include "connect.h"


static char brk[3] = { 0x0D, 0x0A, 0x00 };


vConnect::vConnect(vServer *server) {
  port = 0;
  client = NULL;
  listener = NULL;
  localServer = server;
  deleteOnDisconnect = FALSE;
  closeOnSend = FALSE;
  curBuf = 0;
  maxConnections = 1;
  flush = FALSE;

  ZeroMemory(buffer,     sizeof(buffer));
  ZeroMemory(hostname,   sizeof(hostname));
  ZeroMemory(hostaddr,   sizeof(hostaddr));
  ZeroMemory(remotename, sizeof(remotename));
  ZeroMemory(remoteaddr, sizeof(remoteaddr));

  if (gethostname(hostname, sizeof(hostname)-1) != SOCKET_ERROR) {
    LPHOSTENT hostEntry = gethostbyname(hostname);
    if (hostEntry) {
      in_addr *addr = (LPIN_ADDR) hostEntry->h_addr;
      VPRINTF(hostaddr, sizeof(hostaddr), "%d.%d.%d.%d", addr->S_un.S_un_b.s_b1, addr->S_un.S_un_b.s_b2, addr->S_un.S_un_b.s_b3, addr->S_un.S_un_b.s_b4);
    }
  }
}


vConnect::~vConnect() {
  if (localServer) localServer->removeConnection(this);
  closeSocket();
}


void vConnect::onPostDisconnected() {
  if (deleteOnDisconnect) delete this;
}


void vConnect::closeSocket(BOOL flushSocket) {
  SOCKET s = client;
  flush = flushSocket;

  if (listener) {
    End();
    SOCKET li = listener;
    listener = NULL;
//    shutdown(li, SD_SEND);
//    Sleep(10);
    closesocket(li);
  }

  if (s) {
    End();
    if (flush) {
      closeOnSend = FALSE;
      while (onSend() == 0);
    }
    onDisconnected();
    client = NULL;
    shutdown(s, SD_SEND);
    Sleep(10);
    closesocket(s);
  }

  emptyItems();
//  End();
  if (s) onPostDisconnected();
  s = NULL;
}


void vConnect::acceptSocket(SOCKET listenSocket) {
  client = accept(listenSocket, NULL, NULL);
  Begin(THREAD_PRIORITY_NORMAL);
  onConnected();
}


void vConnect::acceptSocket() {
  client = accept(listener, NULL, NULL);
  Begin(THREAD_PRIORITY_NORMAL);
  onConnected();
}


void vConnect::acceptSocket(vServer *server) {
  client = accept(server->getListenSocket(), NULL, NULL);
  if (client == NULL) return;
  localServer = server;
//  setNotifyParent(server);
  server->addConnection(this);
  Begin(THREAD_PRIORITY_NORMAL);
  onConnected();
}

void vConnect::acceptServerSocket() {
  acceptSocket(localServer);
}



void vConnect::listenSocket(int listenPort) {
  listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (listener == INVALID_SOCKET) {
    MessageBox(0, L"invalid socket", 0, 0);
    return;
  }

  if (listenPort != 0) port = listenPort;

  ZeroMemory(&sockAddr, sizeof(sockAddr));
  sockAddr.sin_family = AF_INET;
  sockAddr.sin_addr.s_addr = INADDR_ANY;
  sockAddr.sin_port = htons(port);

  int nRet = bind(listener, (LPSOCKADDR)&sockAddr, sizeof(sockAddr));
  if (nRet == SOCKET_ERROR) {
    MessageBox(0, L"unable to bind socket", 0, 0);
    return;
  }


  int connections = maxConnections;
  if (connections = -1) connections = SOMAXCONN;
  nRet = listen(listener, connections);
  if (nRet == SOCKET_ERROR) {
    MessageBox(0, L"unable to open listener", 0, 0);
    return;
  }


  Begin(THREAD_PRIORITY_NORMAL);
}


BOOL vConnect::connectSocket(const char *host, int port) {
  int nRet = 0;

  LPHOSTENT lpHostEntry = gethostbyname(host);
  if (lpHostEntry == NULL)
  { MessageBox(0, L"Unknown Host", L"", 0); return FALSE; }

  client = socket(AF_INET,  SOCK_STREAM, IPPROTO_TCP);
  if (client == INVALID_SOCKET)
  { MessageBox(0, L"Unable to create socket", L"", 0); return FALSE; }

  sockAddr.sin_family = AF_INET;
  sockAddr.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
  sockAddr.sin_port = htons(port);

  nRet = connect(client, (LPSOCKADDR)&sockAddr, sizeof(struct sockaddr));
  if (nRet == SOCKET_ERROR)
  { MessageBox(0, L"Unable to connect", L"", 0); return FALSE; }

  Begin(THREAD_PRIORITY_NORMAL);
  onConnected();

  return TRUE;
}


void vConnect::process(char *txt) {
  int len = VSTRLEN(txt);
  if (len < 4) return;

  char *parse = txt;
  while (*parse) {
    if (*parse == ':') { *parse = NULL; parse++; break; }
    else parse++;
  }

  char *parse2 = parse;
  while (*parse2) {
    if (*parse2 == ':') { *parse2 = NULL; parse2++; break; }
    else parse2++;
  }

  if (VSTRCMPI(parse, "REPLY") == 0) {
    UINT id = 0;
    VSCANF(txt, "%08X", &id);
    vFunctionData *data = getFunctionData(id);
    if (data) {
      int str_len = VSTRLEN(parse2) + 1;
      char *str = (char*)malloc(str_len);
      memcpy(str, parse2, str_len);
      data->data = str;
      callstack.removeItem(data);
      data->wait = FALSE;
      //gotta change this to a delayed delete of teh string
    }
  }
}


int vConnect::onSend() {
  if (hasItems()) {
    LPvSendData data = getFirstItem();
    int total =  send(client, data->data, data->len, 0);
    if (total > 0) {
      Sleep(0);
      removeItem(data);
      return 0;
    }
    else if (total == SOCKET_ERROR) {
      int err = WSAGetLastError();
      switch (err) {
      case WSAENETRESET:
      case WSAESHUTDOWN:
      case WSAECONNRESET:
      case WSAECONNABORTED:
        closeSocket();
        break;
      }
    }
  } else {
    if (closeOnSend) closeSocket();
  }
  return 1;
}


int vConnect::onRead() {
  int charsRead = recv(client, buffer+curBuf, sizeof(buffer)-curBuf-3, 0);
  if (charsRead > 0) {
    int buflen = charsRead+curBuf;
    buffer[buflen] = NULL;

    VINT start=0;
    char *bufloc = buffer;

    while (bufloc < buffer+buflen) {
      if ( (*bufloc == 0x0D) | (*bufloc == 0x0A) ) {
        *bufloc = NULL;
        char *bf = buffer+start;
        while (*bf==0x0D || *bf==0x0A) bf++;
        process(bf);
//        while (*bufloc==0x0D || *bufloc==0x0A) bufloc++;
        if (*bufloc == 0x0D) bufloc++;
        if (*bufloc == 0x0A) bufloc++;
        if (*bufloc == NULL) bufloc++;
        start = bufloc - buffer /*+ 1*/;
        if (start > buflen) start = buflen;
      }
      bufloc++;
    }
    /*
    for (int i=start; i<buflen; i++) {
      if ( (buffer[i] == 0x0D) | (buffer[i] == 0x0A) ) {
        buffer[i] = NULL;
        process(buffer+start);
        i++;
        if (buffer[i] == 0x0D) i++;
        if (buffer[i] == 0x0A) i++;
        start = i;
      }
    }
    */

    if (start > 0) {
      if (start == buflen) {
        curBuf = 0;
        *buffer = NULL;
      } else {
        char *bufbgn = buffer;
        char *bufend = buffer+start;
        while (bufend < buffer+buflen) {
          *bufbgn = *bufend;
          bufbgn++;
          bufend++;
        }
      }
    }

//    if (i > buflen) i = buflen;
/*
    if (start > 0) {
      if (VSTRLEN(buffer+start) > 0) {
        int i=0;
        char *p = buffer+start;
        while (*p != NULL) buffer[i++] = *p++;
        curBuf = VSTRLEN(buffer);
      } else {
        curBuf = 0;
        ZeroMemory(buffer, sizeof(buffer));
      }
    }
*/
  }
  return 0;
}


void vConnect::onRequestConnection(SOCKET sock) {
  acceptSocket(sock);
}


void vConnect::onDelete(LPvSendData data) {
  free((void*)data->data);
  delete data;
}


void vConnect::onConnected() {
//  notifyParent(vConnectNotify::ONCONNECT);
  
  int size = sizeof(SOCKADDR_IN);
  getpeername(client, (sockaddr*)&sockAddr, &size);
  getnameinfo((sockaddr*)&sockAddr, size, remotename, sizeof(remotename), NULL, 0, 0);
  getnameinfo((sockaddr*)&sockAddr, size, remoteaddr, sizeof(remoteaddr), NULL, 0, NI_NUMERICHOST);
}


void vConnect::onDisconnected() {
  buffer[curBuf] = NULL;
  if (*buffer) process(buffer);
}


void vConnect::Send(const char *str) {
  if (str == NULL) return;
  int len = VSTRLEN(str)+3;
  if (len < 4) return;
  char *a = (char*)malloc(len);
  
  if (a) {
    memcpy(a, str, len-2);
    LPvSendData data = new vSendData();
    data->data = a;
    data->len = len-1;
    VSTRCAT((char*)data->data, brk, data->len);
    appendItem(data);
  }
}


const char *vConnect::SendFunction(const char *str) {
  if (str == NULL) return NULL;
  int len = VSTRLEN(str)+12;
  if (len < 4) return NULL;
  char *a = (char*)malloc(len);
  
  if (a) {
    VPRINTF(a, len, "%08X:%s%s", a, str, brk);
    LPvSendData data = new vSendData();
    data->data = a;
    data->len = len-1;
    vFunctionData fdata;
    fdata.data = NULL;
    fdata.id = (UINT)a;
    fdata.wait = TRUE;
    callstack.appendItem(&fdata);
    appendItem(data);
    while (fdata.wait) { Sleep(1); }
    return (char*) fdata.data;
  }
  return NULL;
}


UINT vConnect::SendFunctionInt(const char *str) {
  int id=0;
  const char *buffer = SendFunction(str);
  if (!buffer) return 0;
  VSCANF(buffer, "%d", &id);
  return id;
}


void vConnect::SendNoSep(const char *str) {
  if (str == NULL) return;
  int len = VSTRLEN(str);
  char *a = (char*)malloc(len);
  
  if (a) {
    memcpy(a, str, len);
    LPvSendData data = new vSendData();
    data->data = a;
    data->len = len;
    appendItem(data);
  }
}


void vConnect::SendRaw(const BYTE *bytes, int len) {
  if (bytes == NULL) return;
  if (len < 1) return;

  char *a = (char*)malloc(len);
  if (a) {
    memcpy(a, bytes, len);
    LPvSendData data = new vSendData();
    data->data = a;
    data->len = len;
    appendItem(data);
  }
}


void vConnect::SendFast(const char *str) {
  if (str == NULL) return;
  int len = VSTRLEN(str)+1;
  if (len < 2) return;

  LPvSendData data = new vSendData();
  data->data = str;
  data->len = len+2;
  VSTRCAT((char*)data->data, brk, data->len);
  appendItem(data);
}


vFunctionData *vConnect::getFunctionData(UINT function) {
  if (!function) return NULL;
  vFunctionData *data = callstack.getFirstItem();
  while (data) {
    if (data->id == function) return data;
    data = callstack.getNextItem(data);
  }
  return NULL;
}


DWORD vConnect::ThreadProc() {
  static timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 1000;

  while (isRunning()) {
    if (client) {
      static fd_set socketSend;
      socketSend.fd_count = 1;
      socketSend.fd_array[0] = client;

      static fd_set socketRecv;
      socketRecv.fd_count = 1;
      socketRecv.fd_array[0] = client;

      static fd_set socketErr;
      socketErr.fd_count = 1;
      socketErr.fd_array[0] = client;

      int ret = select(NULL, &socketRecv, &socketSend, &socketErr, &timeout);
      if (ret == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err == WSAENOTSOCK) closeSocket();
        else Sleep(1);
      }
      else if (ret > 0) {
        int wait = 0;
        if (socketRecv.fd_count > 0) { int w = onRead(); wait = max(wait, w); }
        if (client == NULL) return FALSE;
        if (socketSend.fd_count > 0) { int w = onSend(); wait = max(wait, w); }
        Sleep(wait);
      } else Sleep(10);
    }

    else if (listener) {
      static fd_set socketListen;
      socketListen.fd_count = 1;
      socketListen.fd_array[0] = listener;

      if (select(NULL, &socketListen, NULL, NULL, &timeout) > 0) {
        onRequestConnection(listener);
      }
      Sleep(10);
    }

    else Sleep(10);

    if ( (getSelfDelete()) & (getDeleteNow()) ) break;
  }

  return FALSE;
}


//-------------------------------------------------------------------------------

vServer::vServer() {
}

vServer::~vServer() {
  closeAllConnections();
}


void vServer::onRequestConnection(SOCKET sock) {
  vConnect *connect = newConnection();
  if (connect == NULL) return;
  connect->acceptSocket(this);
}


void vServer::SendAll(const char *str) {
  vConnect *connect = connections.getFirstItem();
  while (connect) {
    connect->Send(str);
    connect = connections.getNextItem(connect);
  }
}


void vServer::SendRawAll(const BYTE *bytes, int len) {
  vConnect *connect = connections.getFirstItem();
  while (connect) {
    connect->SendRaw(bytes, len);
    connect = connections.getNextItem(connect);
  }
}


void vServer::closeAllConnections(BOOL flushSockets) {
  vConnect *connect = connections.getFirstItem();
  while (connect) {
    connect->closeSocket(flushSockets);
    connections.removeItem(connect);
    connect = connections.getFirstItem();
  }
}

/*
int vServer::onNotify(vNotify *sender, vNotify *child, int msg, int param1, int param2) {
  if (child) switch (msg) {
    case vConnectNotify::ONCONNECT: {
      vConnect *connect = static_cast<vConnect*>(child);
      if (connect) onClientConnected(connect);
    } break;

    case vConnectNotify::ONDISCONNECT: {
      vConnect *connect = static_cast<vConnect*>(child);
      if (connect) onClientDisconnected(connect);
    } break;
  }
  return vNotify::onNotify(sender, child, msg, param1, param2);
}
*/
