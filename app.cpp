#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include "ImgurAPI.h"
#include "WindowFunctions.h"
#include "WindowCropper.h"
#include "WindowUploader.h"
#include <vector>

void renderCroppedImage(int width, int height){
}




int main(int argc, char* argv[])
{

	
	initCropWindow();
	initWindowUploader(cropCoords.w, cropCoords.h);
	SDL_Quit();

	return 0;


}