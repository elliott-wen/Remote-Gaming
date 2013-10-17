#include "ISoundComponent.h"
#include "IVideoComponent.h"
#include "IController.h"
#include "getopt.h"
#include <process.h>
#define HD1280_720_6M 1
#define SD800_600_4M 2
#define CD640_480_2M 3
#define LD320_240_1M 4
static IController controller;
static IVideoComponent vComponent;
static ISoundComponent aComponent;
static IDataTunnel tunnel;
static WSADATA wsaData;
static bool runFlag;
static void initExternLibrary();
static void shutdownServer();
static void handleArgument(int argc, char* argv[]);
static void videoComponentThread(void*);
static void audioComponentThread(void*);
static void controllerThread(void *);
static void tunnelThread(void *);
static void instanceProtect();
static bool consoleHandler( DWORD fdwctrltype );
static bool consoleHandler( DWORD fdwctrltype )
{
	switch( fdwctrltype ) 
    { 

		case CTRL_C_EVENT: 

		case CTRL_CLOSE_EVENT: 

		case CTRL_BREAK_EVENT: 
 
		case CTRL_LOGOFF_EVENT: 
 
		case CTRL_SHUTDOWN_EVENT: 
			runFlag=false;
			//shutdownServer();
			return true;

		default: 
			return false; 
    } 

}
static void initExternLibrary()
{
	WSAStartup(MAKEWORD(2,1),&wsaData);
	avcodec_register_all();
}
static void shutdownServer()
{
	tunnel.sendConnectionCloseRequest();
	tunnel.stopTunnelLoop();
	vComponent.stopFrameLoop();
	aComponent.stopFrameLoop();
	controller.stopControllerLoop();
	Sleep(2000);
	WSACleanup();
	printf("System going to shutdown\n");
	Sleep(5000);

}
static void tunnelThread(void *)
{
	tunnel.startTunnelLoop();
	runFlag=false;

}
static void videoComponentThread(void*)
{
	vComponent.startFrameLoop();
	runFlag=false;

}
static void audioComponentThread(void*)
{
	aComponent.startFrameLoop();
	runFlag=false;
}
static void controllerThread(void *)
{
	controller.startControllerLoop();
	runFlag=false;
}


static void handleArgument(int argc, char* argv[])
{
	 int c;
	 int digit_optind = 0;
	 while (true)
      {
			int this_option_optind = optind ? optind : 1;
			c = getopt (argc, argv, "q:p:");
			if (c == -1)
				break;

			switch (c)
			{
				case 'q':
					{
						int quality=atoi(optarg);
						switch(quality)
						{
							case HD1280_720_6M:
								printf("Using HD Mode:1280*720\n");
								vComponent.setQuality(1280,720,6000000);
								break;
							case SD800_600_4M:
								printf("Using SD Mode:800*600\n");
								vComponent.setQuality(800,600,4000000);
								break;
							case CD640_480_2M:
								printf("Using CD Mode:640*480\n");
								vComponent.setQuality(640,480,2000000);
								break;
							case LD320_240_1M:
								printf("Using LD Mode:320*240\n");
								vComponent.setQuality(320,240,1000000);
								break;
							default:
								vComponent.setQuality(320,240,1000000);
						}
					}
					break;
				case 'p':
					{
						int localPort=atoi(optarg);
						if(localPort>1024&&localPort<65534)
						{
							tunnel.setLocalPort(localPort);
						}
					}
					break;


			}
    }

}
static void instanceProtect()
{
	CreateMutexA(NULL,true,"SHOULDNOTRUNTOOMUCHINSAMEMACHINE"); 
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		printf("Can not run one more server in same time\n");
		exit(-11);
	}
}
int main(int argc, char* argv[])
{
	instanceProtect();
	handleArgument(argc,argv);
	initExternLibrary();
	if(!tunnel.initDataTunnel())
	{
		printf("Init data tunnel failed\n");
		return -7;
	}
	if(!controller.initController())
	{
		printf("Init controller failed\n");
		return -2;
	}
	if(!vComponent.initVideoComponent())
	{
		printf("Video Component Server Failed\n");
		return -1;	
	}
	if(!aComponent.initSoundComponent())
	{
		printf("Audio Component Server Failed\n");
		return -1;	
	}
	aComponent.setDataTunnel(&tunnel);
	vComponent.setDataTunnel(&tunnel);
	controller.setDataTunnel(&tunnel);
	atexit(shutdownServer);
	SetConsoleCtrlHandler( (PHANDLER_ROUTINE) consoleHandler, true ) ;
	_beginthread(videoComponentThread,0,NULL);
	_beginthread(audioComponentThread,0,NULL);
	_beginthread(controllerThread,0,NULL);
	_beginthread(tunnelThread,0,NULL);
	
	runFlag=true;
	while(runFlag)
	{
		Sleep(500);
	}
	return 0;


}