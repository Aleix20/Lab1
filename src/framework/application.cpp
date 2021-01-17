#include "application.h"
#include "utils.h"
#include "image.h"

Image img;

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(NULL);

	framebuffer.resize(w, h);
}

//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	std::cout << "initiating app..." << std::endl;

	//here add your init stuff
	
	img.loadTGA("../res/image.tga");
	img.scale(window_width, window_height);
}

//render one frame
void Application::render( Image& framebuffer )
{
	//clear framebuffer if we want to start from scratch
	framebuffer.fill(Color::BLACK);
	
	//filling the framebuffer with the image
	/*for (unsigned int x = 0; x < framebuffer.width; x++) {
		for (unsigned int y = 0; y < framebuffer.height; y++) {
			framebuffer.setPixel(x, y, img.getPixelSafe(x, y));
		}
	}*/

	//Task 1: Simple figures
	//framebuffer.drawRectangle(50, 50, 50, 70, Color(20, 20, 20), true);
	//framebuffer.drawCircle(400, 300, 300, Color(255, 0, 0), false);
	framebuffer.drawLine(400, 200, 400, 100, Color::BLUE);
	
	//Task 2: Gradients
	//framebuffer.drawGradientH(Color::BLUE, Color::RED);
	//framebuffer.drawRadialGradient(Color::BLACK, Color::WHITE);

	//Task 3: Effects
	//framebuffer.greyImg(img);
	//framebuffer.invertImg(img);
	
	//Task 4: Warping
	//framebuffer.rotateImg(img, 45);
	//framebuffer.scaleImg(img,10.0);

	
}

//called after render
void Application::update(double seconds_elapsed)
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (keystate[SDL_SCANCODE_SPACE]) //if key space is pressed
	{
		//...
	}

	//to read mouse position use mouse_position
}

//keyboard press event 
void Application::onKeyDown( SDL_KeyboardEvent event )
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	switch(event.keysym.scancode)
	{
		case SDL_SCANCODE_ESCAPE:
			exit(0); 
			break; //ESC key, kill the app
	}
}

//keyboard key up event 
void Application::onKeyUp(SDL_KeyboardEvent event)
{
	//...
}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse pressed
	{
		//if you read mouse position from the event, careful, Y is reversed, use mouse_position instead
	}
}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse unpressed
	{

	}
}

//when the app starts
void Application::start()
{
	std::cout << "launching loop..." << std::endl;
	launchLoop(this);
}

