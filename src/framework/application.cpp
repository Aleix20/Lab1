#include "application.h"
#include "utils.h"
#include "image.h"


Image img1;
Image img2;
Image canvas;
int var;
const int midaArray = 50;

struct Particle {
	int x;
	int y;
	int size;
};

struct Particle arr[midaArray];

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
	img1.loadTGA("../res/image.tga");
	img1.scale(window_width, window_height);
	
	img2.loadTGA("../res/toolbar.tga");
	
	
	//Task 5:
	for (int i = 0; i < midaArray; i++)
	{
		
		int x = rand() % (int)window_width;
		int y = rand() % (int)(window_height)+300;
		int size =  rand() % 3;
		
		arr[i].x = x;
		arr[i].y = y;
		arr[i].size = size;
	}
}

//render one frame
void Application::render( Image& framebuffer )
{
	//clear framebuffer if we want to start from scratch
	//framebuffer.fill(Color::BLACK);
	
	if (var == 1) {
		//Task 1: Simple figures
		framebuffer.fill(Color::BLACK);
		
		framebuffer.drawRectangle(330, 450, 50, 70, Color(255, 0, 0), true);
		framebuffer.drawRectangle(420, 450, 50, 70, Color(255, 0, 0), false);
		
		framebuffer.drawCircle(345, 375, 45, Color(0, 255, 0), true);
		framebuffer.drawCircle(455, 375, 45, Color(0, 255, 0), false);

		framebuffer.drawLine(400, 200, 400, 300, Color(0,0,255));
		framebuffer.drawLine(400, 200, 500, 300, Color(0, 0, 255));
		
		framebuffer.drawLine(400, 200, 500, 200, Color(0, 0, 255));
		framebuffer.drawLine(400, 200, 500, 100, Color(0, 0, 255));
		
		framebuffer.drawLine(400, 200, 400, 100, Color(0, 0, 255));
		framebuffer.drawLine(400, 200, 300, 100, Color(0, 0, 255));
		
		framebuffer.drawLine(400, 200, 300, 200, Color(0, 0, 255));
		framebuffer.drawLine(400, 200, 300, 300, Color(0, 0, 255));

	}

	//Task 2: Gradients
	if (var == 2) {
		framebuffer.fill(Color::BLACK);
		framebuffer.drawGradientH(Color::BLUE, Color::RED);
	}

	if (var == 3) {
		framebuffer.fill(Color::BLACK);
		framebuffer.drawRadialGradient();
	}
	
	//Task 3: Effects
	if (var == 4) {
		//filling the framebuffer with the image
		for (unsigned int x = 0; x < framebuffer.width; x++) {
			for (unsigned int y = 0; y < framebuffer.height; y++) {
				framebuffer.setPixel(x, y, img1.getPixelSafe(x, y));
			}
		}
	}
	
	if (var == 5) {
		//filling the framebuffer with the image
		for (unsigned int x = 0; x < framebuffer.width; x++) {
			for (unsigned int y = 0; y < framebuffer.height; y++) {
				framebuffer.setPixel(x, y, img1.getPixelSafe(x, y));
			}
		}
		framebuffer.greyImg(img1);
	}
	
	if (var == 6) {
		//filling the framebuffer with the image
		for (unsigned int x = 0; x < framebuffer.width; x++) {
			for (unsigned int y = 0; y < framebuffer.height; y++) {
				framebuffer.setPixel(x, y, img1.getPixelSafe(x, y));
			}
		}
		framebuffer.invertImg(img1);
	}
	
	//Task 4: Warping
	if (var == 7) {
		framebuffer.fill(Color::BLACK);
		framebuffer.rotateImg(img1, 45);
	}

	if (var == 8) {
		//filling the framebuffer with the image
		for (unsigned int x = 0; x < framebuffer.width; x++) {
			for (unsigned int y = 0; y < framebuffer.height; y++) {
				framebuffer.setPixel(x, y, img1.getPixelSafe(x, y));
			}
		}
		framebuffer.scaleImg(img1, 10.0);
	}
	if (var==9) {
		framebuffer.fill(Color::BLACK);

		for (int i = 0; i < midaArray; i++)
		{
			framebuffer.drawCircle(arr[i].x, arr[i].y, arr[i].size, Color::WHITE, true);

		}
	}

	if (var == 10) {
		//filling the framebuffer with the image
		for (unsigned int x = 0; x < framebuffer.width; x++) {
			for (unsigned int y = framebuffer.height-1; y > 0; y--) {
				framebuffer.setPixel(x, y, img2.getPixelSafe(x, y-framebuffer.height + img2.height));
			}
		}
	}
}

//called after render
void Application::update(double seconds_elapsed)
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (keystate[SDL_SCANCODE_SPACE]) //if key space is pressed
	{
		
		//...
	}

	//Task 5
	for (int i = 0; i < midaArray; i++)
	{
		
		arr[i].y = arr[i].y - 2*seconds_elapsed;
		arr[i].x = arr[i].x +1 * seconds_elapsed;

		if (arr[i].y <= 0) {
			arr[i].y = 700;
		}

	}

	
	//to read mouse position use mouse_position
}

//keyboard press event 
void Application::onKeyDown(SDL_KeyboardEvent event)
{
	//to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	switch (event.keysym.scancode)
	{
	case SDL_SCANCODE_ESCAPE:
		exit(0);
		break; //ESC key, kill the app	

	case SDL_SCANCODE_1:
		var = 1;
		break;

	case SDL_SCANCODE_2:
		var = 2;
		break;

	case SDL_SCANCODE_3:
		var = 3;
		break;

	case SDL_SCANCODE_4:
		var = 4;
		break;

	case SDL_SCANCODE_5:
		var = 5;
		break;

	case SDL_SCANCODE_6:
		var = 6;
		break;

	case SDL_SCANCODE_7:
		var = 7;
		break;

	case SDL_SCANCODE_8:
		var = 8;
		break;

	case SDL_SCANCODE_9:
		var = 9;
		break;
	
	case SDL_SCANCODE_0:
		var = 10;
		break;
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

