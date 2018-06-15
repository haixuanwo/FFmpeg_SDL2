
#include "txh_FFmpeg.h"
#include "txh_SDL2.h"

int main()
{
	int ret = -1;
	unsigned char *data = (unsigned char *)malloc(1024*500);
	int len = -1;

	char fileName[] = {"Titanic.ts"};
	
	CFFmpeg ffmpeg(fileName);
	CSDL2 sdl;
	sdl.initSDL2(640, 272);

	long long pts;

	FILE *fp = fopen("txh_out.yuv","wb");
	if(NULL == fp)
	{
		return -1;
	}
	
	while(1)
	{
		ret = ffmpeg.FFmpeg_read_frame(data, len);
		if(0 == ret)
		{
			sdl.playFrame(data);
		}
		else if(-1 == ret)
		{
			break;
		}				
	}

	return 0;
}

