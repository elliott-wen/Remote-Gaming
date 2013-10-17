#include "stdafx.h"
#include "getopt.h"
#include "VideoStreamDecoder.h"
#include "AudioStreamDecoder.h"
#include <SDL.h>
#include "DataTunnel.h"
#include <queue>
#include <process.h>
#include "Controller.h"
using namespace std;
#define HD1280_720_6M 1
#define SD800_600_4M 2
#define CD640_480_2M 3
#define LD320_240_1M 4

#define AUDIOPLAYBUFSIZE 51200
//*-**********************SOUND**************
static void getAudioFromBuffer(void *udata, Uint8 *stream, int len);
static void decodeAudioFromQueue();
//***************************SDL***************
static void initSDL();
//********************Network**************
static void initTunnelNetwork();
static void networkThread(void *);
//**********************Decoder**************
static void initDecoder();
//===================Controller===================
static void initController();
//====================Library=================
static void initExternLibrary();
//=====================Video================
static void SDL_VideoDisplayThread(void *);
//=================Argument=====================
static void handleArgument();
//============PUBLIC API===========
static void shutdownClient();
static void initLogger();
