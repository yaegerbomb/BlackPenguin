#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <SDL.h>

using namespace std;
//The button
class Button
{
private:
	//The attributes of the button
	SDL_Rect box;

	//The part of the button sprite sheet that will be shown
	//SDL_Rect * clip;

public:
	//default constructor
	Button();
	~Button();

	//Initialize the variables
	Button(int x, int y, int w, int h, string fn);

	//Handles events and set the button's sprite region
	bool handle_events(SDL_Event event, SDL_Renderer *renderer);

	//Shows the button on the screen
	void show(SDL_Renderer *renderer);
	
	//clean up everything for quitting
	void clean_up();

	void set_clips();

	void apply_surface(int x, int y, SDL_Surface* buttonimage, SDL_Renderer* thewindow, SDL_Rect* clip = NULL, bool renderself = false);
};


#endif