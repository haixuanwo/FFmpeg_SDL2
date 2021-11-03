/*
 * @Author: Clark
 * @Email: haixuanwoTxh@gmail.com
 * @Date: 2021-11-03 12:47:42
 * @LastEditors: Clark
 * @LastEditTime: 2021-11-03 18:49:23
 * @Description: file content
 */

#ifndef _TXH_SDL2_H_
#define _TXH_SDL2_H_

#include <stdio.h>

extern "C"
{
#include "SDL2/SDL.h"
};

typedef enum {
	ONE_SCREEN,
	TWO_SCREEN,
	FOUR_SCREEN,
}PlayMode;

class CSDL2
{
public:
	CSDL2();
	int initSDL2(int weith, int height, PlayMode playMode);
	int playFrame(unsigned char *buf);
	static int refresh_video(void *opaque);
	~CSDL2();

	int screen_w;
	int screen_h;
	int pixel_w;
	int pixel_h;

	static int playDelay;
private:
	SDL_Window *screen;
	SDL_Renderer *sdlRenderer;
	SDL_Texture* sdlTexture;
	SDL_Rect sdlRect1;
	SDL_Rect sdlRect2;
	SDL_Rect sdlRect3;
	SDL_Rect sdlRect4;
	SDL_Rect sdlRectDst;

	SDL_Thread *refresh_thread;
	SDL_Event event;
	static int thread_exit;
	PlayMode playMode;
};

#endif

