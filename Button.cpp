#include <iostream>
#include <SDL.h>
#include "Button.h"

using namespace std;

//built with help of lazy foo tutorial

//Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 130;
const int TOTAL_BUTTONS = 3;

//The surfaces
SDL_Surface *buttonSheet = NULL;
SDL_Texture * croppedImageTexture = NULL; 
SDL_Rect * clip;


//The clip regions of the sprite sheet
SDL_Rect clips[TOTAL_BUTTONS];

//The button states in the sprite sheet
const int CLIP_MOUSEOVER = 0;
const int CLIP_MOUSEOUT = 1;
const int CLIP_MOUSEDOWN = 2;
const int CLIP_MOUSEUP = 3;

//filename
string filename;



void Button::apply_surface(int x, int y, SDL_Surface* buttonimage, SDL_Renderer* thewindow, SDL_Rect* clip, bool renderself)
{
	//Holds offsets
	SDL_Rect offset;

	//Get offsets
	offset.x = x;
	offset.y = y;

	croppedImageTexture = SDL_CreateTextureFromSurface(thewindow, buttonimage);

	if (clip != nullptr){
		offset.w = clip->w;
		offset.h = clip->h;
	}
	else {
		SDL_QueryTexture(croppedImageTexture, NULL, NULL, &offset.w, &offset.h);
	}

	SDL_RenderCopy(thewindow, croppedImageTexture, clip, &offset);
	if (renderself){
		SDL_RenderPresent(thewindow);
	}
}


void Button::clean_up()
{
	SDL_FreeSurface(buttonSheet);
	buttonSheet = nullptr;


	SDL_DestroyTexture(croppedImageTexture);
	croppedImageTexture = nullptr;

}

void Button::set_clips()
{
	//clips in order: mousedown -> mouseover -> ntrl

	//Clip the sprite sheet
	clips[CLIP_MOUSEDOWN] = { 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT };
	clips[CLIP_MOUSEOVER] = { 0, BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT };
	clips[CLIP_MOUSEOUT] = { 0, (2 * BUTTON_HEIGHT), BUTTON_WIDTH, BUTTON_HEIGHT };
	clips[CLIP_MOUSEUP] = { 0, BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT };
}

Button::Button(){}

Button::Button(int x, int y, int w, int h, string fn)
{
	//Set the button's attributes
	box.x = x;
	box.y = y;
	box.w = w;
	box.h = h;

	filename = fn;
	buttonSheet = SDL_LoadBMP(filename.c_str());

	//Set the default sprite
	//set_clips();
	clip = new SDL_Rect({ 0, (2 * BUTTON_HEIGHT), BUTTON_WIDTH, BUTTON_HEIGHT });
}

Button::~Button(){
	
};

bool Button::handle_events(SDL_Event event, SDL_Renderer *renderer)
{
	//The mouse offsets
	int x = 0, y = 0;

	//If the mouse moved
	if (event.type == SDL_MOUSEMOTION)
	{
		//Get the mouse offsets
		x = event.motion.x;
		y = event.motion.y;

		//If the mouse is over the button
		if ((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h))
		{
			//Set the button sprite
			clip = new SDL_Rect({ 0, BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT });
			apply_surface(box.x, box.y, buttonSheet, renderer, clip, true);
		}
		//If not
		else
		{
			//Set the button sprite
			clip = new SDL_Rect({ 0, (2 * BUTTON_HEIGHT), BUTTON_WIDTH, BUTTON_HEIGHT });
			apply_surface(box.x, box.y, buttonSheet, renderer, clip, true);
		}
	}
	//If a mouse button was pressed
	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		//If the left mouse button was pressed
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			//Get the mouse offsets
			x = event.button.x;
			y = event.button.y;

			//If the mouse is over the button
			if ((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h))
			{
				//Set the button sprite
				clip = new SDL_Rect({ 0, 0, BUTTON_WIDTH, BUTTON_HEIGHT });;
				apply_surface(box.x, box.y, buttonSheet, renderer, clip, true);
			}
		}
	}
	//If a mouse button was released
	if (event.type == SDL_MOUSEBUTTONUP)
	{
		//If the left mouse button was released
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			//Get the mouse offsets
			x = event.button.x;
			y = event.button.y;

			//If the mouse is over the button
			if ((x > box.x) && (x < box.x + box.w) && (y > box.y) && (y < box.y + box.h))
			{
				//Set the button sprite
				clip = new SDL_Rect({ 0, BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT });
				apply_surface(box.x, box.y, buttonSheet, renderer, clip, true);
				
				//button was pressed
				return true;
			}
		}
	}

	//button was not pressed
	return false;
} 

void Button::show(SDL_Renderer *renderer)
{
	//Show the button
	apply_surface(box.x, box.y, buttonSheet, renderer, clip);
}