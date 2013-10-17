#include "DataTunnel.h"
DataTunnel::DataTunnel()
{
	this->agentFd=INVALID_SOCKET;
	this->serverConnected=false;
	this->runFlag=false;
	this->g_hMutex_send_network= INVALID_HANDLE_VALUE;  
	this->g_hMutex_audio_network=INVALID_HANDLE_VALUE;
	this->g_hMutex_video_network=INVALID_HANDLE_VALUE;
	this->nalPacketCursor=0;
	this->videoPacketCursor=0;
	setLocalPort(DEFAULT_LOCALPORT);
	this->setEndpointAddr(DEFAULT_REMOTEADDRESS,DEFAULT_REMOTEPORT);
}
DataTunnel::~DataTunnel()
{
	if(this->agentFd!=INVALID_SOCKET)
	{
		closesocket(this->agentFd);
	}
}
void DataTunnel::setLocalPort(int port)
{
	this->agentLocalAddr.sin_addr.s_addr=INADDR_ANY;
	this->agentLocalAddr.sin_family=AF_INET;
	this->agentLocalAddr.sin_port=htons(port);
}
bool DataTunnel::isServerConnected() const
{
	return this->serverConnected;
}
void DataTunnel::setEndpointAddr(const char* address,int port)
{
	this->endpointAddr.sin_addr.s_addr=inet_addr(address);
	this->endpointAddr.sin_family=AF_INET;
	this->endpointAddr.sin_port=htons(port);
}
void DataTunnel::setEndpointIPAddr(const char *addr)
{
	this->endpointAddr.sin_addr.s_addr=inet_addr(addr);
	this->endpointAddr.sin_family=AF_INET;
}
void DataTunnel::setEndpointPort(int port)
{
	this->endpointAddr.sin_port=htons(port);
	this->endpointAddr.sin_family=AF_INET;
}
bool DataTunnel::sendControllerData(char*data,int size)
{
	char *tmpBuf=(char*)malloc(size+1);
	tmpBuf[0]=CONTROLERDATAHEADERTYPE;
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
		printf("Error Happen in sending controller data:%d\n",WSAGetLastError());
		return false;
	}
	return true;
}
void DataTunnel::sendConnectionCloseRequest()
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
bool DataTunnel::initDataTunnel()
{
	g_hMutex_send_network = CreateMutex(NULL, FALSE, "Mutex");
	g_hMutex_audio_network = CreateMutex(NULL, FALSE, "Mutex2");
	g_hMutex_video_network = CreateMutex(NULL, FALSE, "Mutex3");
	if(this->g_hMutex_send_network==INVALID_HANDLE_VALUE || this->g_hMutex_audio_network== INVALID_HANDLE_VALUE || this->g_hMutex_video_network== INVALID_HANDLE_VALUE)
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
bool DataTunnel::sendConnectionRequestData()
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
bool DataTunnel::getAudioData(char **data,int *size)
{
	if(WaitForSingleObject(this->g_hMutex_audio_network,INFINITE)==WAIT_OBJECT_0)
	{
		if(this->audioQueue.size()==0)
		{
			ReleaseMutex(this->g_hMutex_audio_network);
			return false;
		}
		pair<char*,int> t=this->audioQueue.front();
		*data=t.first;
		*size=t.second;
		this->audioQueue.pop();
		ReleaseMutex(this->g_hMutex_audio_network);
		return true;
	}
	else
	{
		printf("Get controller data failed due to lock error\n");
		return false;
	}
}
void DataTunnel::startTunnelLoop()
{
	char buf[10240];
	fd_set fdread;
	timeval tv;
	tv.tv_sec=1;
	tv.tv_usec=0;
	long lastSendKeepAliveTime=clock();
	long lastRecvKeepAliveTime=clock();
	//long lastActionTime=clock();
	while(runFlag)
	{
		if(clock()-lastRecvKeepAliveTime>MAXKEEPALIVETIME)
		{

			stopTunnelLoop();
			exit(TUNNELBROKENERROR);
			return;
		}
		if(!this->serverConnected)
		{
			printf("Sending Connection Request\n");
			if(!this->sendConnectionRequestData())
			{
				printf("Error in sending back connection data\n");
				return ;
			}

		}
		else
		{
			if(clock()-lastSendKeepAliveTime>KEEPALIVEINTERVAL)
			{
						lastSendKeepAliveTime=clock();
						this->sendConnectionKeepAlivePacket();
			}
		}
		FD_ZERO(&fdread);
		FD_SET(agentFd,&fdread);
		if(select(0,&fdread,0,0,&tv)!=0)
		{


				int size=recvfrom(this->agentFd,buf,10240,0,NULL,NULL);
				if(size==SOCKET_ERROR)
				{
					printf("Network Error when recv from udp port:%d\n",WSAGetLastError());
					runFlag=false;
					return;
				}
				if(buf[0]&CONNECTIONREQUESTHEADERTYPE)
				{
					printf("Connection established\n");
					this->serverConnected=true;
					continue;
				}
				if(serverConnected)
				{

					if(buf[0]&CONNECTIONCLOSEHEADERTYPE)
					{
						stopTunnelLoop();
						exit(SHUTDOWNSIGNAL);
						return ;
					}
					else if(buf[0]& CONNECTIONKEEPALIVEHEADERTYPE)
					{
						lastRecvKeepAliveTime=clock();
					}
					else if(buf[0]&AUDIODATAHEADERTYPE)
					{
						if(WaitForSingleObject(this->g_hMutex_audio_network,3)==WAIT_OBJECT_0)
						{
							if(this->audioQueue.size()>MAXWAITQUEUENUM)
							{
								int tsize=this->audioQueue.size();
								for(int i=0;i<tsize;i++)
								{
									free(this->audioQueue.front().first);
									this->audioQueue.pop();
								}
							}
							char *tmpBuf=(char*)malloc(size-HEADERLENGTH);
							memcpy(tmpBuf,buf+HEADERLENGTH,size-HEADERLENGTH);//remove header
							this->audioQueue.push(pair<char*,int>(tmpBuf,size-HEADERLENGTH));
							ReleaseMutex(this->g_hMutex_audio_network);
						}
						else
						{
							printf("Due to lock condition! Loss a packet\n");
						}
					}
					else if(buf[0]&VIDEODATAHEADERTYPE)
					{
						/*if(size<4)//In order to protect following code
						{
							printf("Seems bad packet\n");
							continue;
						}*/
						handleNALPacket(buf,size);

					}
				}

			}


		}



}
void DataTunnel::stopTunnelLoop()
{
	this->serverConnected=false;
	runFlag=false;
}
void DataTunnel::handleNALPacket(char *buf, int size)
{
			bool isLast=(buf[0]&LASTMARKERBIT);
			BYTE head1=*(buf+1);
			BYTE head2=*(buf+2);
			BYTE type=head1&0x1f;

			if(type==0x1c)
			{
				//printf("It's a fu-a nal:%d\n",size-1);
				if(this->nalPacketCursor+size-HEADERLENGTH>MAXVIDEONALPACKETBUFSIZE)
				{
					printf("NAL Buffer full! May have another problem\n");
					this->nalPacketCursor=0;
					return;
				}
				BYTE flag = head2 & 0xe0;
				if(flag==0x80)//start For Begin! We should only discard one bit rather then two bits
				{
					//printf("nal start:%d\n",size-1);
					if(this->nalPacketCursor!=0)
					{
						 this->nalPacketCursor=0;//Drop Bad Packet
						 printf("Drop some bad packet due to packet loss\n");
					}
					buf[2]=(head1&0xE0)+(head2&0x1F);
					memcpy(this->nalPacketBuf+this->nalPacketCursor,buf+HEADERLENGTH+1,size-HEADERLENGTH-1);//1 For FU_A HEADER
					this->nalPacketCursor+=(size-HEADERLENGTH-1);
			    }
				else if (flag==0x40) //end
				{
					//printf("nal end:%d\n",size-1);
					if(this->nalPacketCursor==0)
					{
						 printf("Ignore a end packet\n");
						 return;
					}
					memcpy(this->nalPacketBuf+this->nalPacketCursor,buf+HEADERLENGTH+2,size-HEADERLENGTH-2);
					this->nalPacketCursor+=(size-HEADERLENGTH-2);
					writeNALPacket((char*)nalPacketBuf,nalPacketCursor);
					nalPacketCursor=0;
				}
				else //middle
				{
					//printf("nal middle:%d\n",size-1);
					if(this->nalPacketCursor==0)
					{
						printf("Ignore a middle packet\n");
						return;
					}
					memcpy(this->nalPacketBuf+this->nalPacketCursor,buf+HEADERLENGTH+2,size-HEADERLENGTH-2);
					this->nalPacketCursor+=(size-HEADERLENGTH-2);
				}
			}
			else//Single
			{	
				printf("It's a single nal:%d\n",size-1);
				writeNALPacket(buf+HEADERLENGTH,size-HEADERLENGTH);
			}
			if(isLast)
			{
				//printf("ISLAST\n");
				if(WaitForSingleObject(this->g_hMutex_video_network,5)==WAIT_OBJECT_0)
				{
					if(this->videoQueue.size()>MAXWAITQUEUENUM)
					{
							int tsize=videoQueue.size();
							for(int i=0;i<tsize;i++)
							{
								free(videoQueue.front().first);
								this->videoQueue.pop();
							}
					}
					char *tmpBuf=(char*)malloc(this->videoPacketCursor);
					memcpy(tmpBuf,this->videoPacketBuf,videoPacketCursor);
					videoQueue.push(pair<char*,int>(tmpBuf,this->videoPacketCursor));
					ReleaseMutex(g_hMutex_video_network);
				}
				else
				{
					printf("Due to lock reason! Packet Drop\n");
				}
				this->videoPacketCursor=0;
			}
}
void DataTunnel::writeNALPacket(char *buf, int size)
{
	char header[4]={0x00,0x00,0x00,0x01};
	if(this->videoPacketCursor+size>MAXVIDEOPACKETBUFSIZE)
	{
		this->videoPacketCursor=0;
		printf("buffer full! Drop the packet~\n");
		return;
	}
	memcpy(videoPacketBuf+videoPacketCursor,header,4);
	videoPacketCursor+=4;
	memcpy(videoPacketBuf+videoPacketCursor,buf,size);
	videoPacketCursor+=(size);
}
bool DataTunnel::getVideoData(char **data,int *size)
{
	if(WaitForSingleObject(this->g_hMutex_video_network,INFINITE)==WAIT_OBJECT_0)
	{
		if(this->videoQueue.size()==0)
		{
			ReleaseMutex(this->g_hMutex_video_network);
			return false;
		}
		pair<char*,int> t=this->videoQueue.front();
		*data=t.first;
		*size=t.second;
		this->videoQueue.pop();
		ReleaseMutex(this->g_hMutex_video_network);
		return true;
	}
	else
	{
		printf("Get controller data failed due to video lock error\n");
		return false;
	}
}

void DataTunnel::sendConnectionKeepAlivePacket()
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