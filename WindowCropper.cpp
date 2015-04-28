#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include "ImgurAPI.h"
#include "WindowFunctions.h"
#include "WindowCropper.h"
#include "WindowUploader.h"
#include <vector>

// Declare display mode structure to be filled in.
SDL_DisplayMode current;
SDL_Window *windowCropper;
SDL_Renderer *rendererCropper;
SDL_Event event;
SDL_SysWMinfo SysInfo;
HWND hWnd;
bool quit = 0;
bool mousedown = false;

//track crop rectangle drawn by user
int startX = 0;
int startY = 0;
int endX = 0;
int endY = 0;
int screenHeight = 0;
int screenWidth = 0;


void initCropWindow(){
	SDL_Init(SDL_INIT_VIDEO);


	SDL_Rect r;
	if (SDL_GetDisplayBounds(0, &r) != 0) {
		SDL_Log("SDL_GetDisplayBounds failed: %s", SDL_GetError());		
	}

	screenHeight = r.h;
	screenWidth = r.w;

	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

	// Create an application windowCropper with the following settings:
	windowCropper = SDL_CreateWindow(
		"",                  // windowCropper title
		0,           // initial x position
		0,           // initial y position
		r.w,                               // width, in pixels
		r.h,                               // height, in pixels
		SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS            // flags - see below
		);


	SDL_VERSION(&SysInfo.version);

	if (SDL_GetWindowWMInfo(windowCropper, &SysInfo) <= 0) {
		printf("%s : %d\n", SDL_GetError(), SysInfo.info.win.window);
	}

	hWnd = SysInfo.info.win.window;
	// Handles the transparency of the windowCropper !!!
	// LWA_ALPHA works fine but LWA_COLORKEY doesn't work even the function returns 1.
	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255), 100, LWA_ALPHA);

	// Check that the windowCropper was successfully made
	if (windowCropper == NULL) {
		// In the event that the windowCropper could not be made...
		printf("Could not create windowCropper: %s\n", SDL_GetError());
	}

	//SDL_SetWindowFullscreen(windowCropper, SDL_WINDOW_FULLSCREEN_DESKTOP);

	// We must call SDL_CreateRenderer in order for draw calls to affect this windowCropper.
	rendererCropper = SDL_CreateRenderer(windowCropper, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawBlendMode(rendererCropper, SDL_BLENDMODE_BLEND);

	//// Select the color for drawing. It is set to red here.
	SDL_SetRenderDrawColor(rendererCropper, 255, 255, 255, 0);
	
	initCropEvents();
}

void initCropEvents(){
	/* Poll for events. SDL_PollEvent() returns 0 when there are no  */
	/* more events on the event queue, our while loop will exit when */
	/* that occurs.                                                 */

	while (!quit){
		while (SDL_PollEvent(&event)){
			/* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
			switch (event.type){
			case SDL_KEYDOWN:
				printf("Key press detected\n");
				break;

			case SDL_KEYUP:
				printf("Key release detected\n");
				break;

				/* SDL_QUIT event (windowCropper close) */
			case SDL_QUIT:
				cleanupCropWindow();
				quit = true;
				break;

			case SDL_MOUSEBUTTONDOWN:
				printf("mouse button pressed \n");
				mousedown = true;
				startX = event.button.x;
				startY = event.button.y;
				break;
			case SDL_MOUSEBUTTONUP:
				printf("mouse button released \n");
				printf("end coords: (%d, %d) \n", endX, endY);
				mousedown = false;
				saveScreenshotBMP(setCropRectCoords(startX, startY, endX, endY));
				break;
			case SDL_MOUSEMOTION:
				if (mousedown){
					//capture end points for rect draw
					endX = event.motion.x;
					endY = event.motion.y;

					printf("mouse button moved: (%d, %d) \n", endX, endY);

					printf("coords: (%d, %d), (%d,%d) \n", startX, startY, endX, endY);


					SDL_SetRenderDrawColor(rendererCropper, 255, 255, 255, 0);
					SDL_RenderClear(rendererCropper);



					//draw rect
					SDL_Rect rect = setCropRectCoords(startX, startY, endX, endY);

					// Render rect
					// Change color to blue!
					SDL_SetRenderDrawColor(rendererCropper, 0, 0, 255, 255);

					// Render our SDL_Rect
					// The rectangle will now be blue
					SDL_RenderDrawRect(rendererCropper, &rect);

					// Render the changes above
					SDL_RenderPresent(rendererCropper);

				}
				break;
			default:
				break;
			}
		}
	}
}

void saveScreenshotBMP(SDL_Rect cropArea) {
	cleanupCropWindow();
	//SDL_HideWindow(windowCropper);
	int x1, y1, x2, y2, w, h;

	// get screen dimensions
	x1 = GetSystemMetrics(SM_XVIRTUALSCREEN);
	y1 = GetSystemMetrics(SM_YVIRTUALSCREEN);
	x2 = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	y2 = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	w = x2 - x1;
	h = y2 - y1;

	// copy screen to bitmap
	HDC     hScreen = GetDC(NULL);
	HDC     hDC = CreateCompatibleDC(hScreen);
	// get screen dimensions
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, cropArea.w, cropArea.h);
	HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
	BOOL    bRet = BitBlt(hDC, 0, 0, cropArea.w, cropArea.h, hScreen, cropArea.x, cropArea.y, SRCCOPY);

	CreateBMPFile(hWnd, "screenshot.bmp", CreateBitmapInfoStruct(hWnd, hBitmap), hBitmap, hScreen);

	initWindowUploader(cropArea.w, cropArea.h);

	// clean up
	SelectObject(hDC, old_obj);
	DeleteDC(hDC);
	ReleaseDC(NULL, hScreen);
	DeleteObject(hBitmap);
}

SDL_Rect setCropRectCoords(int &startX, int &startY, int &endX, int &endY){
	//determine rectangle direction 
	SDL_Rect rect;
	int sx, sy, ex, ey = 0;
	if (endX >= startX && endY >= startY) {
		//went down + right
		sx = startX;
		sy = startY;
		ex = endX - startX;
		ey = endY - startY;
	}
	else if (endX <= startX && endY <= startY) {
		//went up + left
		sx = endX;
		sy = endY;
		ex = startX - endX;
		ey = startY - endY;
	}
	else if (endX >= startX && endY < startY) {
		//went up + right
		sx = startX;
		sy = endY;
		ex = endX - startX;
		ey = startY - endY;
	}
	else if (endX <= startX && endY >= startY) {
		// went down + left
		sx = endX;
		sy = startY;
		ex = startX - endX;
		ey = endY - startY;
	}

	rect.x = sx;
	rect.y = sy;
	rect.w = ex;
	rect.h = ey;

	return rect;
}

void cleanupCropWindow(){
	// Clean up
	// Close and destroy the windowCropper
	SDL_DestroyWindow(windowCropper);
	windowCropper = nullptr;

	SDL_DestroyRenderer(rendererCropper);
	rendererCropper = nullptr;
}



