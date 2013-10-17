#include "IDataTunnel.h"
IDataTunnel::IDataTunnel()
{
	this->agentFd=INVALID_SOCKET;
	this->clientConnected=false;
	this->runFlag=false;
	this->g_hMutex_send_network= INVALID_HANDLE_VALUE;  
	this->g_hMutex_controller_network=INVALID_HANDLE_VALUE;
	
	setLocalPort(DEFAULT_LOCALPORT);
	
}
IDataTunnel::~IDataTunnel()
{
	if(this->agentFd!=INVALID_SOCKET)
	{
		closesocket(this->agentFd);
	}
}
void IDataTunnel::setLocalPort(int port)
{
	this->agentLocalAddr.sin_addr.s_addr=INADDR_ANY;
	this->agentLocalAddr.sin_family=AF_INET;
	this->agentLocalAddr.sin_port=htons(port);
}
void IDataTunnel::setEndpointAddr(string address,int port)
{
	this->endpointAddr.sin_addr.s_addr=inet_addr(address.c_str());
	this->endpointAddr.sin_family=AF_INET;
	this->endpointAddr.sin_port=htons(port);
}
bool IDataTunnel::isClientConnected() const
{
	return this->clientConnected;
}
bool IDataTunnel::sendConnectionResponseData()
{
	char tmpBuf[2];
	tmpBuf[0]=CONNECTIONREQUESTHEADERTYPE;
	tmpBuf[1]=0x1;//Not implement yet
	int ret=SOCKET_ERROR;
	if(WaitForSingleObject(this->g_hMutex_send_network,INFINITE)==WAIT_OBJECT_0)
	{
		ret=sendto(this->agentFd,tmpBuf,2,0,(const sockaddr*)&this->endpointAddr,sizeof(this->endpointAddr));
		ReleaseMutex(g_hMutex_send_network); 
	}
	if(ret==SOCKET_ERROR)
	{
		printf("Error Happen in sending connection data:%d\n",WSAGetLastError());
		return false;
	}
	return true;
}
bool IDataTunnel::sendAudioData(char *data,int size)
{
	char *tmpBuf=(char*)malloc(size+1);
	tmpBuf[0]=AUDIODATAHEADERTYPE;
	memcpy(tmpBuf+1,data,size);
	int ret=SOCKET_ERROR;
	if(WaitForSingleObject(this->g_hMutex_send_network,INFINITE)==WAIT_OBJECT_0)
	{

		ret=sendto(this->agentFd,tmpBuf,size+1,0,(const sockaddr*)&this->endpointAddr,sizeof(this->endpointAddr));
		ReleaseMutex(g_hMutex_send_network); 
	}
	free(tmpBuf);
	if(ret==SOCKET_ERROR)
	{
		printf("Error Happen in sending audio data:%d\n",WSAGetLastError());
		return false;
	}
	return true;
}
void IDataTunnel::sendConnectionCloseRequest()
{
	//Not reliable way!
	printf("Try to cutting down the client\n");
	char tmpBuf[1];
	tmpBuf[0]=CONNECTIONCLOSEHEADERTYPE;
	if(WaitForSingleObject(this->g_hMutex_send_network,10)==WAIT_OBJECT_0)
	{
		sendto(this->agentFd,tmpBuf,2,0,(const sockaddr*)&this->endpointAddr,sizeof(this->endpointAddr));
		ReleaseMutex(g_hMutex_send_network); 
	}
	
}
void IDataTunnel::sendConnectionKeepAlivePacket()
{
	//Not reliable way!
	
	char tmpBuf[1];
	tmpBuf[0]=CONNECTIONKEEPALIVEHEADERTYPE;
	if(WaitForSingleObject(this->g_hMutex_send_network,10)==WAIT_OBJECT_0)
	{
		sendto(this->agentFd,tmpBuf,2,0,(const sockaddr*)&this->endpointAddr,sizeof(this->endpointAddr));
		ReleaseMutex(g_hMutex_send_network); 
	}
	
}
bool IDataTunnel::sendVideoData(char* data,int size,bool isLast)
{
	char *tmpBuf=(char*)malloc(size+1);
	tmpBuf[0]=VIDEODATAHEADERTYPE;
	if(isLast) tmpBuf[0]|=LASTMARKERBIT;
	memcpy(tmpBuf+1,data,size);
	int ret=SOCKET_ERROR;
	if(WaitForSingleObject(this->g_hMutex_send_network,INFINITE)==WAIT_OBJECT_0)
	{
		ret=sendto(this->agentFd,tmpBuf,size+1,0,(const sockaddr*)&this->endpointAddr,sizeof(this->endpointAddr));
		ReleaseMutex(g_hMutex_send_network); 
	}
	free(tmpBuf);
	if(ret==SOCKET_ERROR)
	{
		printf("Error Happen in sending video data:%d\n",WSAGetLastError());
		return false;
	}
	return true;
}
bool IDataTunnel::initDataTunnel()
{
	g_hMutex_send_network = CreateMutex(NULL, FALSE, "Mutex");
	g_hMutex_controller_network = CreateMutex(NULL, FALSE, "Mutex2");
	if(this->g_hMutex_send_network==INVALID_HANDLE_VALUE || this->g_hMutex_controller_network== INVALID_HANDLE_VALUE)
	{
		printf("Failed to init  mutex\n");
		return false;
	}
	this->agentFd=socket(AF_INET,SOCK_DGRAM,0);
	if(agentFd==INVALID_SOCKET )
	{
		printf("Failed to get socket\n");
		return false;
	}
	if (bind(agentFd,(sockaddr*)&this->agentLocalAddr,sizeof(agentLocalAddr)) == SOCKET_ERROR) 
	{   
		printf("Error when bind Agent socket\n");
        return false;
    }
	int buff_size=65536;
	if(setsockopt(agentFd,SOL_SOCKET,SO_RCVBUF,(char*)&buff_size,sizeof(buff_size))==SOCKET_ERROR)
	{
		printf("SET SND Buffer failed\n");
		return false;
	}
	if(setsockopt(agentFd,SOL_SOCKET,SO_SNDBUF,(char*)&buff_size,sizeof(buff_size))==SOCKET_ERROR)
	{
		printf("SET SND Buffer failed\n");
		return false;
	}
	u_long nonblock=1;
	if(ioctlsocket(agentFd,FIONBIO,&nonblock)==SOCKET_ERROR)
	{
		printf("Error when set non-blocking agentFD\n");
        return false;
	}
	
	DWORD dwBytesReturned = 0;
	BOOL bNewBehavior = FALSE;
	if(WSAIoctl(agentFd, SIO_UDP_CONNRESET,&bNewBehavior, sizeof(bNewBehavior),NULL, 0, &dwBytesReturned, NULL, NULL)==SOCKET_ERROR)
	{
		printf("Failed to patch the udp socket\n");
		return false;
	}
	this->runFlag=true;
	return true;
}
bool IDataTunnel::getControllerData(char **data,int *size)
{
	if(WaitForSingleObject(this->g_hMutex_controller_network,INFINITE)==WAIT_OBJECT_0)
	{
		if(this->controllerInformationQueue.size()==0)
		{
			ReleaseMutex(this->g_hMutex_controller_network);
			return false;
		}
		pair<char*,int> t=this->controllerInformationQueue.front();
		*data=t.first;
		*size=t.second;
		this->controllerInformationQueue.pop();
		ReleaseMutex(this->g_hMutex_controller_network);
		return true;
	}
	else
	{
		printf("Get controller data failed due to lock error\n");
		return false;
	}
}
void IDataTunnel::startTunnelLoop()
{
	char buf[10240];
	fd_set fdread;
	timeval tv;
	tv.tv_sec=1;
	tv.tv_usec=0;
	//long lastActionTime=clock();
	long lastSendKeepAliveTime=clock();
	long lastRecvKeepAliveTime=clock();
	SOCKADDR_IN tmpEndPointAddr;
	int fromlen=sizeof(tmpEndPointAddr);
	while(runFlag)
	{
		
		FD_ZERO(&fdread);
		FD_SET(agentFd,&fdread);
		if(clock()-lastRecvKeepAliveTime>MAXKEEPALIVETIME)
		{
			this->stopTunnelLoop();
			printf("Lost Connection\n");
			return;
		}
		if(clientConnected&&clock()-lastSendKeepAliveTime>KEEPALIVEINTERVAL)
		{
					lastSendKeepAliveTime=clock();
					printf("Sending connection keep alive packet\n");
					this->sendConnectionKeepAlivePacket();
		}
		/*if(clock()-lastActionTime>MAXPENDINGTIME)
		{
				this->stopTunnelLoop();
				printf("Max USER Pending Time Arrive\n");
				return;
		}*/
		if(select(0,&fdread,0,0,&tv)!=0)
		{
			
			
			int size=recvfrom(this->agentFd,buf,10240,0,(sockaddr *)&tmpEndPointAddr,&fromlen);
			if(size==SOCKET_ERROR)
			{
				printf("Network Error when recv from udp port:%d\n",WSAGetLastError());
				runFlag=false;
				return;
			}
			
			if(buf[0]&CONNECTIONREQUESTHEADERTYPE)
			{
				
				this->clientConnected=true;
				if(WaitForSingleObject(this->g_hMutex_send_network,INFINITE)==WAIT_OBJECT_0)
				{
					memcpy(&this->endpointAddr,&tmpEndPointAddr,sizeof(tmpEndPointAddr));
					ReleaseMutex(g_hMutex_send_network); 
				}
				else
				{
					printf("Error when setting target address\n");
					return ;
				}
				printf("One way connection OK!Sending HandShake Connection Data\n");
				//Check Data ! Not implement yet!
				if(!this->sendConnectionResponseData())
				{
					printf("Error in sending back connection data\n");
					return ;
				}
				continue;
			}
			
			if(this->clientConnected)
			{
				
				if(memcmp(&this->endpointAddr,&tmpEndPointAddr,sizeof(tmpEndPointAddr))!=0)
				{
					printf("Endpoint Address not meet!\n");
					continue;
				}
				
				if(buf[0]&CONNECTIONCLOSEHEADERTYPE)
				{
					printf("Got a request from client to abort the game\n");
					stopTunnelLoop();
					return ;
				}
				else if(buf[0]& CONNECTIONKEEPALIVEHEADERTYPE)
				{
					printf("Got a keepalive packet\n");
					lastRecvKeepAliveTime=clock();
				}
				else if(buf[0]&CONTROLERDATAHEADERTYPE)
				{
					//printf("Recv a new controller signal\n");
					if(WaitForSingleObject(this->g_hMutex_controller_network,3)==WAIT_OBJECT_0)
					{
						if(this->controllerInformationQueue.size()>MAXWAITQUEUENUM)
						{
							int tsize=this->controllerInformationQueue.size();
							for(int i=0;i<tsize;i++)
							{
								printf("Buffer Full\n");
								free(this->controllerInformationQueue.front().first);
								this->controllerInformationQueue.pop();
							}
						}
						char *tmpBuf=(char*)malloc(size-HEADERLENGTH);
						memcpy(tmpBuf,buf+HEADERLENGTH,size-HEADERLENGTH);
						this->controllerInformationQueue.push(pair<char*,int>(tmpBuf,size-HEADERLENGTH));
						ReleaseMutex(this->g_hMutex_controller_network);
						}
						else
						{
							printf("Due to lock condition! Loss a packet\n");
						}
				}

			}
		}
		
	}
}
void IDataTunnel::stopTunnelLoop()
{
	this->clientConnected=false;
	runFlag=false;
}