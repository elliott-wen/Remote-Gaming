#include "AudioStreamDecoder.h"
AudioStreamDecoder::AudioStreamDecoder()
{
	this->outputaudioFrame=0;
	this->audio_codec=0;
	this->audioframe=0;
	avcodec_register_all();

}
AudioStreamDecoder::~AudioStreamDecoder()
{
	if(this->audio_codec_context!=0)
	{
		avcodec_close(audio_codec_context);
		this->audio_codec=0;
	}
	if(this->audio_codec!=0)
	{
		av_freep(this->audio_codec);
		this->audio_codec=0;
	}
	if(this->audioframe!=0)
	{
		avcodec_free_frame(&audioframe);
		audioframe=0;
	}
	if(this->outputaudioFrame!=0)
	{
		avcodec_free_frame(&outputaudioFrame);
		outputaudioFrame=0;
	}
	av_free_packet(&audioavpkt);
}

bool AudioStreamDecoder::decodeAudioFrame(char*indata,int insize,AVFrame **outdata,int *outSize)
{
	int len, got_frame;
	audioavpkt.data = (uint8_t*)indata;
	audioavpkt.size=insize;
	avcodec_get_frame_defaults(audioframe);
	len = avcodec_decode_audio4(this->audio_codec_context, audioframe, &got_frame, &audioavpkt);
	if(len<0)
	{
		printf("Error happen when decoding\n");
		return false;
	}
	if (got_frame) 
	{
		*outdata=audioframe;
		*outSize= av_samples_get_buffer_size(NULL, this->audio_codec_context->channels,
                                                       audioframe->nb_samples,
                                                      this->audio_codec_context->sample_fmt, 1);
		//printf("OutSize:%d,format:%d\n",*outSize,audioframe->format);

		return true;
	}
	else
	{
		printf("OOP!Unknown Error in decoding\n");
		return false;
	}
	return false;
}

bool AudioStreamDecoder::openAudioCodec()
{
	this->audio_codec=avcodec_find_decoder(AV_CODEC_ID_OPUS);
	if (!this->audio_codec) 
	{
		printf( "audio codec not found/n");
		return false;
	}
	this->audio_codec_context = avcodec_alloc_context3(this->audio_codec);
	this->audio_codec_context->channels=2;
	this->audio_codec_context->sample_rate=OUTPUTSAMPLERATE;

	if (avcodec_open2(this->audio_codec_context, this->audio_codec,NULL) < 0) 
	{
        printf( "could not open audio codec\n");
        return false;
    }
	return true;
}


bool AudioStreamDecoder::initDecorder()
{

	if(!this->openAudioCodec())
	{
		printf("Can open add video stream@!\n");
	    return false;
	}
	audioframe = avcodec_alloc_frame();
	outputaudioFrame=avcodec_alloc_frame();
    if (!audioframe || !outputaudioFrame) {
        printf("Could not allocate video frame\n");
        return false;
    }
	av_init_packet(&audioavpkt);
	return true;
}
