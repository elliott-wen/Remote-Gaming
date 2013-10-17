#pragma once

#include <stdlib.h>
#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <queue>
using namespace std;
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
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}


class VideoStreamDecoder
{
public:
	VideoStreamDecoder();
	~VideoStreamDecoder();
	
	bool initDecorder();
	bool decodeVideoFrame(char *indata,int insize,AVFrame **frame);
	bool setOutputSize(int outputWidth,int outputHeight);
private:
    AVCodec  *video_codec;
	AVCodecContext *video_codec_context;
	bool openVideoCodec();
	bool setupSwscale();
	void removeSwscale();
	AVPacket videoavpkt;
	AVFrame* videoframe;
	AVFrame* videopicture;
	int lastWidth;
	int lastHeight;
	int outputWidth;
	int outputHeight;
	SwsContext *img_convert_ctx;
	AVFrame* alloc_picture(enum PixelFormat pix_fmt, int width, int height);
	
};