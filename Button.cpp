//The headers
#include "SDL.h"
#include <string>


using namespace std;

//The event structure
SDL_Event event;


//The button
class Button
{
private:
	//The attributes of the button
	SDL_Rect box;
	string text;


public:
	//Initialize the variables
	Button(int x, int y, int w, int h, string buttonText);

	//Handles events and set the button's sprite region
	void handle_events();

	//Draws the button on the screen
	void draw(SDL_Window *window);
};

Button::Button(int x, int y, int w, int h, string buttonText)
{
	//Set the button's attributes
	box.x = x;
	box.y = y;
	box.w = w;
	box.h = h;
	text = buttonText;

}


void Button::handle_events()
{
	
}

void Button::draw(SDL_Window *window){
	//TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24); //this opens a font style and sets a size
	SDL_Color White = { 255, 255, 255 };  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color

}