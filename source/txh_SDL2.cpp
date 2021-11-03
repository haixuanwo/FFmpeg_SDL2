
#include "txh_SDL2.h"

#define REFRESH_EVENT  (SDL_USEREVENT + 1)
#define BREAK_EVENT  (SDL_USEREVENT + 2)

int CSDL2::thread_exit = 0;
int CSDL2::playDelay = 40; // 25fps=40ms

CSDL2::CSDL2()
{

}

int CSDL2::initSDL2(int weith, int height, PlayMode playMode)
{
	//SDL_DisplayMode DM;
	//SDL_GetCurrentDisplayMode(0, &DM);

	this->playMode = playMode;

	switch (playMode)
	{
		case ONE_SCREEN:
		{
			screen_w = weith;
			screen_h = height;
			pixel_w  = weith;
			pixel_h  = height;
			break;
		}
		case TWO_SCREEN:
		{
			break;
		}
		case FOUR_SCREEN:
		{
			screen_w = weith*2;
			screen_h = height*2;
			pixel_w  = weith*2;
			pixel_h  = height*2;

			sdlRect1.x = 0;
			sdlRect1.y = 0;
			sdlRect1.w = weith;
			sdlRect1.h = height;

			sdlRect2.x = weith;
			sdlRect2.y = 0;
			sdlRect2.w = weith;
			sdlRect2.h = height;

			sdlRect3.x = 0;
			sdlRect3.y = height;
			sdlRect3.w = weith;
			sdlRect3.h = height;

			sdlRect4.x = weith;
			sdlRect4.y = height;
			sdlRect4.w = weith;
			sdlRect4.h = height;
			break;
		}
		default:
		{
			printf("error play mode");
			return -1;
		}
	}

	sdlRectDst.x = 0;
	sdlRectDst.y = 0;
	sdlRectDst.w = screen_w;
	sdlRectDst.h = screen_h;

	printf("screen_w[%d] screen_h[%d] pixel_w[%d] pixel_h[%d]\n", screen_w, screen_h, pixel_w, pixel_h);

	if(SDL_Init(SDL_INIT_VIDEO))
	{
		printf( "Could not initialize SDL - %s\n", SDL_GetError());
		return -1;
	}

	//SDL 2.0 Support for multiple windows
	screen = SDL_CreateWindow("Clark play happy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		sdlRectDst.w, sdlRectDst.h,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	if(!screen) {
		printf("SDL: could not create window - exiting:%s\n",SDL_GetError());
		return -1;
	}

	sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
	Uint32 pixformat = SDL_PIXELFORMAT_IYUV;
	sdlTexture = SDL_CreateTexture(sdlRenderer, pixformat, SDL_TEXTUREACCESS_STREAMING, pixel_w, pixel_h);

	refresh_thread = SDL_CreateThread(refresh_video, nullptr, nullptr);
	return 0;
}

int CSDL2::playFrame(unsigned char *buf)
{
	while(1)
	{
		SDL_WaitEvent(&event);
		if(event.type==REFRESH_EVENT)
		{
			break;
		}
		else if(event.type == SDL_WINDOWEVENT)
		{
			if (SDL_WINDOWEVENT_CLOSE == event.window.event)
			{
				thread_exit=1;
				return -1;
			}

			SDL_GetWindowSize(screen, &screen_w, &screen_h);

			sdlRectDst.w = screen_w;
			sdlRectDst.h = screen_h;
		}
		else if(event.type==SDL_QUIT)
		{
			thread_exit=1;
			return -1;
		}
		else if(event.type==BREAK_EVENT)
		{
			return -1;
		}
	}

	if (FOUR_SCREEN == playMode)
	{
		SDL_UpdateTexture(sdlTexture, &sdlRect1, buf, pixel_w/2);
		SDL_UpdateTexture(sdlTexture, &sdlRect2, buf, pixel_w/2);
		SDL_UpdateTexture(sdlTexture, &sdlRect3, buf, pixel_w/2);
		SDL_UpdateTexture(sdlTexture, &sdlRect4, buf, pixel_w/2);
	}
	else if (ONE_SCREEN == playMode)
	{
		SDL_UpdateTexture( sdlTexture, nullptr, buf, pixel_w);
	}

	SDL_RenderClear(sdlRenderer);
	SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &sdlRectDst);
	SDL_RenderPresent(sdlRenderer);
	return 0;
}

int CSDL2::refresh_video(void *opaque)
{
	thread_exit=0;
	while (thread_exit==0) {
		SDL_Event event;
		event.type = REFRESH_EVENT;
		SDL_PushEvent(&event);
		SDL_Delay(playDelay);
	}

	thread_exit=0;
	SDL_Event event;
	event.type = BREAK_EVENT;
	SDL_PushEvent(&event);

	return 0;
}

CSDL2::~CSDL2()
{
	SDL_Quit();
}
