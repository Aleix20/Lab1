#include "application.h"
#include "utils.h"
#include "image.h"


//Variables globals

Image img1;
Image img2;
Image canvas;
Color c;
int var;
const int midaArray = 50;
int xrect = 0;


//Creem la estructura partícula amb la coordenada x i y i el seu tamany
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
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

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

	//Carreguem imatges
	img1.loadTGA("../../res/image.tga");
	img1.scale(window_width, window_height);

	img2.loadTGA("../../res/toolbar.tga");

	//Task 6

	canvas.resize(800, 600);
	canvas.fill(Color::WHITE);

	//Task 5:
	for (int i = 0; i < midaArray; i++)
	{
		//Calculem valors aleatoris per cada particula
		int x = rand() % (int)window_width;
		int y = rand() % (int)window_height;
		int size = rand() % 3;

		//Assignem els valors a les estructures emmagatzemades a l'array
		arr[i].x = x;
		arr[i].y = y;
		arr[i].size = size;
	}
}

//render one frame
void Application::render(Image& framebuffer)
{

	if (var == 1) {
		//Task 1: Simple figures
		Task1(framebuffer);
	}
	//Task 2: Gradients
	if (var == 2) {
		Task2_1(framebuffer);
	}

	if (var == 3) {
		Task2_2(framebuffer);
	}
	//Task 3: Effects
	if (var == 4) {
		//filling the framebuffer with the image1 (fruita)
		Task3_Image(framebuffer);
	}
	if (var == 5) {
		Task3_Grey(framebuffer);
	}
	if (var == 6) {
		Task3_Negative(framebuffer);
	}

	//Task 4: Warping
	if (var == 7) {
		Task4_Rotate(framebuffer);
	}
	if (var == 8) {
		Task4_Scale(framebuffer);
	}

	//Task 5
	if (var == 9) {
		Task5_DrawParticles(framebuffer);
	}

	//Task 6
	if (var == 10) {
		Task6_Canvas(framebuffer);
	}
}

void Application::Task6_Canvas(Image& framebuffer)
{
	//Afegim al buffer la toolbar
	for (unsigned int x = 0; x < framebuffer.width; x++) {
		for (unsigned int y = framebuffer.height - 1; y > 0; y--) {
			framebuffer.setPixel(x, y, img2.getPixelSafe(x, y - framebuffer.height + img2.height));
		}
	}

	//En assegurem que un color estigui seleccionat i el marquem
	if (xrect > 0) {
		framebuffer.drawRectangle(xrect, 562, 26, 26, Color::WHITE, false);
	}

	//Actualitzem el canvas
	for (unsigned int x = 5; x < framebuffer.width - 5; x++) {
		for (unsigned int y = 5; y < framebuffer.height - 50; y++) {
			framebuffer.setPixel(x, y, canvas.getPixelSafe(x, y));
		}
	}
}

void Application::Task5_DrawParticles(Image& framebuffer)
{
	framebuffer.fill(Color::BLACK);

	for (int i = 0; i < midaArray; i++)
	{
		//Dibuixem les particules del array
		framebuffer.drawCircle(arr[i].x, arr[i].y, arr[i].size, Color::WHITE, true);
	}
}

void Application::Task4_Scale(Image& framebuffer)
{
	//filling the framebuffer with the image
	for (unsigned int x = 0; x < framebuffer.width; x++) {
		for (unsigned int y = 0; y < framebuffer.height; y++) {
			framebuffer.setPixel(x, y, img1.getPixelSafe(x, y));
		}
	}
	//Re-escalem la imatge
	framebuffer.scaleImg(img1, 10.0);
}

void Application::Task4_Rotate(Image& framebuffer)
{
	framebuffer.fill(Color::BLACK);
	//Girem la imatge 45 graus
	framebuffer.rotateImg(img1, 45);
}

void Application::Task3_Negative(Image& framebuffer)
{
	//filling the framebuffer with the image
	for (unsigned int x = 0; x < framebuffer.width; x++) {
		for (unsigned int y = 0; y < framebuffer.height; y++) {
			framebuffer.setPixel(x, y, img1.getPixelSafe(x, y));
		}
	}
	//Apliquem el filtre de negatius
	framebuffer.invertImg(img1);
}

void Application::Task3_Grey(Image& framebuffer)
{
	//filling the framebuffer with the image
	for (unsigned int x = 0; x < framebuffer.width; x++) {
		for (unsigned int y = 0; y < framebuffer.height; y++) {
			framebuffer.setPixel(x, y, img1.getPixelSafe(x, y));
		}
	}
	//Apliquem el filtre de grisos
	framebuffer.greyImg(img1);
}

void Application::Task3_Image(Image& framebuffer)
{
	for (unsigned int x = 0; x < framebuffer.width; x++) {
		for (unsigned int y = 0; y < framebuffer.height; y++) {
			framebuffer.setPixel(x, y, img1.getPixelSafe(x, y));
		}
	}
}

void Application::Task2_2(Image& framebuffer)
{
	framebuffer.fill(Color::BLACK);
	framebuffer.drawRadialGradient();
}

void Application::Task2_1(Image& framebuffer)
{
	framebuffer.fill(Color::BLACK);
	framebuffer.drawGradientH(Color::BLUE, Color::RED);
}

void Application::Task1(Image& framebuffer)
{
	framebuffer.fill(Color::BLACK);
	framebuffer.drawRectangle(330, 450, 50, 70, Color(255, 0, 0), true);
	framebuffer.drawRectangle(420, 450, 50, 70, Color(255, 0, 0), false);
	framebuffer.drawCircle(345, 375, 45, Color(0, 255, 0), true);
	framebuffer.drawCircle(455, 375, 45, Color(0, 255, 0), false);
	framebuffer.drawLine(400, 200, 400, 300, Color(0, 0, 255), false);
	framebuffer.drawLine(400, 200, 500, 300, Color(0, 0, 255), false);
	framebuffer.drawLine(400, 200, 500, 200, Color(0, 0, 255), false);
	framebuffer.drawLine(400, 200, 500, 100, Color(0, 0, 255), false);
	framebuffer.drawLine(400, 200, 400, 100, Color(0, 0, 255), false);
	framebuffer.drawLine(400, 200, 300, 100, Color(0, 0, 255), false);
	framebuffer.drawLine(400, 200, 300, 200, Color(0, 0, 255), false);
	framebuffer.drawLine(400, 200, 300, 300, Color(0, 0, 255), false);
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
		//Actualitzem els valors de les coordenades de cada partícula segons l'equació del moviment P' = P + V*t
		arr[i].x = arr[i].x + 1 * seconds_elapsed;
		arr[i].y = arr[i].y - 2 * seconds_elapsed;


		//Reset de cada particula quan arriba al final de la pantalla
		if (arr[i].y <= 0) {
			arr[i].y = 700;
		}
	}

	//Ens assegurem d'estar a la tasca 6
	if (var == 10) {

		//Si el ratoli esta presionat i dins del canvas
		if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) {

			if ((mouse_position.x >= 5 && mouse_position.x <= 795) && (mouse_position.y >= 5 && mouse_position.y <= 545)) {
				//Dibuixem una línia connectant els punts següents i anteriors del nostre recorregut
				canvas.drawLine(mouse_position.x, mouse_position.y, mouse_position.x + mouse_delta.x, mouse_position.y + mouse_delta.y, c, true);
			}
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

	//Programem els codis per executar cada tasca
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
void Application::onMouseButtonDown(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse pressed
	{
		//if you read mouse position from the event, careful, Y is reversed, use mouse_position instead

		//Posicio per resetejar canvas
		if ((mouse_position.x >= 10 && mouse_position.x <= 35) && (mouse_position.y >= 560 && mouse_position.y <= 590)) {
			canvas.fill(Color::WHITE);
		}

		//Posicio boto guardar imatge
		else if ((mouse_position.x >= 60 && mouse_position.x <= 90) && (mouse_position.y >= 560 && mouse_position.y <= 590)) {
			canvas.saveTGA("../../res/LaObraMagnífica.tga");
		}

		//A partir d'aqui, posicions de cada color
		else if ((mouse_position.x >= 111 && mouse_position.x <= 138) && (mouse_position.y >= 560 && mouse_position.y <= 590)) {
			c = Color::BLACK;
			xrect = 112;
		}

		else if ((mouse_position.x >= 163 && mouse_position.x <= 186) && (mouse_position.y >= 560 && mouse_position.y <= 590)) {
			c = Color::RED;
			xrect = 162;
		}

		else if ((mouse_position.x >= 214 && mouse_position.x <= 238) && (mouse_position.y >= 560 && mouse_position.y <= 590)) {
			c = Color::GREEN;
			xrect = 212;
		}

		else if ((mouse_position.x >= 264 && mouse_position.x <= 288) && (mouse_position.y >= 560 && mouse_position.y <= 590)) {
			c = Color::BLUE;
			xrect = 262;
		}

		else if ((mouse_position.x >= 314 && mouse_position.x <= 338) && (mouse_position.y >= 560 && mouse_position.y <= 590)) {
			c = Color::YELLOW;
			xrect = 312;
		}

		else if ((mouse_position.x >= 364 && mouse_position.x <= 388) && (mouse_position.y >= 560 && mouse_position.y <= 590)) {
			c = Color::PURPLE;
			xrect = 362;
		}

		else if ((mouse_position.x >= 414 && mouse_position.x <= 438) && (mouse_position.y >= 560 && mouse_position.y <= 590)) {
			c = Color::CYAN;
			xrect = 412;
		}

		else if ((mouse_position.x >= 464 && mouse_position.x <= 488) && (mouse_position.y >= 560 && mouse_position.y <= 590)) {
			c = Color::WHITE;
			xrect = 462;
		}
	}
}

void Application::onMouseButtonUp(SDL_MouseButtonEvent event)
{
	if (event.button == SDL_BUTTON_LEFT)
	{

	}
}

//when the app starts
void Application::start()
{
	std::cout << "launching loop..." << std::endl;
	launchLoop(this);
}


