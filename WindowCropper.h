#ifndef WINDOWCROPPER_H
#define WINDOWCROPPER_H

#include <SDL.h>


SDL_Rect setCropRectCoords(int &startX, int &startY, int &endX, int &endY);

void initCropWindow();

void initCropEvents();

void saveScreenshotBMP(SDL_Rect cropArea);

void cleanupCropWindow();

#endif