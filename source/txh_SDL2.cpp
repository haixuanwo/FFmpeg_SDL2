
#include "txh_SDL2.h"

#define REFRESH_EVENT  (SDL_USEREVENT + 1)
#define BREAK_EVENT  (SDL_USEREVENT + 2)

int CSDL2::thread_exit = 0;
int CSDL2::playDelay = 40; // 帧率25，播放频率1000/25=40ms

CSDL2::CSDL2()
{
	buf_c = (unsigned char*)malloc(1024*500);
}

int CSDL2::initSDL2(int weith, int height)
{
	screen_w = weith*2;
	screen_h = height*2;
	pixel_w  = weith*2;
	pixel_h  = height*2;

	// 主屏大小
	sdlRectDst.x = 0;  
	sdlRectDst.y = 0;  
	sdlRectDst.w = screen_w;  
	sdlRectDst.h = screen_h;

	// 第一个视频
	sdlRect1.x = 0;  
	sdlRect1.y = 0;  
	sdlRect1.w = weith;  
	sdlRect1.h = height; 

	// 第二个视频
	sdlRect2.x = weith;  
	sdlRect2.y = 0;  
	sdlRect2.w = weith;  
	sdlRect2.h = height;

	// 第三个视频
	sdlRect3.x = 0;  
	sdlRect3.y = height;  
	sdlRect3.w = weith;  
	sdlRect3.h = height; 

	// 第四个视频
	sdlRect4.x = weith;  
	sdlRect4.y = height;  
	sdlRect4.w = weith;  
	sdlRect4.h = height;	

	if(SDL_Init(SDL_INIT_VIDEO))
	{  
		printf( "Could not initialize SDL - %s\n", SDL_GetError()); 
		return -1;
	}
	
	//SDL 2.0 Support for multiple windows
	screen = SDL_CreateWindow("Simplest Video Play SDL2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		sdlRectDst.w, sdlRectDst.h,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	if(!screen) {  
		printf("SDL: could not create window - exiting:%s\n",SDL_GetError());  
		return -1;
	}
	
	sdlRenderer = SDL_CreateRenderer(screen, -1, 0);

	Uint32 pixformat=0;
	pixformat= SDL_PIXELFORMAT_IYUV;
	sdlTexture = SDL_CreateTexture(sdlRenderer,pixformat, SDL_TEXTUREACCESS_STREAMING,pixel_w,pixel_h);

	refresh_thread = SDL_CreateThread(refresh_video,NULL,NULL);
	
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
		else if(event.type==SDL_WINDOWEVENT)
		{
			// 获得调试窗口后的尺寸
			SDL_GetWindowSize(screen,&screen_w,&screen_h);

			// 主屏
			sdlRectDst.x = 0;  
			sdlRectDst.y = 0;  
			sdlRectDst.w = screen_w;  
			sdlRectDst.h = screen_h;

			// 第一个视频
			sdlRect1.x = 0;  
			sdlRect1.y = 0;  
			sdlRect1.w = screen_w/2;  
			sdlRect1.h = screen_h/2; 

			// 第二个视频
			sdlRect2.x = screen_w/2;  
			sdlRect2.y = 0;  
			sdlRect2.w = screen_w/2;  
			sdlRect2.h = screen_h/2;

			// 第三个视频
			sdlRect3.x = 0;  
			sdlRect3.y = screen_h/2;  
			sdlRect3.w = screen_w/2;  
			sdlRect3.h = screen_h/2; 

			// 第四个视频
			sdlRect4.x = screen_w/2;  
			sdlRect4.y = screen_h/2;  
			sdlRect4.w = screen_w/2;  
			sdlRect4.h = screen_h/2;
			
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
	

	// 将视频数据放在四个不同的位置
	SDL_UpdateTexture( sdlTexture, &sdlRect1, buf, pixel_w/2);
	SDL_UpdateTexture( sdlTexture, &sdlRect2, buf, pixel_w/2);	
	SDL_UpdateTexture( sdlTexture, &sdlRect3, buf, pixel_w/2);
	SDL_UpdateTexture( sdlTexture, &sdlRect4, buf, pixel_w/2);

	// 将要显示的画面copy到渲染器
	SDL_RenderClear( sdlRenderer );    
	SDL_RenderCopy( sdlRenderer, sdlTexture, NULL, &sdlRectDst);

	// 显示画面
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

