#include "VideoStreamDecoder.h"
VideoStreamDecoder::VideoStreamDecoder()
{
	this->video_codec=0;
	this->videoframe=0;
	this->videopicture=0;
	this->lastWidth=0;
	this->lastHeight=0;
	this->img_convert_ctx=0;
	this->outputHeight=RHEIGHT;
	this->outputWidth=RWIDTH;
	avcodec_register_all();
}
VideoStreamDecoder::~VideoStreamDecoder()
{
	if(this->video_codec_context!=0)
	{
		avcodec_close(video_codec_context);
		video_codec=0;

	}
	if(this->video_codec!=0)
	{
		av_freep(this->video_codec);
		this->video_codec=0;
	}
	this->removeSwscale();
	if(this->videoframe!=0)
	{
		avcodec_free_frame(&videoframe);
		videoframe=0;
	}
	if(this->videopicture!=0)
	{
		avcodec_free_frame(&videopicture);
		videopicture=0;
	}
	av_free_packet(&videoavpkt);

}


bool VideoStreamDecoder::decodeVideoFrame(char*data,int size,AVFrame **getframe)
{
	int len, got_frame;
	videoavpkt.data = (uint8_t*)data;
	videoavpkt.size=size;
	len = avcodec_decode_video2(this->video_codec_context, videoframe, &got_frame, &videoavpkt);
	if(len<0)
	{
		printf("Error happen when decoding\n");
		return false;
	}
	if (got_frame) 
	{
		if(lastHeight!=videoframe->height || lastWidth!=videoframe->width)
		{
			removeSwscale();
			if(!setupSwscale())
			{
				printf("Error happen when setting up swscale!\n");
				return false;
			}
		}
		sws_scale(img_convert_ctx, videoframe->data, videoframe->linesize,0, videoframe->height, videopicture->data, videopicture->linesize);  
		*getframe=videopicture;
		return true;
	}
	else
	{
		printf("Bad packet! OOP! Maybe next will be good!\n");
		return false;
	}
	return true;
}

bool VideoStreamDecoder::openVideoCodec()
{

	this->video_codec = avcodec_find_decoder(AV_CODEC_ID_H264);
	if (!this->video_codec) 
	{
		printf( "video codec not found/n");
		return false;
    }
	this->video_codec_context = avcodec_alloc_context3(this->video_codec);
	if (avcodec_open2(this->video_codec_context, this->video_codec,NULL) < 0) 
	{
        printf( "could not open video codec\n");
        return false;
    }
	return true;
}

bool VideoStreamDecoder::initDecorder()
{

	if(!this->openVideoCodec())
	{
		printf("Can open add video stream@!\n");
	    return false;
	}
	videoframe = avcodec_alloc_frame();
    if (!videoframe) {
        printf("Could not allocate video frame\n");
        return false;
    }
	av_init_packet(&videoavpkt);
	videopicture=alloc_picture(PIX_FMT_YUV420P, this->outputWidth, this->outputHeight);
	return true;
}
bool VideoStreamDecoder::setOutputSize(int outputWidth,int outputHeight)
{
	this->outputHeight=outputHeight;
	this->outputWidth=outputWidth;
	this->lastWidth=0;
	this->lastHeight=0;//Require Refresh!
	return true;
}
bool VideoStreamDecoder::setupSwscale()
{
	img_convert_ctx = sws_getContext(videoframe->width, videoframe->height, this->video_codec_context->pix_fmt, 
	outputWidth, outputHeight, PIX_FMT_YUV420P, SWS_BILINEAR, 
	NULL, NULL, NULL);
	if(img_convert_ctx == NULL) { 
	printf( "Cannot initialize the conversion context!\n"); 
	return false; 
	}
	return true;
}
void VideoStreamDecoder::removeSwscale()
{
	if(img_convert_ctx!=NULL)
	{
		sws_freeContext(img_convert_ctx);
		img_convert_ctx=NULL;
	}

}
AVFrame *VideoStreamDecoder::alloc_picture(enum PixelFormat pix_fmt, int width, int height)
{
	AVFrame *picture = avcodec_alloc_frame();
    if (!picture || avpicture_alloc((AVPicture *)picture, pix_fmt, width, height) < 0)
        av_freep(&picture);
    return picture;
}
