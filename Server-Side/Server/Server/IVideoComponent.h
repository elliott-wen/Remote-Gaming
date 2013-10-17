#pragma once
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "IDataTunnel.h"
#include <Windows.h>
#define RESERVEDMEMORY 256
#define SHAREDMEMSIZE 1920*1200*32+RESERVEDMEMORY
#define MAXFPS 200
#define RHEIGHT 800
#define RWIDTH 600
#define RBANDWIDTH 4000000
extern "C"
{
#ifdef HAVE_AV_CONFIG_H
#undef HAVE_AV_CONFIG_H
#endif
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libswscale/swscale.h>
}

class IVideoComponent
{
public:
	IVideoComponent();
	~IVideoComponent();
	bool initVideoComponent();
	void startFrameLoop();
	void stopFrameLoop();
	void setDataTunnel(IDataTunnel *tunnel);
	void setQuality(int width,int height,int bandwidth);
private:
	BYTE* lpvMem;      // pointer to shared memory
	HANDLE hMapObject;
	AVFrame *rawFrame;
	IDataTunnel *tunnel;
	SwsContext *img_convert_ctx; 
	uint8_t *picture_buf;
	int frameCounter;
	int lastWidth,lastHeight,lastBPPFormat;
	int outputWidth,outputHeight;
	bool workingThread;
	void uninstallSharedMemory();
	bool setupSwscale(int in_width,int in_height,int bppFormat);
	void removeSwscale();
	AVFrame* allocFrame(enum PixelFormat pix_fmt, int width, int height);
	bool setupSharedMemory();
	bool isMemoryReadable();
	void setMemoryWritable();
	void removeVideoCodec();
	long fpsClock;
	int bandwidth;
	AVCodec *video_codec;
	AVCodecContext *video_codec_context;
	bool sendOutFrame(AVPacket *packet);
	bool sendOutSliceFrame(const PBYTE pNal,int nalSize,bool isLast);
	bool openVideoEncoder();
	
	void cleanupEncoder();
	bool write_video_frame(AVFrame *frame);
	

};