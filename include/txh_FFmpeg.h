/*
 * @Author: Clark
 * @Email: haixuanwoTxh@gmail.com
 * @Date: 2021-11-03 12:46:36
 * @LastEditors: Clark
 * @LastEditTime: 2021-11-03 18:47:12
 * @Description: file content
 */

#ifndef _TXH_FFMPEG_H_
#define _TXH_FFMPEG_H_

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
};

#include <iostream>

class CFFmpeg
{
public:
	CFFmpeg(std::string &fileName);
	int FFmpeg_read_frame(unsigned char* data, int &len);
	unsigned short get_width();
    unsigned short get_height();
	~CFFmpeg();

private:
	AVFormatContext	*pFormatCtx;
	int				i, videoindex;
	AVCodecContext	*pCodecCtx;
	AVCodec			*pCodec;
	AVFrame	*pFrame,*pFrameYUV;
	uint8_t *out_buffer;
	AVPacket *packet;
	int ret, got_picture;
	struct SwsContext *img_convert_ctx;
};

#endif

