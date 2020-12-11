/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "mpr.lib")


#include "Network.h"


static Network *NETWORK = new Network();


Network::Network() : plugBase("Network Tools") {
  table = NULL;
}


void Network::onInit() {
  JNL::open_socketlib();

  table = new funcGroup("Network Tools", "network", this);
//  table->setTooltip("A collection of network related functions");

  new MacAddressScript(table);
  new DnsScript(       table);
  new ReverseDnsScript(table);
  new PingScript(      table);

#ifdef _DEBUG
  new SharesScript(    table);
  new TraceRouteScript(table);
#endif

  new vWol(table);
}


void Network::onQuit() {
  vdelnull(table);
  JNL::close_socketlib();
}


//------------------------------------------------------------------------------


DnsScript::DnsScript(funcGroup *parent) : funcSingle(NULL, "dns", parent) {
//  setTooltip("Returns IP Address of specified Host Address");
  new funcParam("f_address",  "f_address",  "str", this);
}


void DnsScript::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  JNL_AsyncDNS dns;
  unsigned long addr = 0;
  char *address = (char*) source->list_getParamSafe(input, "f_address");

  while (addr == 0) {
    switch (dns.resolve(address, &addr)) {
      case 1:
        break;

      case 0: {
        char str[256];
        JNL::addr_to_ipstr(addr, str, sizeof(str));
        source->list_insertParam(output, "prm1");
        source->list_setParam(output, "prm1", str);
        return;
      }

      case -1:
        source->list_insertParam(output, "prm1");
        return;
    }
    Sleep(1);
  }

  source->list_insertParam(output, "prm1");
}


//------------------------------------------------------------------------------


ReverseDnsScript::ReverseDnsScript(funcGroup *parent) : funcSingle(NULL, "host", parent) {
//  setTooltip("Returns Host Address of specified IP Address");
  new funcParam("f_address",  "f_address",  "str", this);
}


void ReverseDnsScript::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  JNL_AsyncDNS dns;
  char *address = (char*) source->list_getParamSafe(input, "f_address");

  char host[512] = "";
  while (*host == NULL) {
    switch (dns.reverse(JNL::ipstr_to_addr(address), host)) {
      case 1:
        break;

      case 0:
        source->list_insertParam(output, "prm1");
        source->list_setParam(output, "prm1", host);
        return;

      case -1:
        source->list_insertParam(output, "prm1");
        return;
    }
    Sleep(1);
  }

  source->list_insertParam(output, "prm1");
}


//------------------------------------------------------------------------------


SharesScript::SharesScript(funcGroup *parent) : funcSingle(NULL, "shares", parent) {
}


void SharesScript::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  HANDLE hEnum;
  WNetOpenEnum(RESOURCE_CONTEXT, RESOURCETYPE_ANY, NULL, NULL, &hEnum);

  if (hEnum) {
    int count = -1;
    int size = 2048;
    NETRESOURCE *resource = (NETRESOURCE*)malloc(size);
    NETRESOURCE *resourceptr = resource;
    WNetEnumResource(hEnum, (DWORD*)&count, resource, (DWORD*)&size);

    for (int i=0; i<count; i++, resource++) {
      if (resource->dwUsage==RESOURCEUSAGE_CONTAINER && resource->dwType==RESOURCETYPE_ANY) {

        rmxToUtf8 namestr(resource->lpRemoteName);
        const char *name = namestr.getBuffer();
        if (name!=NULL  && *name!=NULL) {
          int len = VSTRLEN(name);
          if (len>2 && name[0]=='\\' && name[1]=='\\') {
            ULONG addr;

            int status = 1;
            JNL_AsyncDNS dns;
            while (status == 1) {
              status = dns.resolve((char*)name+2, &addr);
              Sleep(1);
            }

            if (status == 0) {
              char str[1024];
              char addrstr[32];
              JNL::addr_to_ipstr(addr, addrstr, sizeof(addrstr));
              VPRINTF(str, sizeof(str), "%s %s", addrstr, name);
              CONSOLE_ACTIVE->print(str);

              HANDLE sub_hEnum;
              WNetOpenEnum(RESOURCE_GLOBALNET, RESOURCETYPE_ANY, RESOURCEUSAGE_ALL, resource, &sub_hEnum);

              if (sub_hEnum) {
                int sub_count = -1;
                int sub_size = 2048;
                NETRESOURCE *sub_resource = (NETRESOURCE*)malloc(size);
                NETRESOURCE *sub_resourceptr = sub_resource;
                WNetEnumResource(sub_hEnum, (DWORD*)&sub_count, sub_resource, (DWORD*)&sub_size);
                
                for (int j=0; j<sub_count; j++, sub_resource++) {
                  rmxToUtf8 text(sub_resource->lpRemoteName);
                  CONSOLE_ACTIVE->print(text.getBuffer());
                }
              }

            }
          }

        }
      }
    }

    free(resourceptr);
    WNetCloseEnum(hEnum); 
  }
}


//------------------------------------------------------------------------------


TraceRouteScript::TraceRouteScript(funcGroup *parent) : funcSingle(NULL, "route", parent) {
  new funcParam("f_address",  "f_address",  "str", this);
}


void TraceRouteScript::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  char *address = (char*) source->list_getParamSafe(input, "f_address");

  HANDLE hIP = IcmpCreateFile();
  int badhop = 0;

  struct hostent* phe;
  if ((phe = gethostbyname(address)) == 0) {
    return;
  }

  char acPingBuffer[64];
  memset(acPingBuffer, '\xAA', sizeof(acPingBuffer));
  PIP_ECHO_REPLY pIpe =(PIP_ECHO_REPLY)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer));
  if (pIpe == 0) {
    return;
  }

  pIpe->Data = acPingBuffer;
  pIpe->DataSize = sizeof(acPingBuffer);

  for (int i=1; i<=30; i++) {
    IP_OPTION_INFORMATION opt_info;
    ZeroMemory(&opt_info, sizeof(opt_info));
    opt_info.Ttl = i;

    // Send the ping packet
    DWORD dwStatus = IcmpSendEcho(hIP, *((DWORD*)phe->h_addr_list[0]), acPingBuffer, sizeof(acPingBuffer), &opt_info, pIpe, sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer), 5000);
    if (dwStatus != 0) {
      badhop = 0;

      char str_addr[16];
      char str[512]="";
      JNL::addr_to_ipstr(pIpe->Address, str_addr, sizeof(str_addr));
      VPRINTF(str, sizeof(str), "%dms\t%s\t", pIpe->RoundTripTime, str_addr);
      int len = VSTRLEN(str);

      int status = 1;
      JNL_AsyncDNS dns;
      while (status == 1) {
        status = dns.reverse(pIpe->Address, str+len);
        Sleep(1);
      }

      CONSOLE_ACTIVE->print(str);

      if (pIpe->Status != IP_HOP_LIMIT_EXCEEDED) break;
    } else {
      badhop++;
      if (badhop == 4) {
        CONSOLE_ACTIVE->print("4 bad hops, asuming blocked request");
        break;
      }
      CONSOLE_ACTIVE->print("bad hop");
    }
  }

  GlobalFree(pIpe);
}


//------------------------------------------------------------------------------


PingScript::PingScript(funcGroup *parent) : funcSingle(NULL, "ping", parent) {
//  setTooltip("Returns round trip time to specified Host or IP Address");
  new funcParam("f_address",  "f_address",  "str", this);
}


void PingScript::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  char *address = (char*) source->list_getParamSafe(input, "f_address");

  HANDLE hIP = IcmpCreateFile();
  int time = -1;

  struct hostent* phe;
  if ((phe = gethostbyname(address)) == 0) {
    source->list_insertParam(output, "prm1");
    source->list_setParamInt(output, "prm1", time);
    return;
  }


  char acPingBuffer[64];
  memset(acPingBuffer, '\xAA', sizeof(acPingBuffer));
  PIP_ECHO_REPLY pIpe =(PIP_ECHO_REPLY)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer));
  if (pIpe == 0) {
    source->list_insertParam(output, "prm1");
    source->list_setParamInt(output, "prm1", time);
    return;
  }

  pIpe->Data = acPingBuffer;
  pIpe->DataSize = sizeof(acPingBuffer);

  // Send the ping packet
  DWORD dwStatus = IcmpSendEcho(hIP, *((DWORD*)phe->h_addr_list[0]), acPingBuffer, sizeof(acPingBuffer), NULL, pIpe, sizeof(IP_ECHO_REPLY) + sizeof(acPingBuffer), 5000);
  if (dwStatus != 0) {
    time = (int)pIpe->RoundTripTime;
  }

  GlobalFree(pIpe);

  source->list_insertParam(output, "prm1");
  source->list_setParamInt(output, "prm1", time);
}


//------------------------------------------------------------------------------


MacAddressScript::MacAddressScript(funcGroup *parent) : funcSingle(NULL, "mac", parent) {
//  setTooltip("Returns MAC Address of specified IP Address");
  new funcParam("f_address",  "f_address",  "str", this);
}


void MacAddressScript::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  char *address = (char*) source->list_getParamSafe(input, "f_address");

  IPAddr  ipAddr = inet_addr(address);;
  ULONG   pulMac[2] = { 0xFFFFFFFF, 0xFFFFFFFF };
  ULONG   ulLen = 6;
  PBYTE pbHexMac = (PBYTE) pulMac;
  HRESULT hr = SendARP (ipAddr, 0, pulMac, &ulLen);

  char szMac[20] = "";
  VPRINTF(szMac, sizeof(szMac), "%02X:%02X:%02X:%02X:%02X:%02X",
    pbHexMac[0], pbHexMac[1], pbHexMac[2], pbHexMac[3], pbHexMac[4], pbHexMac[5]);

  source->list_insertParam(output, "prm1");
  source->list_setParam(output, "prm1", szMac);
}


//--------------------------------------------------------------------------------------------


vWol::vWol(funcGroup *parent) : funcSingle("WOL (Wake On Lan)", "wol", parent) {
  new funcParam("MAC Address", "f_mac", "string", this);
}

vWol::~vWol() {
}

void vWol::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  PROFILE_STR("WOL");

  const char *mac = source->list_getParam(input, "f_mac");
  if (!mac  &&  !*mac) return;

  rmxThread *t = new vWolThread(mac);
  t->Begin(THREAD_PRIORITY_NORMAL);
}


//--------------------------------------------------------------------------------------------


vWolThread::vWolThread(const char *macaddress) {
  PROFILE_STR("WOL CLASS");

  int len = VSTRLEN(macaddress);
  char *buffer = (char*)malloc(len+1);
  VSTRCPY(buffer, macaddress, (len+1));

  char *start = buffer;
  char *pos = buffer;

  while (*pos) {
    if (*pos != ':') {
      *start = *pos;
      start++;
    }
    pos++;
  }
  
  int data[6] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  VSCANF(buffer, "%02X%02X%02X%02X%02X%02X", &data[0], &data[1], &data[2], &data[3], &data[4], &data[5]);

  for (int i=0; i<6; i++) mac[i] = (BYTE)data[i];

  free(buffer);
}


int vWolThread::threadProc() {
#ifdef RMX_DEBUG
  char str[64]="";
  VPRINTF(str, sizeof(str), "WOL THREAD: %02X:%02X:%02X:%02X:%02X:%02X",
    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  DEBUGMSG("WOL", str);
#endif

  LPHOSTENT lpHostEntry = gethostbyname("192.168.1.255");  //TODO: make this dynamic
  if (!lpHostEntry) return rmxThread::threadProc();

  SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (s == INVALID_SOCKET) return rmxThread::threadProc();

  SOCKADDR_IN sockAddr;
  sockAddr.sin_family = AF_INET;
  sockAddr.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
  sockAddr.sin_port = htons(9);

  int ret = connect(s, (LPSOCKADDR)&sockAddr, sizeof(struct sockaddr));
  if (ret == INVALID_SOCKET) return rmxThread::threadProc();

  BYTE buffer[6 + (6*16)] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
  
  BYTE *pos = buffer + 6;
  for (int i=0; i<16; i++) {
    for (int j=0; j<6; j++) {
      *pos = mac[j];
      pos++;
    }
  }

  int sent = send(s, (char*)buffer, sizeof(buffer), 0);

  closesocket(s);
  return rmxThread::threadProc();
}
