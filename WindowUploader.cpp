#include <iostream>
#include <stdio.h>
#include <SDL.h>

SDL_Window * windowUploader;
SDL_Renderer * rendererUploader;
SDL_Surface * image;
SDL_Surface * uploadButton;
SDL_Texture * imageTexture;
SDL_Texture * buttonTexture;

//yay magic numbers!
int ButtonHeight = 50;
int ButtonWidth = 100;


//need to add button for uploading
void initWindowUploader(int width, int height){
	windowUploader = SDL_CreateWindow("Image To Upload", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height + (ButtonHeight + 26), 0);

	rendererUploader = SDL_CreateRenderer(windowUploader, -1, 0);

	image = SDL_LoadBMP("screenshot.bmp");
	imageTexture = SDL_CreateTextureFromSurface(rendererUploader, image);

	uploadButton = SDL_LoadBMP("uploadbutton.bmp");
	buttonTexture = SDL_CreateTextureFromSurface(rendererUploader, uploadButton);



	SDL_SetRenderDrawColor(rendererUploader, 255, 255, 255, 0);
	SDL_RenderClear(rendererUploader);


	SDL_Rect imageRect = { 0 , 0, width, height };
	SDL_RenderCopy(rendererUploader, imageTexture, NULL, &imageRect);

	SDL_Rect buttonRect = { width - (ButtonWidth + 13), height + 13, ButtonWidth, ButtonHeight };
	SDL_RenderCopy(rendererUploader, buttonTexture, NULL, &buttonRect);

	SDL_RenderPresent(rendererUploader);
}

//setup key bindings


//clean up
void cleanUpWindowUploader(){
	SDL_DestroyTexture(imageTexture);
	imageTexture = nullptr;

	SDL_DestroyTexture(buttonTexture);
	buttonTexture = nullptr;

	SDL_FreeSurface(image);
	image = nullptr;

	SDL_FreeSurface(uploadButton);
	uploadButton = nullptr;

	SDL_DestroyRenderer(rendererUploader);
	rendererUploader = nullptr;

	SDL_DestroyWindow(windowUploader);
	windowUploader = nullptr;
}