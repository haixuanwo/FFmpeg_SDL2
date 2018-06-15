
/*
 * 2018-06-10
 * haixuanwo_clark@hotmail.com
 * 
**/

#ifndef _TXH_FFMPEG_H_
#define _TXH_FFMPEG_H_

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
};

class CFFmpeg
{
public:
	CFFmpeg(char *fileName);	
	int FFmpeg_read_frame(unsigned char* data, int &len);
	~CFFmpeg();
	
private:
	AVFormatContext	*pFormatCtx;
	int				i, videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	AVFrame	*pFrame,*pFrameYUV;
	uint8_t *out_buffer;
	AVPacket *packet;
	int y_size;
	int ret, got_picture;
	struct SwsContext *img_convert_ctx;	
};

#endif

