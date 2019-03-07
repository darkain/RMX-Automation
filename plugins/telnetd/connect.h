/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __NETSLAVECONNECTION_H_
#define __NETSLAVECONNECTION_H_


#include "../../sdk/rmxBase.h"
#include "../../sdk/rmxList.h"
#include "../../sdk/Win32/Thread.h"


#include <winsock2.h>
#include <Ws2tcpip.h>


class vServer;
class vConnect;


typedef struct vtagSENDDATA {
  vtagSENDDATA() { ZeroMemory(this, sizeof(vtagSENDDATA)); }
  const char *data;
  int len;
} vSendData, *LPvSendData;

typedef struct vtagFUNCTIONDATA {
  volatile const char *data;
  UINT id;
  volatile BOOL wait;
} vFunctionData, *LPvFunctionData;


class vConnect : public rmxThread, protected rmxList<LPvSendData> {
  public:
    vConnect(vServer *server=NULL);
    virtual ~vConnect();

    void setSocket(SOCKET s) { client = s; }
    SOCKET getSocket() { return client; }
    SOCKET getListenSocket() { return listener; }

    void setPort(int p) { port = p; }
    int getPort() { return port; }

    void acceptSocket(SOCKET listenSocket);
    void acceptSocket();
    void acceptSocket(vServer *server);
    void acceptServerSocket();

    void listenSocket(int listenPort=0);

    BOOL connectSocket(const char *host, int port);
    void closeSocket(BOOL flushSocket=FALSE);
    
    void Send(const char *str);
    void SendNoSep(const char *str);
    void SendFast(const char *str);
    void SendRaw(const BYTE *bytes, int len);
    const char *SendFunction(const char *str);
    UINT SendFunctionInt(const char *str);


    //both of these should return ms delay before next check
    //or else the thread may kill CPU
    virtual int onRead();
    virtual int onSend();

    virtual void onRequestConnection(SOCKET sock);

    virtual void process(char *txt);

    const char *getLocalName() { return hostname; }
    const char *getLocalAddress() { return hostaddr; }
    
    const char *getRemoteName() { return remotename; }
    const char *getRemoteAddress() { return remoteaddr; }

    void setLocalServer(vServer *serv) { localServer = serv; }
    vServer *getLocalServer() { return localServer; }

    void setMaxConnections(int max) { maxConnections = max; }
    int getMaxConnections() { return maxConnections; }

    void setDeleteOnDisconnect(BOOL del) { deleteOnDisconnect = !!del; }
    BOOL getDeleteOnDisconnect() { return deleteOnDisconnect; }

    void setCloseOnSend(BOOL c) { closeOnSend = c; }
    BOOL getCloseOnSend() { return closeOnSend; }

    vFunctionData *getFunctionData(UINT function);

    //from vThread
    virtual DWORD ThreadProc();

    //from vListMT
    virtual void onDelete(LPvSendData data);

  protected:
    virtual void onConnected();
    virtual void onDisconnected();

    void onPostDisconnected();

  protected:
    char hostname[256];
    char hostaddr[32];

    char remotename[256];
    char remoteaddr[32];

    SOCKET client;
    SOCKET listener;

    SOCKADDR_IN sockAddr;
    int port;
    int maxConnections;

    char buffer[4096];
    int curBuf;

    BOOL flush;

  protected:
    vServer *localServer;
    BOOL deleteOnDisconnect;
    BOOL closeOnSend;
    rmxList<vFunctionData*> callstack;
};


class vServer : public vConnect {
  public:
    vServer();
    virtual ~vServer();

    void addConnection(vConnect *c) { connections.appendItem(c); }
    void removeConnection(vConnect *c) { connections.removeItem(c); }
    vConnect *getFirstConnection() { return connections.getFirstItem(); }
    vConnect *getNextConnection(vConnect *c) { return connections.getNextItem(c); }
    vConnect *getLastConnection() { return connections.getLastItem(); }

    virtual vConnect *newConnection() { return new vConnect(this); }

    void closeAllConnections(BOOL flushSockets=FALSE);
  
    virtual void onRequestConnection(SOCKET sock);

//    virtual int onNotify(vNotify *sender, vNotify *child, int msg, int param1, int param2);
    virtual void onClientConnected(vConnect *c) {}
    virtual void onClientDisconnected(vConnect *c) {}

    void SendAll(const char *str);
    void SendRawAll(const BYTE *bytes, int len);

  protected:
    rmxList<vConnect*> connections;
};


#endif __NETSLAVECONNECTION_H_