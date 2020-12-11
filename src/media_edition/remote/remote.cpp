/******************************  RMX SDK  ******************************\
*  Copyright (c) 2010 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#pragma comment(lib, "WS2_32.lib")


#include "../../../sdk/rmxBase.h"
#include "../../core/callbacks/cbFunction.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include "remote.h"


rmxUdpRemote::rmxUdpRemote() : vThread(false), vTimer(1, 0) {
	Begin();
}


rmxUdpRemote::~rmxUdpRemote() {
	End();
	stopTimer();
}



DWORD rmxUdpRemote::ThreadProc() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
		MessageBoxA(0, "Error starting Winsock", "RMX Automation", 0);
		return false;
	}


	SOCKET RecvSocket;
	sockaddr_in RecvAddr;
	int Port = 31055;
	char RecvBuf[201] = "";
	int  BufLen = sizeof(RecvBuf)-1;
	int  readLen = -1;
	sockaddr_in SenderAddr;
	int SenderAddrSize = sizeof(SenderAddr);

	//-----------------------------------------------
	// Create a receiver socket to receive datagrams
	RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//-----------------------------------------------
	// Bind the socket to any address and the specified port.
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(RecvSocket, (SOCKADDR *) &RecvAddr, sizeof(RecvAddr));


	while (isRunning()) {
		readLen = recvfrom(RecvSocket, RecvBuf, BufLen, 0, (SOCKADDR*)&SenderAddr, &SenderAddrSize);
		if (readLen >= 1) {
			RecvBuf[sizeof(RecvBuf)-1] = 0x00;
			if (readLen < sizeof(RecvBuf)) RecvBuf[readLen] = 0x00;

			beginThreadSafe();
			list.appendItem(new rmxString(RecvBuf));
			startTimer();
			endThreadSafe();

			RecvBuf[0] = 0x00;
		}
	}

	return false;
}


void rmxUdpRemote::onTimer(DWORD systemTime) {
	beginThreadSafe();

	rmxString *item = list.getFirstItem();
	while (item) {
		cbCoreFunction *function = cbCoreFunction::getFunctionByScriptName(item);

		list.removeItem(item);
		item = list.getFirstItem();

		if (function) function->activateFunction(plg->getMasterHandle(), 0, 0);
	}

	stopTimer();
	endThreadSafe();
}
