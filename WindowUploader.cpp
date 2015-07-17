#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include "Button.h"
#include "ImgurAPI.h"
#include "WindowFunctions.h"

SDL_Window * windowUploader;
SDL_Renderer * rendererUploader;
SDL_Surface * image;
//SDL_Surface * uploadButton;
SDL_Texture * imageTexture;
//SDL_Texture * buttonTexture; 

Button uploadButton;

SDL_Event uploadEvent;
bool uploaderRunning = false;;

//yay magic numbers!
int ButtonHeight = 130;
int ButtonWidth = 300;

//clean up
void cleanUpWindowUploader(){
	SDL_DestroyTexture(imageTexture);
	imageTexture = nullptr;

	//SDL_DestroyTexture(buttonTexture);
	//buttonTexture = nullptr;


	//SDL_FreeSurface(uploadButton);
	//uploadButton = nullptr;

	SDL_FreeSurface(image);
	image = nullptr;


	SDL_DestroyRenderer(rendererUploader);
	rendererUploader = nullptr;

	SDL_DestroyWindow(windowUploader);
	windowUploader = nullptr;

}


//setup key bindings
void initKeyBindings(){
	while (uploaderRunning){
		while (SDL_PollEvent(&uploadEvent) && uploaderRunning){
			bool uploadPressed = uploadButton.handle_events(uploadEvent, rendererUploader);
			if (uploadPressed){
				//code to upload to imgur
				string linkUrl;
				UploadImgurImage(&linkUrl);
				AddStringToClipboard(linkUrl);
			}

			/* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
			switch (uploadEvent.type){
			case SDL_KEYDOWN:
				printf("Key press detected\n");
				break;

			case SDL_KEYUP:
				printf("Key release detected\n");
				break;
			case SDL_QUIT:
				uploadButton.clean_up();
				cleanUpWindowUploader();
				uploaderRunning = false;
				break;

			case SDL_MOUSEBUTTONDOWN:
				printf("mouse button pressed \n");
				break;
			case SDL_MOUSEBUTTONUP:
				break;
			case SDL_MOUSEMOTION:
				break;
			default:
				break;
			}
		}
	}
}


//need to add button for uploading
void initWindowUploader(int width, int height){
	windowUploader = SDL_CreateWindow("Image To Upload", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height + (ButtonHeight + 26), 0);

	rendererUploader = SDL_CreateRenderer(windowUploader, -1, 0);

	string filepath = ExePath() + '\\' + "screenshot.bmp";
	image = SDL_LoadBMP(filepath.c_str());
	imageTexture = SDL_CreateTextureFromSurface(rendererUploader, image);

	//uploadButton = SDL_LoadBMP("uploadbutton.bmp");
	//buttonTexture = SDL_CreateTextureFromSurface(rendererUploader, uploadButton);



	SDL_SetRenderDrawColor(rendererUploader, 255, 255, 255, 0);
	SDL_RenderClear(rendererUploader);


	SDL_Rect imageRect = { 0 , 0, width, height };
	SDL_RenderCopy(rendererUploader, imageTexture, NULL, &imageRect);

	//SDL_Rect buttonRect = { width - (ButtonWidth + 13), height + 13, ButtonWidth, ButtonHeight };
	//SDL_RenderCopy(rendererUploader, buttonTexture, NULL, &buttonRect);

	//add buttons
	uploadButton = Button(width - (ButtonWidth + 13), height + 13, ButtonWidth, ButtonHeight, "UploadButtonSprite.bmp");
	uploadButton.show(rendererUploader);


	SDL_RenderPresent(rendererUploader);
	uploaderRunning = true;
	initKeyBindings();
}



