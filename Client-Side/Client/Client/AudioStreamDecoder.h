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


class AudioStreamDecoder
{
public:
	AudioStreamDecoder();
	~AudioStreamDecoder();
	bool initDecorder();
	bool decodeAudioFrame(char *indata,int insize,AVFrame **frame,int *outSize);
private:
    AVCodec *audio_codec;
	AVCodecContext *audio_codec_context;
	bool openAudioCodec();
	AVFrame *outputaudioFrame;
	AVPacket audioavpkt;
	AVFrame* audioframe;
	
};