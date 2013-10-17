#pragma once
#include "stdafx.h"
#include <audioclient.h>
#include <stdio.h>
#include <avrt.h>
#include <iostream>
#include <fstream>
#include <vector> 
#include <iterator>
#include <mmsystem.h>
#include <mmdeviceapi.h>
#include "IDataTunnel.h"

extern "C"
{
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
#define SOUNDCAPTUREMAXBUFSIZE 4096*4
#define OUTPUTSAMPLERATE 48000
#define REQUESTRECORDBLOCK 4
#define REQEUSTRECORDCHANNEL 2
#define REQUESTBIT 16

class ISoundComponent
{
public:
	ISoundComponent();
	~ISoundComponent();
	void setDataTunnel(IDataTunnel *tunnel);
	bool initSoundComponent();
	void startFrameLoop();
	void stopFrameLoop();

private:
	IMMDeviceEnumerator *enumerator;
	IMMDevice *device ;
	IAudioClient *audioClient;
	WAVEFORMATEX *waveFormat;
	IAudioCaptureClient *audioCaptureClient;
	
	SwrContext *swr_ctx;
	bool runFlag;
	bool setupSwscale();
	void removeSwscale();
	int alloc_samples_array_and_data(uint8_t ***data, int *linesize, int nb_channels,
                                    int nb_samples, enum AVSampleFormat sample_fmt, int align);

	AVFrame *frame;
	bool sendChunkedData(BYTE *data,int dataSize);
	BYTE* soundBuffer;
	int soundBufferCursor;

	IDataTunnel *tunnel;
	//***************Encoder*******************
	AVCodec *audio_codec;
	AVCodecContext *audio_codec_context;
	bool openAudioEncoder();
	bool write_audio_frame(AVFrame *frame);
	int getRequestedFrameSize() const;
	void cleanupEncoder();
	//******************************************

};