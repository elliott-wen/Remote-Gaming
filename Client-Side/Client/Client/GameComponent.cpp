#include "GameComponent.h"
//==============Application status Flag=======================
static bool runFlag=false;
//==========Sound=====================
static unsigned char audioplayBuf[AUDIOPLAYBUFSIZE];
static int audioplaycursor=0;
//******************SDL*******************
static SDL_Surface *screen;
HANDLE screenLock;
static SDL_Overlay *screenOverlay;
static SDL_AudioSpec wanted;
static bool isFullScreen=false;
static int outputWidth=RWIDTH;
static int outputHeight=RHEIGHT;
//================Network========================
static WSADATA wsaData;
static DataTunnel tunnel;
//==================Decoder======================
static AudioStreamDecoder adecoder;
static VideoStreamDecoder vdecoder;
//===============Controntoler====================
static Controller controller;
//===============================================
static void shutdownClient()
{

	runFlag=false;
	tunnel.stopTunnelLoop();
	tunnel.sendConnectionCloseRequest();
	SDL_CloseAudio();
	SDL_FreeSurface(screen);
	_exit(0);
	//WSACleanup();
	//SDL_Quit();

}
//=========================================================
static void decodeAudioFromQueue()
{
	AVFrame *frame;
	int outSize;
	char *data;
	int size;

	if(tunnel.isServerConnected()&&tunnel.getAudioData(&data,&size))
	{
		if(adecoder.decodeAudioFrame((char *)data,size,&frame,&outSize))
		{
			if(audioplaycursor+outSize>AUDIOPLAYBUFSIZE)
			{
				audioplaycursor=0;
			}
			memcpy(audioplayBuf+audioplaycursor,frame->data[0],outSize);
			audioplaycursor+=outSize;
		}
		free(data);
	}

}
static void getAudioFromBuffer(void *udata, Uint8 *stream, int len)
{
	while(audioplaycursor<len)
	{
		if(!runFlag)
		{
			return;
		}
		decodeAudioFromQueue();
		if(audioplaycursor<len)
		{
			Sleep(1);
		}
	}
	memcpy(stream,audioplayBuf,len);
	memmove(audioplayBuf,audioplayBuf+len,audioplaycursor-len);
	audioplaycursor-=len;
}
//**************Network********************************
static void initTunnelNetwork()
{
	if(!tunnel.initDataTunnel())
	{

		exit(TUNNELINITERROR);
	}
}
static void networkThread(void *)
{
	tunnel.startTunnelLoop();
	runFlag=false;
}
//==========SDL*******==========================
static void initSDL()
{
	if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1)) 
	{ 
        //printf("Could not initialize SDL: %s.\n", SDL_GetError());
       exit(INITAUDIOERROR);
    }


	//=============Video Screen=======================
	screen = SDL_SetVideoMode(outputWidth, outputHeight, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	if ( screen == NULL ) {
        //printf("Couldn't set 640x480x32 video mode: %s\n",SDL_GetError());
        exit(INITVIDEOERROR);
    }
	screenLock=CreateMutexA(NULL, FALSE, "ScreenMutex");
	if(screenLock==INVALID_HANDLE_VALUE)
	{
		exit(INITVIDEOERROR);
	}
	screenOverlay=SDL_CreateYUVOverlay(outputWidth,outputHeight,SDL_IYUV_OVERLAY,screen);
	SDL_WM_SetCaption("Cloud Gaming",NULL);
	SDL_ShowCursor(0);
	//SDL_WM_GrabInput( SDL_GRAB_ON );
	//=============Sound==============================
	wanted.freq = OUTPUTSAMPLERATE;//音频的频率 
	wanted.format = AUDIO_S16SYS;//数据格式为有符号16位		
	wanted.channels = 2;//双声道 
	wanted.samples = AUDIOBUFFERNUM;//采样数 
	wanted.callback = getAudioFromBuffer;//设置回调函数 
	wanted.userdata = NULL; 
	if(SDL_OpenAudio(&wanted, NULL) < 0) 
	{  

		exit(INITAUDIOERROR); 
	}	  
	SDL_PauseAudio(0);
}
//================Decoder====================
static void initDecoder()
{

	if(!vdecoder.initDecorder())
	{

		exit(INITDECODERERROR);
	}
	if(!adecoder.initDecorder())
	{

		exit(INITDECODERERROR);
	}

}
static void initController()
{
	if(!controller.initController())
	{
		exit(INITCONTROLERERROR);
	}
	controller.SetDataTunnel(&tunnel); 
}
//==================Video======================
static void SDL_VideoDisplayThread(void *)
{
	SDL_Rect rect;  
	while(runFlag)
	{
		Sleep(GUISLEEPTIME);

		AVFrame* frame;
		char *data;
		int size;
		if(tunnel.isServerConnected()&&tunnel.getVideoData(&data,&size))
		{

			if(vdecoder.decodeVideoFrame((char*)data,size,&frame))
			{
				WaitForSingleObject(screenLock,INFINITE);
				SDL_LockYUVOverlay(screenOverlay);
				screenOverlay->pixels[0]=frame->data[0];
				screenOverlay->pixels[1]=frame->data[1];
				screenOverlay->pixels[2]=frame->data[2];
				screenOverlay->pitches[0]=frame->linesize[0];
				screenOverlay->pitches[1]=frame->linesize[1];
				screenOverlay->pitches[2]=frame->linesize[2];
				rect.w = outputWidth;  
				rect.h = outputHeight;  
				rect.x=0;
				rect.y=0;
				SDL_DisplayYUVOverlay(screenOverlay, &rect);
				SDL_UnlockYUVOverlay(screenOverlay);
				ReleaseMutex(screenLock);
			}
			free(data);
		}

	}

}
//==========================================
static void initExternLibrary()
{
	WSAStartup(MAKEWORD(2,1),&wsaData);
	avcodec_register_all();
}
//==============================
static void handleArgument()
{
	 int c;
	 int digit_optind = 0;

	 while (true)
      {
			int this_option_optind = optind ? optind : 1;
			c = getopt (__argc, __argv, "a:p:q:l:");
			if (c == -1)
				break;
			switch (c)
			{
				case 'q':
				{
					int quality=atoi(optarg);//1 Means HD(8M 1024*768), 2 Means Common(4M,800*600) , 3 Means Low Quality (2M 640*480) , 4 Means Low band(1M, 320*240); 
					switch(quality)
					{
						case HD1280_720_6M:
							vdecoder.setOutputSize(1280,720);
							outputWidth=1280;
							outputHeight=720;
							break;
						case SD800_600_4M:
							vdecoder.setOutputSize(800,600);
							outputWidth=800;
							outputHeight=600;
							break;
						case CD640_480_2M:
							vdecoder.setOutputSize(640,480);
							outputWidth=640;
							outputHeight=480;
							break;
						case LD320_240_1M:
							vdecoder.setOutputSize(320,240);
							outputWidth=320;
							outputHeight=240;
							break;
						default:
							vdecoder.setOutputSize(320,240);
							outputWidth=320;
							outputHeight=240;
							break;
					}
				}
				break;
				case 'l':
				{
					int localPort=atoi(optarg);
					if(localPort<65534&&localPort>1024)
					{
						tunnel.setLocalPort(localPort);
					}
				}
				break;
				case 'a':
				{
					tunnel.setEndpointIPAddr(optarg);
				}
				break;
				case 'p':
				{
					int remotePort=atoi(optarg);
					if(remotePort<65534&&remotePort>1024)
					{
						tunnel.setEndpointPort(remotePort);
					}
				}
				break;
			}
	 }

	if(__argc==5)//Quality And Port Set
	{

		int remotePort=atoi(__argv[3]);
		if(remotePort<65534&&remotePort>1024)
		{
			tunnel.setEndpointAddr(__argv[2],remotePort);
		}

	}
}

int WINAPI WinMain( HINSTANCE hInst , HINSTANCE hPrev , LPSTR line , int CmdShow )
{
	
	handleArgument();
	initExternLibrary();
	initSDL();
	initDecoder();
	initTunnelNetwork();
	initController();
	SDL_Event event;
	_beginthread(networkThread,NULL,NULL);
	_beginthread(SDL_VideoDisplayThread,NULL,NULL);
	atexit(shutdownClient);
	runFlag=true;
	while(runFlag)
	{
		 Sleep(GUISLEEPTIME);
		 while( SDL_PollEvent( &event ) )
		 {
                switch( event.type )
				{
             
                    case SDL_KEYDOWN:
						if(event.key.keysym.sym==SDLK_F2)
						{
							SDL_WM_GrabInput(SDL_GRAB_OFF);
							break;
						}
						else if(event.key.keysym.sym==SDLK_F3)
						{
							WaitForSingleObject(screenLock,INFINITE);
							if(!isFullScreen)
							{
								screen = SDL_SetVideoMode(outputWidth, outputHeight, 32, SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
								isFullScreen=true;
							}
							else
							{
								screen = SDL_SetVideoMode(outputWidth, outputHeight, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
								isFullScreen=false;
							}
							ReleaseMutex(screenLock);
						}
						else
						{
						//LOG4CPLUS_INFO(Logger::getInstance(TEXT("CLIENTLOG")),"%d%d\n",event.key.keysym.sym,event.key.keysym.mod);
							controller.sendKeyEvent(event.key.keysym.sym,event.key.keysym.mod,PRESSDOWNDIRECTION);
						}				   
						break;
					case SDL_KEYUP:
						controller.sendKeyEvent(event.key.keysym.sym,0,PRESSUPDIRECTION);
						break;
					case SDL_MOUSEMOTION:
						//LOG4CPLUS_INFO(Logger::getInstance(TEXT("CLIENTLOG")),"Mouse moved by %d,%d to (%d,%d)\n",event.motion.xrel, event.motion.yrel,event.motion.x, event.motion.y);
						controller.sendMouseEvent(event.motion.xrel, event.motion.yrel,0,0);
					break;

					case SDL_MOUSEBUTTONDOWN:
						if(SDL_WM_GrabInput(SDL_GRAB_QUERY)==SDL_GRAB_OFF)
						{
							SDL_WM_GrabInput(SDL_GRAB_ON);
							break;
						}
						//LOG4CPLUS_INFO(Logger::getInstance(TEXT("CLIENTLOG")),"Mouse button %d pressed at (%d,%d,%d,%d)\n",event.button.button, event.button.x, event.button.y,event.motion.xrel,event.motion.yrel);
						controller.sendMouseEvent(0,0,event.button.button,PRESSDOWNDIRECTION);
						break;

					case SDL_MOUSEBUTTONUP:
						controller.sendMouseEvent(0,0,event.button.button,PRESSUPDIRECTION);
						break;

                    case SDL_QUIT:
                        runFlag = false;
                        break;

                    default:
						break;
				 }
		 }
	}
	
	return 0;
}


