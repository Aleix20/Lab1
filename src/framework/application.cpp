#include "application.h"
#include "utils.h"
#include "image.h"

Image img1; //Declarem les variables globals que utilitzarem més endavant
Image img2;
Image canvas;
Color c;
int var;
const int midaArray = 50;
int xrect = 0;

struct Particle { //Creem la estructura partícula amb la coordenada x i y i el seu tamany
	int x;
	int y;
	int size;
};

struct Particle arr[midaArray]; //Creem un array d´estructures partícula

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
	img1.loadTGA("../../res/image.tga");       //Carreguem les imatges que necessitem i les escalem
	img1.scale(window_width, window_height);   
	
	img2.loadTGA("../../res/toolbar.tga");
	
	canvas.resize(800, 600);
	canvas.fill(Color::WHITE); //Pintem el canvas de blanc en un inici
	
	//Task 5:
	for (int i = 0; i < midaArray; i++)
	{
		int x = rand() % (int)window_width; //Calculem una coordenada x aleatoria per la partícula
		int y = rand() % (int)window_height; //Calculem una coordenada y aleatoria per la partícula
		int size =  rand() % 3; //Calculem un valor aleatori entre 0 i 3 per al tamany
		
		arr[i].x = x; //Assignem els valors a les estructures emmagatzemades a l'array
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
		
		//Apliquem les funcions de la tasca 1
		
		framebuffer.drawRectangle(330, 450, 50, 70, Color(255, 0, 0), true);
		framebuffer.drawRectangle(420, 450, 50, 70, Color(255, 0, 0), false);
		
		framebuffer.drawCircle(345, 375, 45, Color(0, 255, 0), true);
		framebuffer.drawCircle(455, 375, 45, Color(0, 255, 0), false);

		framebuffer.drawLine(400, 200, 400, 300, Color(0,0,255), false);
		framebuffer.drawLine(400, 200, 500, 300, Color(0, 0, 255), false);
		
		framebuffer.drawLine(400, 200, 500, 200, Color(0, 0, 255), false);
		framebuffer.drawLine(400, 200, 500, 100, Color(0, 0, 255), false);
		
		framebuffer.drawLine(400, 200, 400, 100, Color(0, 0, 255), false);
		framebuffer.drawLine(400, 200, 300, 100, Color(0, 0, 255), false);
		
		framebuffer.drawLine(400, 200, 300, 200, Color(0, 0, 255), false);
		framebuffer.drawLine(400, 200, 300, 300, Color(0, 0, 255), false);
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
		//filling the framebuffer with the image1 (fruita)
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
		framebuffer.greyImg(img1); //Apliquem el filtre de grisos
	}
	
	if (var == 6) {
		//filling the framebuffer with the image
		for (unsigned int x = 0; x < framebuffer.width; x++) {
			for (unsigned int y = 0; y < framebuffer.height; y++) {
				framebuffer.setPixel(x, y, img1.getPixelSafe(x, y));
			}
		}
		framebuffer.invertImg(img1); //Apliquem el filtre de negatius
	}
	
	//Task 4: Warping
	if (var == 7) {
		framebuffer.fill(Color::BLACK);
		framebuffer.rotateImg(img1, 45); //Girem la imatge 45 graus
	}

	if (var == 8) {
		//filling the framebuffer with the image
		for (unsigned int x = 0; x < framebuffer.width; x++) {
			for (unsigned int y = 0; y < framebuffer.height; y++) {
				framebuffer.setPixel(x, y, img1.getPixelSafe(x, y));
			}
		}
		framebuffer.scaleImg(img1, 10.0); //Re-escalem la imatge
	}
	if (var==9) {
		framebuffer.fill(Color::BLACK);

		for (int i = 0; i < midaArray; i++)
		{
			framebuffer.drawCircle(arr[i].x, arr[i].y, arr[i].size, Color::WHITE, true); //Dibuixem cada floquet accedint a cada posició de l'array de partícules
		}
	}

	if (var == 10) {
		//filling the framebuffer with the image2 (Entorn per pintar)
		for (unsigned int x = 0; x < framebuffer.width; x++) {
			for (unsigned int y = framebuffer.height-1; y > 0; y--) {
				framebuffer.setPixel(x, y, img2.getPixelSafe(x, y-framebuffer.height + img2.height));
			}
		}
		
		if (xrect > 0) { //En el cas de que la coordenada x del botó sigui major que 0 li dibuixem un rectangle blanc que indiqui que està pressionat
			framebuffer.drawRectangle(xrect, 562, 26, 26, Color::WHITE, false);
		}

		//filling the framebuffer with the canvas
		for (unsigned int x = 5; x < framebuffer.width - 5; x++) {
			for (unsigned int y = 5; y < framebuffer.height - 50; y++) {
				framebuffer.setPixel(x, y, canvas.getPixelSafe(x, y));
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
		arr[i].x = arr[i].x + 1*seconds_elapsed; //Actualitzem els valors de les coordenades de cada partícula segons l'equació del moviment P' = P + V*t
		arr[i].y = arr[i].y - 2*seconds_elapsed; 		

		if (arr[i].y <= 0) { //Si el floquet ja ha sortit del framebuffer el fem sortir per dalt canviant el valor de la seva coordenada y per el de dalt de tot
			arr[i].y = 700;
		}
	}
	
	if (var == 10) { //Si estem a la tasca de l'entorn gràfic
		
		if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) { //Si pressionem el botó del ratolí

			if ((mouse_position.x >= 5 && mouse_position.x <= 795) && (mouse_position.y >= 5 && mouse_position.y <= 545)) { //Si el ratolí esta dins el canvas
				canvas.drawLine(mouse_position.x, mouse_position.y, mouse_position.x + mouse_delta.x, mouse_position.y + mouse_delta.y, c, true); //Dibuixem una línia connectant els punts següents i anteriors del nostre recorregut
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

	case SDL_SCANCODE_1: //Programem els codis per executar cada tasca
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
void Application::onMouseButtonDown( SDL_MouseButtonEvent event ) //Funció que ens permet programar events del ratolí
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse pressed
	{
		//if you read mouse position from the event, careful, Y is reversed, use mouse_position instead
		if ((mouse_position.x >= 10 && mouse_position.x <= 35) && (mouse_position.y >= 560 && mouse_position.y <= 590)){  //Programem que farà el programa segons el botó que hem clicat de l'entorn per pintar
			canvas.fill(Color::WHITE); //Si fem clic a l'icona de nova fulla, netegem el canvas i comencem de nou
		}

		else if ((mouse_position.x >= 60 && mouse_position.x <= 90) && (mouse_position.y >= 560 && mouse_position.y <= 590)) { //Si fem clic al boto de guardat guardarem la imatge
			canvas.saveTGA("../../res/LaObraMagnífica.tga");
		}

		else if ((mouse_position.x >= 111 && mouse_position.x <= 138) && (mouse_position.y >= 560 && mouse_position.y <= 590)) { //En els altres casos canviem el color segons el del botó i ens guardem la coordenada x d'aquest per dibuixar el rectangle al seu voltant
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

