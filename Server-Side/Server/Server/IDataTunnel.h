#pragma once
#include "stdafx.h"
#include <WinSock2.h>
#include <windows.h>
#include <string>
#include <queue>
#include <time.h>
using namespace std;
#define DEFAULT_LOCALPORT 20000
#define NETWORKMTU 1450 
#define MAXWAITQUEUENUM 20
#define MAXPENDINGTIME 10*60*1000
#define MAXKEEPALIVETIME 60*1000
#define KEEPALIVEINTERVAL 500
//===========Explation
#define VIDEODATAHEADERTYPE 0x1
#define AUDIODATAHEADERTYPE 0x2
#define CONTROLERDATAHEADERTYPE 0x4
#define CONNECTIONREQUESTHEADERTYPE 0x8
#define CONNECTIONCLOSEHEADERTYPE 0x10
#define CONNECTIONKEEPALIVEHEADERTYPE 0x20
#define LASTMARKERBIT 0x40
#define HEADERLENGTH 1
//0TKQ LTTT From left to right, second bit show whether the last frame, From right to left, show the type of packet!
//like Video Audio Controller, L means connection header type, Q means close connection, K mean keep alive
//=================
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR,12)
class IDataTunnel
{
public:
	IDataTunnel();
	~IDataTunnel();
	bool initDataTunnel();
	void startTunnelLoop();
	void stopTunnelLoop();
	void setLocalPort(int port);
	void setEndpointAddr(string addr,int port);
	bool sendVideoData(char*data,int size,bool last);
	bool sendAudioData(char*data,int size);
	void sendConnectionCloseRequest();
	bool isClientConnected() const;
	bool getControllerData(char **data,int *size);
	void sendConnectionKeepAlivePacket();
	
private:
	SOCKET agentFd;
	SOCKADDR_IN agentLocalAddr;
	SOCKADDR_IN endpointAddr;
	bool clientConnected;
	bool runFlag;
	HANDLE g_hMutex_send_network;
	HANDLE g_hMutex_controller_network;
	queue< pair<char*,int> > controllerInformationQueue;
	bool sendConnectionResponseData();
	
};