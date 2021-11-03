
#include "txh_FFmpeg.h"

CFFmpeg::CFFmpeg(std::string &fileName)
{
	av_register_all();
	avformat_network_init();
	pFormatCtx = avformat_alloc_context();

	if(avformat_open_input(&pFormatCtx, fileName.c_str(), nullptr, nullptr) !=0 ){
		printf("Couldn't open input stream.\n");
		exit(-1);
	}

	if(avformat_find_stream_info(pFormatCtx, nullptr)<0){
		printf("Couldn't find stream information.\n");
		exit(-1);
	}

	videoindex=-1;
	for(i=0; i<pFormatCtx->nb_streams; i++)
	{
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
		{
			videoindex=i;
			break;
		}
	}

	if(videoindex==-1){
		printf("Didn't find a video stream.\n");
		exit(-1);
	}

	pCodecCtx=pFormatCtx->streams[videoindex]->codec;
	pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL){
		printf("Codec not found.\n");
		exit(-1);
	}
	if(avcodec_open2(pCodecCtx, pCodec, nullptr)<0){
		printf("Could not open codec.\n");
		exit(-1);
	}

	pFrame=av_frame_alloc();
	pFrameYUV=av_frame_alloc();
	out_buffer=(uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
	avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);
	packet=(AVPacket *)av_malloc(sizeof(AVPacket));
	//Output Info-----------------------------
	printf("--------------- File Information ----------------\n");
	av_dump_format(pFormatCtx, 0, fileName.c_str(), 0);
	printf("-------------------------------------------------\n");
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, nullptr, nullptr, nullptr);
}

int CFFmpeg::FFmpeg_read_frame(unsigned char* data, int &len)
{
	int tmp_len = pCodecCtx->width*pCodecCtx->height;
	int ret = -1;
	ret = av_read_frame(pFormatCtx, packet);
	if(0 <= ret)
	{
		if(packet->stream_index==videoindex){

			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
			if(ret < 0)
			{
				printf("Decode Error.\n");
				return -1;
			}

			if(got_picture)
			{
				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
					pFrameYUV->data, pFrameYUV->linesize);

				memcpy(data, pFrameYUV->data[0], tmp_len);   // Y数据
				data += tmp_len;
				memcpy(data, pFrameYUV->data[1], tmp_len/4); // U数据
				data += tmp_len/4;
				memcpy(data, pFrameYUV->data[2], tmp_len/4); // V数据
				len = tmp_len*3/2;
				av_free_packet(packet);
				return 0;
			}
			av_free_packet(packet);
			return 2;
		}
		av_free_packet(packet);
		return 1;
	}
	return -1;
}

unsigned short CFFmpeg::get_width()
{
	return pCodecCtx->width;
}

unsigned short CFFmpeg::get_height()
{
	return pCodecCtx->height;
}

unsigned int CFFmpeg::get_fps()
{
	unsigned int fps = pCodecCtx->framerate.num/pCodecCtx->framerate.den;
	fps += (pCodecCtx->framerate.num%pCodecCtx->framerate.den ? 1 : 0);

	return fps;
}

CFFmpeg::~CFFmpeg()
{
	sws_freeContext(img_convert_ctx);
	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
}
