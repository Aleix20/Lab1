#include "image.h"




Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width * height];
	memset(pixels, 0, width * height * sizeof(Color));
}

//copy constructor
Image::Image(const Image& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new Color[width * height];
		memcpy(pixels, c.pixels, width * height * sizeof(Color));
	}
}

//assign operator
Image& Image::operator = (const Image& c)
{
	if (pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if (c.pixels)
	{
		pixels = new Color[width * height * sizeof(Color)];
		memcpy(pixels, c.pixels, width * height * sizeof(Color));
	}
	return *this;
}

Image::~Image()
{
	if (pixels)
		delete pixels;
}



//change image size (the old one will remain in the top-left corner)
void Image::resize(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width * height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for (unsigned int x = 0; x < min_width; ++x)
		for (unsigned int y = 0; y < min_height; ++y)
			new_pixels[y * width + x] = getPixel(x, y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

//change image size and scale the content
void Image::scale(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width * height];

	for (unsigned int x = 0; x < width; ++x)
		for (unsigned int y = 0; y < height; ++y)
			new_pixels[y * width + x] = getPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)));

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::getArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for (unsigned int x = 0; x < width; ++x)
		for (unsigned int y = 0; y < height; ++y)
		{
			if ((x + start_x) < this->width && (y + start_y) < this->height)
				result.setPixel(x, y, getPixel(x + start_x, y + start_y));
		}
	return result;
}

void Image::flipX()
{
	for (unsigned int x = 0; x < width * 0.5; ++x)
		for (unsigned int y = 0; y < height; ++y)
		{
			Color temp = getPixel(width - x - 1, y);
			setPixel(width - x - 1, y, getPixel(x, y));
			setPixel(x, y, temp);
		}
}

void Image::flipY()
{
	for (unsigned int x = 0; x < width; ++x)
		for (unsigned int y = 0; y < height * 0.5; ++y)
		{
			Color temp = getPixel(x, height - y - 1);
			setPixel(x, height - y - 1, getPixel(x, y));
			setPixel(x, y, temp);
		}
}


//Loads an image from a TGA file
bool Image::loadTGA(const char* filename)
{
	unsigned char TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int bytesPerPixel;
	unsigned int imageSize;

	FILE* file = fopen(filename, "rb");
	if (file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
		memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
		fread(header, 1, sizeof(header), file) != sizeof(header))
	{
		std::cerr << "File not found: " << filename << std::endl;
		if (file == NULL)
			return NULL;
		else
		{
			fclose(file);
			return NULL;
		}
	}

	TGAInfo* tgainfo = new TGAInfo;

	tgainfo->width = header[1] * 256 + header[0];
	tgainfo->height = header[3] * 256 + header[2];

	if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
	{
		std::cerr << "TGA file seems to have errors or it is compressed, only uncompressed TGAs supported" << std::endl;
		fclose(file);
		delete tgainfo;
		return NULL;
	}

	tgainfo->bpp = header[4];
	bytesPerPixel = tgainfo->bpp / 8;
	imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;

	tgainfo->data = new unsigned char[imageSize];

	if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
	{
		if (tgainfo->data != NULL)
			delete tgainfo->data;

		fclose(file);
		delete tgainfo;
		return false;
	}

	fclose(file);

	//save info in image
	if (pixels)
		delete pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width * height];

	//convert to float all pixels
	for (unsigned int y = 0; y < height; ++y)
		for (unsigned int x = 0; x < width; ++x)
		{
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			this->setPixel(x, height - y - 1, Color(tgainfo->data[pos + 2], tgainfo->data[pos + 1], tgainfo->data[pos]));
		}

	delete tgainfo->data;
	delete tgainfo;

	return true;
}

// Saves the image to a TGA file
bool Image::saveTGA(const char* filename)
{
	unsigned char TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	FILE* file = fopen(filename, "wb");
	if (file == NULL)
	{
		fclose(file);
		return false;
	}

	unsigned short header_short[3];
	header_short[0] = width;
	header_short[1] = height;
	unsigned char* header = (unsigned char*)header_short;
	header[4] = 24;
	header[5] = 0;

	//tgainfo->width = header[1] * 256 + header[0];
	//tgainfo->height = header[3] * 256 + header[2];

	fwrite(TGAheader, 1, sizeof(TGAheader), file);
	fwrite(header, 1, 6, file);

	//convert pixels to unsigned char
	unsigned char* bytes = new unsigned char[width * height * 3];
	for (unsigned int y = 0; y < height; ++y)
		for (unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[(height - y - 1) * width + x];
			unsigned int pos = (y * width + x) * 3;
			bytes[pos + 2] = c.r;
			bytes[pos + 1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width * height * 3, file);
	fclose(file);
	return true;
}

#ifndef IGNORE_LAMBDAS

//you can apply and algorithm for two images and store the result in the first one
//forEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void forEachPixel(Image& img, const Image& img2, F f) {
	for (unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f(img.pixels[pos], img2.pixels[pos]);
}
#endif


//Funció per dibuixar un rectangle
void Image::drawRectangle(int start_x, int start_y, int width, int height, Color color, bool fill) {

	//Comprovem si volem pintar el rectangle per dins
	if (fill) {
		for (int x = start_x; x < (start_x + width); ++x)
		{
			for (int y = start_y; y < (start_y + height); ++y)
			{
				this->setPixelSafe(x, y, color); //Iterem en tots els pixels coresponents al rectangle i els pintem
			}
		}
	}
	else {
		for (int x = start_x; x < (start_x + width); ++x)
		{
			//Pintem la línia inferior i superior
			this->setPixelSafe(x, start_y, color);
			this->setPixelSafe(x, start_y + height, color);
		}
		for (int y = start_y; y < (start_y + height); ++y)
		{

			//Pintem les verticals
			this->setPixelSafe(start_x, y, color);
			this->setPixelSafe(start_x + width, y, color);
		}
	}
}

//Funció que ens permet dibuixar un cercle
void Image::drawCircle(int start_x, int start_y, int r, Color color, bool fill) {

	//Comprovem si volem pintar el cercle per dins
	if (fill) {
		//Comencem desde el vertex esquerra de sota del quadrat que inclou el cercle fins on acaba aquest rectangle
		for (int x = start_x - r; x < ((start_x - r) + 2 * r); ++x)
		{
			for (int y = start_y - r; y < ((start_y - r) + 2 * r); ++y)
			{

				//Comprobem si el punt està forma part de la circumferència amb la seva ecuació implícita
				if (((x - start_x) * (x - start_x) + (y - start_y) * (y - start_y)) - (r * r) <= 0) {
					this->setPixelSafe(x, y, color);
				}
			}
		}
	}
	else {
		for (int x = start_x - r; x < ((start_x - r) + 2 * r); ++x)
		{
			for (int y = start_y - r; y < ((start_y - r) + 2 * r); ++y)
			{
				//En aquest cas hem de comprobar si els punts estan dins el cercle, però compleixen que obtenen com a resultat de l'equació un valor >= a -2r 
				//donat per aplicar un gruix donat per (r-G)^2 <= equació <=0
				if (((x - start_x) * (x - start_x) + (y - start_y) * (y - start_y)) - (r * r) <= 0 && ((x - start_x) * (x - start_x) + (y - start_y) * (y - start_y)) - (r * r) >= -2 * r) {
					this->setPixelSafe(x, y, color);
				}
			}
		}
	}
}


//Funció que ens permetrá dibuixar una línia
void Image::drawLine(int x1, int y1, int x2, int y2, Color color, bool thickness) {

	//Calculem vectors
	float v1 = (x2 - x1);
	float v2 = (y2 - y1);
	float m = 0.0;

	//En el cas de que la primera component sigui diferent de 0, calculem el pendent de la recta
	if (v1 != 0) {
		m = (v2 / v1);
	}

	//Comprovacions per si no intrdueixen els punts ordenats
	if (x2 > x1) {
		for (int x = x1; x < x2; x++)
		{
			//Calculem el valor del píxel y per cada x
			int y = m * (x - x1) + y1;
			this->setPixelSafe(x, y, color);

			//Si volem afegir gruixària a la recta, dibuixem els pixels del seu voltant
			if (thickness) {
				this->setPixelSafe(x, y + 1, color);
				this->setPixelSafe(x, y + 2, color);
				this->setPixelSafe(x, y + 3, color);
			}

		}
	}

	else if (x2 < x1) {
		for (int x = x2; x < x1; x++)
		{
			int y = m * (x - x1) + y1;
			this->setPixelSafe(x, y, color);
			if (thickness) {
				this->setPixelSafe(x, y + 1, color);
				this->setPixelSafe(x, y + 2, color);
				this->setPixelSafe(x, y + 3, color);
			}
		}
	}
	else {

		//Si la recta va cap amunt amb pendent infinit
		if (y2 > y1) { 
			for (int y = y1; y < y2; y++)
			{
				this->setPixelSafe(x1, y, color);
				if (thickness) {
					this->setPixelSafe(x1, y + 1, color);
					this->setPixelSafe(x1, y + 2, color);
					this->setPixelSafe(x1, y + 3, color);
				}
			}
		}
		else { 
			//Si la recta va cap avall amb pendent infinit
			for (int y = y1; y > y2; y--) //Per anar de y1 a y2 en aquest cas hem de decrementar y
			{
				
				this->setPixelSafe(x1, y, color);
				if (thickness) {
					this->setPixelSafe(x1, y + 1, color);
					this->setPixelSafe(x1, y + 2, color);
					this->setPixelSafe(x1, y + 3, color);
				}
			}
		}
	}
}

//Funció que ens permet dibuixar un gradient de dos color escollits
void Image::drawGradientH(Color startColor, Color endColor) {

	for (int x = 0; x < this->width; ++x)
	{
		for (int y = 0; y < this->height; ++y)
		{
			//Calculem la intensitat del color iterativament segons el pixel en el que ens trobem
			float intensity = x / (float)this->width;

			//interpolem cada color
			//Calculem el nivell de color segons la intensitat donat el pixel en el que estem 
			float red = startColor.r + (endColor.r - startColor.r) * (intensity);
			float green = startColor.g + (endColor.g - startColor.g) * (intensity);
			float blue = startColor.b + (endColor.b - startColor.b) * (intensity);

			//Pintem el pixel amb els nous valors del pixel segons la intensitat
			this->setPixel(x, y, Color(red, green, blue));
		}
	}
}


//Funcio gradient circular blanc/negre
void Image::drawRadialGradient() {

	//Trobem el centre de la pantalla
	int centerPointX = this->width / 2;
	int centerPointY = this->height / 2;

	//Calculem la distancia mes gran fins el centre
	float farDistanceCenter = sqrt(abs(pow(0 - centerPointX, 2)) + abs(pow(0 - centerPointY, 2)));

	for (int x = 0; x < this->width; x++)
	{
		for (int y = 0; y < this->height; y++)
		{
			//Calculem la distancia fins un punt qualsevol
			float distanceFromPointToCenter = sqrt(abs(pow(x - centerPointX, 2)) + abs(pow(y - centerPointY, 2)));

			//Fem la diferencia entre los dos distancies i normalitzem entre 0 i 1
			float intensity = (farDistanceCenter - distanceFromPointToCenter) / farDistanceCenter;
			float f = intensity * 255;

			//Invertim per tenir el gradient al reves
			f = 255 - f;
			this->setPixelSafe(x, y, Color(f, f, f));

		}
	}
}

//Funció per aplicar un filtre d'average a la imatge
void Image::greyImg(Image img) {

	for (unsigned int x = 0; x < this->width; x++) {
		for (unsigned int y = 0; y < this->height; y++) {
			Color actualPixel = img.getPixelSafe(x, y);

			//Realitzem la mitjana aritmètica dels valors de les components rgb
			float average = (actualPixel.r + actualPixel.g + actualPixel.b) / 3.0;

			//Actualitzem els valors dels píxels amb els nous valors donats per el càlcul anterior
			this->setPixel(x, y, Color(average, average, average));
		}
	}
}

//Funció que ens permet aplicar el filtre negatiu a la imatge
void Image::invertImg(Image img) {

	for (unsigned int x = 0; x < this->width; x++) {
		for (unsigned int y = 0; y < this->height; y++) {
			//Prenem el pixel en el que estem
			Color actualPixel = img.getPixelSafe(x, y);

			//Actualitzem el valor del pixel amb els valors negatius dels valors rgb anteriors
			this->setPixel(x, y, Color(-actualPixel.r, -actualPixel.g, -actualPixel.b));
		}
	}
}


//Funció que ens permet rotar una imatge
void Image::rotateImg(Image img, int angle) {

	//Calculem el centre del framebuffer
	float centerPointX = this->width / 2;
	float centerPointY = this->height / 2;
	for (unsigned int x = 0; x < this->width; x++) {
		for (unsigned int y = 0; y < this->height; y++) {

			//Passem l'angle de rotació a radiants
			float newAngle = angle * (PI / 180.0);

			//Calculem els valors dels píxels dels que volem informació per girar
			float newX = cos(newAngle) * (x - centerPointX) - sin(newAngle) * (y - centerPointY) + centerPointX;
			float newY = sin(newAngle) * (x - centerPointX) + cos(newAngle) * (y - centerPointY) + centerPointY;

			//Actualitzem els valors dels píxels segons el píxel que hem pres per girar
			this->setPixelSafe(x, y, img.getPixelSafe(round(newX), round(newY)));
		}
	}
}

//Funció que permet escalar la imatge
void Image::scaleImg(Image img, float scaleRatio) {

	for (unsigned int x = 0; x < this->width; x++) {
		for (unsigned int y = 0; y < this->height; y++) {
			//Calculem el paràmetre d'escalat
			float s = (1.0 / scaleRatio);

			//Multipliquem els pixels de la foto per l'escalat
			this->setPixelSafe(x, y, img.getPixel(x * s, y * s));
		}
	}
}

