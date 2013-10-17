#pragma once
#include "stdafx.h"
#include <winsock2.h>
#include <time.h>
#include <queue>
using namespace std;
#define DEFAULT_LOCALPORT 20240
#define DEFAULT_REMOTEPORT 20000
#define DEFAULT_REMOTEADDRESS "127.0.0.1"
#define MAXWAITQUEUENUM 20
#define MAXVIDEONALPACKETBUFSIZE 1024*100
#define MAXVIDEOPACKETBUFSIZE 1024*1000
#define MAXPENDINGTIME 10*1000
#define MAXKEEPALIVETIME 10*1000
#define KEEPALIVEINTERVAL 500
//===========Explation
#define VIDEODATAHEADERTYPE 0x1
#define AUDIODATAHEADERTYPE 0x2
#define CONTROLERDATAHEADERTYPE 0x4
#define CONNECTIONREQUESTHEADERTYPE 0x8
#define CONNECTIONCLOSEHEADERTYPE 0x10
#define CONNECTIONKEEPALIVEHEADERTYPE 0x20
#define HEADERLENGTH 1
#define LASTMARKERBIT 0x40
//0TKQ LTTT From left to right, second bit show whether the last frame, From right to left, show the type of packet!
//like Video Audio Controller, L means connection header type,  Q means close connection, K means keep alive;
//=================
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR,12)
class DataTunnel
{
public:
	DataTunnel();
	~DataTunnel();
	bool initDataTunnel();
	void startTunnelLoop();
	void stopTunnelLoop();
	void setLocalPort(int port);
	void setEndpointAddr(const char* addr,int port);
	void setEndpointPort(int port);
	void setEndpointIPAddr(const char *addr);
	void sendConnectionCloseRequest();
	bool isServerConnected() const;
	bool sendControllerData(char *data,int size);
	bool getVideoData(char **data,int *size);
	bool getAudioData(char **data,int *size);
	bool sendConnectionRequestData();
	void sendConnectionKeepAlivePacket();
private:
	SOCKET agentFd;
	SOCKADDR_IN agentLocalAddr;
	SOCKADDR_IN endpointAddr;
	bool serverConnected;
	bool runFlag;
	HANDLE g_hMutex_audio_network;
	HANDLE g_hMutex_video_network;
	HANDLE g_hMutex_send_network;
	queue< pair<char*,int> > audioQueue;
	queue< pair<char*,int> > videoQueue;
	BYTE nalPacketBuf[MAXVIDEONALPACKETBUFSIZE];
	int nalPacketCursor;
	BYTE videoPacketBuf[MAXVIDEOPACKETBUFSIZE];
	
	int videoPacketCursor;
	
	void handleNALPacket(char*data,int size);
	void writeNALPacket(char*data,int size);
	
};