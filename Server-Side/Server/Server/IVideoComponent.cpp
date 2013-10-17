#include "IVideoComponent.h"
IVideoComponent::IVideoComponent()
{

	frameCounter=0;
	fpsClock=clock();
	lastWidth=0;
	lastHeight=0;
	this->outputHeight=RHEIGHT;
	this->outputWidth=RWIDTH;
	this->video_codec_context=0;
	this->video_codec=0;
	this->bandwidth=RBANDWIDTH;
	lpvMem=NULL;
	hMapObject=NULL;
	rawFrame=NULL;
	img_convert_ctx=NULL;
	this->lastBPPFormat=-1;
	this->tunnel=0;
	workingThread=false;
}
IVideoComponent::~IVideoComponent()
{
	if(rawFrame!=NULL)
	{
		av_freep(&rawFrame->data[0]);
		avcodec_free_frame(&rawFrame);
		rawFrame=NULL;
	}
	removeSwscale();
	uninstallSharedMemory();
	cleanupEncoder();
}
bool IVideoComponent::isMemoryReadable()
{
	return lpvMem[SHAREDMEMSIZE/8-1]==1;
}
void IVideoComponent::setMemoryWritable()
{
	lpvMem[SHAREDMEMSIZE/8-1]=0;
}
void IVideoComponent::uninstallSharedMemory()
{
	if(lpvMem!=NULL)
	UnmapViewOfFile(lpvMem);
	lpvMem=NULL;
	if(hMapObject!=NULL)
	CloseHandle(hMapObject);
	hMapObject=NULL;


}
bool IVideoComponent::setupSharedMemory()
{
		hMapObject = CreateFileMapping( 
                INVALID_HANDLE_VALUE,   // use paging file
                NULL,                   // default security attributes
                PAGE_READWRITE,         // read/write access
                0,                      // size: high 32-bits
                SHAREDMEMSIZE,              // size: low 32-bits
                TEXT("ded9dllmemfilemap")); // name of map object
        if (this->hMapObject == NULL) 
            return FALSE; 
		bool fInit = (GetLastError() != ERROR_ALREADY_EXISTS); 
		lpvMem = (BYTE*)MapViewOfFile( 
                hMapObject,     // object to map view of
                FILE_MAP_WRITE, // read/write access
                0,              // high offset:  map from
                0,              // low offset:   beginning
                0);             // default: map entire file
		if (this->lpvMem == NULL) 
           return FALSE; 
		if (fInit) 
           memset(lpvMem, '\0', SHAREDMEMSIZE);

		return TRUE;
}
bool IVideoComponent::initVideoComponent()
{
	if(!setupSharedMemory())
	{
		printf("Memory Failed\n");
		return false;
	}
	if(!openVideoEncoder())
	{
		printf("Video Encoder Failed\n");
		return false;
	}


	workingThread=true;
	return true;
}
void IVideoComponent::startFrameLoop()
{
	while(workingThread)
	{
		//if(isMemoryReadable())
		if(isMemoryReadable()&&tunnel!=NULL&&tunnel->isClientConnected())//Safe Protect
		{	

			UINT copySize=0;
			UINT height=0;
			UINT width=0;
			UINT bpp=0;
			memcpy((void *)&copySize,lpvMem+(SHAREDMEMSIZE-RESERVEDMEMORY)/8,sizeof(UINT));
			memcpy((void *)&height,lpvMem+(SHAREDMEMSIZE-RESERVEDMEMORY)/8+sizeof(height),sizeof(height));
			memcpy((void *)&width,lpvMem+(SHAREDMEMSIZE-RESERVEDMEMORY)/8+sizeof(height)*2,sizeof(width));
			memcpy((void *)&bpp,lpvMem+(SHAREDMEMSIZE-RESERVEDMEMORY)/8+sizeof(height)*3,sizeof(bpp));
			if(copySize<0 || height<0 ||width<0 || height>1200 ||width>1920)
			{

				printf("Bad Config! %d %d %d %d Waiting New Config\n",copySize,height,width,bpp);
				setMemoryWritable();
				continue;
			}
			if(bpp<1 || bpp>3 )
			{
				printf("Currently color not supported");
				setMemoryWritable();
				continue;
			}
			int fps=1000/((clock()-fpsClock)+1);//avoid divide 0
			fpsClock=clock();
			printf("%d bytes height:%d width:%d bppformat:%d FPS:%d \n",copySize,height,width,bpp,fps);
			//===============RGB32toYUV420P===================
			if(lastWidth!=width||lastHeight!=height||lastBPPFormat!=bpp)
			{
				printf("Swscale Rebuilt\n");
				lastWidth=width;
				lastHeight=height;
				lastBPPFormat=bpp;
				this->removeSwscale();
				if(!this->setupSwscale(width,height,bpp))
				{
					printf("Swscale failed\n");
					workingThread=false;
					return;
				}	

			}
			if(rawFrame==NULL)
			{
				rawFrame=allocFrame(PIX_FMT_YUV420P, this->outputWidth, this->outputHeight);
			}
			uint8_t *rgb_src[3]={lpvMem,NULL,NULL};
			int rgb_stride[3]={0,0,0};
			if(bpp==1)//RGB32
			{
				rgb_stride[0]=width*4;
			}
			else
			{
				rgb_stride[0]=width*2;
			}
			sws_scale(img_convert_ctx, rgb_src, rgb_stride, 0, height, rawFrame->data, rawFrame->linesize);

			//=============Write encoded frame and send=============================
			write_video_frame(rawFrame);
			//==========================================
			//
			setMemoryWritable();
		}
		else
		{
			//printf("Try to see if i sleep too much");
			Sleep(1000/MAXFPS);
		}
	}
}
void IVideoComponent::stopFrameLoop()
{
	workingThread=false;
}
bool IVideoComponent::setupSwscale(int in_width,int in_height,int bpp)
{
	AVPixelFormat originFormat=PIX_FMT_RGB32;
	switch(bpp)
	{
		case 1:
			originFormat=AV_PIX_FMT_RGB32;
			break;
		case 2:
			originFormat=AV_PIX_FMT_RGB565;
			break;
		case 3:
			originFormat=AV_PIX_FMT_RGB555;
			break;
	}
	img_convert_ctx = sws_getContext(in_width, in_height, originFormat, 
		this->outputWidth, this->outputHeight, PIX_FMT_YUV420P, SWS_BILINEAR, 
	NULL, NULL, NULL);
	if(img_convert_ctx == NULL) { 
	printf( "Cannot initialize the conversion context!\n"); 
	return false; 
	}
	return true;
}
void IVideoComponent::removeSwscale()
{
	if(img_convert_ctx!=NULL)
	{
		sws_freeContext(img_convert_ctx);
		img_convert_ctx=NULL;
	}

}
AVFrame *IVideoComponent::allocFrame(enum PixelFormat pix_fmt, int width, int height)
{
	AVFrame *picture = avcodec_alloc_frame();
    if (!picture || avpicture_alloc((AVPicture *)picture, pix_fmt, width, height) < 0)
        av_freep(&picture);
	picture->pts=0;
    return picture;
}
void IVideoComponent::setQuality(int width,int height,int bandwidth)
{
	this->outputWidth=width;
	this->outputHeight=height;
	this->bandwidth=bandwidth;
}
bool IVideoComponent::sendOutFrame(AVPacket *pkt)
{
	PBYTE p_buffer = pkt->data;
	int	i_buffer = pkt->size;
	//printf("=====================================Total Size:%d\n",i_buffer);


	while( i_buffer > 4 && ( p_buffer[0] != 0 || p_buffer[1] != 0 || p_buffer[2] != 1 ) )
	{
		i_buffer--;
		p_buffer++;
	}
	while( i_buffer > 4 )
	{
		int i_offset;
		int i_size = i_buffer;
		int i_skip = i_buffer;

	/* search nal end */
		for( i_offset = 4; i_offset+2 < i_buffer ; i_offset++)
		{
			if( p_buffer[i_offset] == 0 && p_buffer[i_offset+1] == 0 && p_buffer[i_offset+2] == 1 )
			{
				/* we found another startcode */
				i_size = i_offset - ( p_buffer[i_offset-1] == 0 ? 1 : 0);
				i_skip = i_offset;
				break;
			} 
		}
		int iWrite=0;
		//printf("Got Nal:%d\n",i_size);
	/* TODO add STAP-A to remove a lot of overhead with small slice/sei/... */
	//UINT iWrite = TransportH264Nal(p_buffer, i_size, pts, (i_size >= i_buffer) );
		sendOutSliceFrame(p_buffer,i_size,(i_size >= i_buffer));
		i_buffer -= i_skip;
		p_buffer += i_skip;
	}
	return true;
}
bool IVideoComponent::sendOutSliceFrame(const PBYTE pNal,int nalSize,bool isLast)
{
	if( nalSize < 5 )
		return false;
	int i_max = NETWORKMTU; /* payload max in one packet */
	int i_nal_hdr;
	int i_nal_type;
	i_nal_hdr = pNal[3];
	i_nal_type = i_nal_hdr&0x1f;
	//printf("Nal Type:%d\n",i_nal_type);
	PBYTE p_data = pNal;
	int	i_data = nalSize;
	p_data += 3;
	i_data -= 3;
	if( i_data <= i_max )
	{
		//printf("Single NAL I_data :%d\n",i_data);
		if(tunnel!=NULL&&tunnel->isClientConnected())
		{
			tunnel->sendVideoData((char*)p_data,i_data,isLast);
		}
	}
	else
	{
		int i_count = ( i_data-1 + i_max-2 - 1 ) / (i_max-2);
		int i;
		p_data++;
		i_data--;
		for( i = 0; i < i_count; i++ )
		{
			int i_payload =  (i_data < (i_max-2)) ? i_data : (i_max-2);
			int nalSize = 2 + i_payload;
			char *packetBuf=(char*)malloc(nalSize);
			packetBuf[0]= 0x00 | (i_nal_hdr & 0x60) | 28;
			packetBuf[1]= ( i == 0 ? 0x80 : 0x00 ) | ( (i == i_count-1) ? 0x40 : 0x00 )  | i_nal_type;
			memcpy(packetBuf+2, p_data, i_payload);
			//printf("FU-A NAL:%d IsStart:%d\n",nalSize,i);
			if(tunnel!=NULL&&tunnel->isClientConnected())
			{

				tunnel->sendVideoData((char*)packetBuf,nalSize,isLast && (i == i_count-1));
			}
			free(packetBuf);
			i_data -= i_payload;
			p_data += i_payload;
		}
	}
	return true;
}
bool IVideoComponent::write_video_frame(AVFrame *frame)
{
	//long encodeVideoPerformanceClock=clock();
	AVCodecContext *c = this->video_codec_context;
	AVPacket pkt;
    int got_output,ret;
    av_init_packet(&pkt);
    pkt.data = NULL;    // packet data will be allocated by the encoder
    pkt.size = 0;
	ret = avcodec_encode_video2(c, &pkt, frame, &got_output);

    if (ret < 0) 
	{
        printf( "Error encoding video frame\n");
        return false;
    }
	if (got_output) 
	{

		ret = sendOutFrame(&pkt);
		av_free_packet(&pkt);
		if(ret<0)
		{
			printf( "Error writing video frame\n");
			return false;
		}
    }
	else
	{
		printf( "Unknown Error in Encoding! But just return true\n");
	}
	//printf("Encoding Performance %d ms\n",clock()-encodeVideoPerformanceClock);
	return true;
}
bool IVideoComponent::openVideoEncoder()
{
	this->video_codec = avcodec_find_encoder(CODEC_ID_H264);
	if (!this->video_codec) 
	{
		printf( "aideo codec not found/n");
		return false;
    }
	this->video_codec_context=avcodec_alloc_context3(this->video_codec);
	this->video_codec_context->codec_id=CODEC_ID_H264;
	this->video_codec_context->bit_rate = this->bandwidth;
	this->video_codec_context->width = this->outputWidth;
	this->video_codec_context->height = this->outputHeight;
	this->video_codec_context->gop_size=0;
	this->video_codec_context->max_b_frames=0;
    this->video_codec_context->pix_fmt = PIX_FMT_YUV420P;
	this->video_codec_context->me_range = 16;
    this->video_codec_context->max_qdiff = 4;
    this->video_codec_context->qmin = 10;
    this->video_codec_context->qmax = 51;
	this->video_codec_context->refs=1;
	this->video_codec_context->dia_size=1;
	this->video_codec_context->keyint_min=46;
	this->video_codec_context->active_thread_type= FF_THREAD_SLICE;
	this->video_codec_context->thread_type=FF_THREAD_SLICE;
	this->video_codec_context->thread_count=4;
	this->video_codec_context->slices=4;
	/*c->rc_max_rate=6500000;
	c->rc_buffer_size=260000;*/
	this->video_codec_context->time_base.den = 25;
    this->video_codec_context->time_base.num = 1;
	av_opt_set(this->video_codec_context->priv_data, "tune", "zerolatency", 0);
	av_opt_set(this->video_codec_context->priv_data, "preset","faster",0);
	av_opt_set(this->video_codec_context->priv_data,"intra-refresh","1",0);
	if( avcodec_open2(this->video_codec_context, this->video_codec, NULL)<0)
	{
		printf( "could not open video codec\n");
        return false;
	}
	return true;
}
void IVideoComponent::cleanupEncoder()
{
	if(this->video_codec_context!=NULL)
	{
		avcodec_close(this->video_codec_context);
		av_freep(&(this->video_codec_context));
	}
	if(this->video_codec!=NULL)
	{
		av_freep(&video_codec);
	}

}
void IVideoComponent::setDataTunnel(IDataTunnel *tunnel)
{
	this->tunnel=tunnel;
}