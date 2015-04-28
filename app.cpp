#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <SDL_syswm.h>
#include "ImgurAPI.h"
#include "WindowFunctions.h"
#include "WindowCropper.h"
#include <vector>

void renderCroppedImage(int width, int height){
}




int main(int argc, char* argv[])
{

	
	initCropWindow();
	cleanupCropWindow();
	return 0;

	//code to upload to imgur
	/*string linkUrl; 
	UploadImgurImage(&linkUrl);
	AddStringToClipboard(linkUrl);*/
}