
#include "txh_FFmpeg.h"
#include "txh_SDL2.h"

#define MAX_BUF_SIZE 10*1024*1024

int main(int argc, char *argv[])
{
	int ret = -1;
	unsigned char *data = (unsigned char *)malloc(MAX_BUF_SIZE);
	int len = -1;

	if(argc < 2)
	{
		printf("please input video file and play mode\n");
		return -1;
	}

	PlayMode playMode = ONE_SCREEN;
	if (argc > 2)
	{
		switch (atoi(argv[2]))
		{
			case 0:
				playMode = ONE_SCREEN;
				break;
			case 1:
				playMode = TWO_SCREEN;
				break;
			case 2:
				playMode = FOUR_SCREEN;
				break;
			default:
				printf("please input correct mode\n");
				return -1;
		}
	}

	std::string fileName = argv[1];
	CFFmpeg ffmpeg(fileName);

	CSDL2 sdl;
	sdl.initSDL2(ffmpeg.get_width(), ffmpeg.get_height(), playMode, ffmpeg.get_fps());

	while(1)
	{
		ret = ffmpeg.FFmpeg_read_frame(data, len);
		if (0 == ret)
		{
			if (sdl.playFrame(data) < 0)
			{
				break;
			}
		}
		else if (ret < 0)
		{
			break;
		}
	}

	delete data;
	return 0;
}
