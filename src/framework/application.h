/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#ifndef APPLICATION_H
#define APPLICATION_H

#include "includes.h"
#include "framework.h"
#include "image.h"

class Application
{
public:
	//window
	SDL_Window* window;
	float window_width;
	float window_height;

	Image framebuffer;

	float time;

	//keyboard state
	const Uint8* keystate;

	//mouse state
	int mouse_state; //tells which buttons are pressed
	Vector2 mouse_position; //last mouse position
	Vector2 mouse_delta; //mouse movement in the last frame

	//constructor
	Application(const char* caption, int width, int height);

	//main methods
	void init( void );
	void render( Image& framebuffer );
	void Task6_Canvas(Image& framebuffer);
	void Task5_DrawParticles(Image& framebuffer);
	void Task4_Scale(Image& framebuffer);
	void Task4_Rotate(Image& framebuffer);
	void Task3_Negative(Image& framebuffer);
	void Task3_Grey(Image& framebuffer);
	void Task3_Image(Image& framebuffer);
	void Task2_2(Image& framebuffer);
	void Task2_1(Image& framebuffer);
	void Task1(Image& framebuffer);
	void update( double dt );

	//methods for events
	void onKeyDown( SDL_KeyboardEvent event );
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown( SDL_MouseButtonEvent event );
	void onMouseButtonUp( SDL_MouseButtonEvent event );

	//other methods to control the app
	void setWindowSize(int width, int height) {
		glViewport( 0,0, width, height );
		this->window_width = width;
		this->window_height = height;
		framebuffer.resize(width,height);
	}

	Vector2 getWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(w,h);
	}

	void start();
};


#endif 