#include "image.h"



Image::Image() {
	width = 0; height = 0;
	pixels = NULL;
}

Image::Image(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	pixels = new Color[width*height];
	memset(pixels, 0, width * height * sizeof(Color));
}

//copy constructor
Image::Image(const Image& c) {
	pixels = NULL;

	width = c.width;
	height = c.height;
	if(c.pixels)
	{
		pixels = new Color[width*height];
		memcpy(pixels, c.pixels, width*height*sizeof(Color));
	}
}

//assign operator
Image& Image::operator = (const Image& c)
{
	if(pixels) delete pixels;
	pixels = NULL;

	width = c.width;
	height = c.height;
	if(c.pixels)
	{
		pixels = new Color[width*height*sizeof(Color)];
		memcpy(pixels, c.pixels, width*height*sizeof(Color));
	}
	return *this;
}

Image::~Image()
{
	if(pixels) 
		delete pixels;
}



//change image size (the old one will remain in the top-left corner)
void Image::resize(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];
	unsigned int min_width = this->width > width ? width : this->width;
	unsigned int min_height = this->height > height ? height : this->height;

	for(unsigned int x = 0; x < min_width; ++x)
		for(unsigned int y = 0; y < min_height; ++y)
			new_pixels[ y * width + x ] = getPixel(x,y);

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

//change image size and scale the content
void Image::scale(unsigned int width, unsigned int height)
{
	Color* new_pixels = new Color[width*height];

	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
			new_pixels[ y * width + x ] = getPixel((unsigned int)(this->width * (x / (float)width)), (unsigned int)(this->height * (y / (float)height)) );

	delete pixels;
	this->width = width;
	this->height = height;
	pixels = new_pixels;
}

Image Image::getArea(unsigned int start_x, unsigned int start_y, unsigned int width, unsigned int height)
{
	Image result(width, height);
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height; ++y)
		{
			if( (x + start_x) < this->width && (y + start_y) < this->height) 
				result.setPixel( x, y, getPixel(x + start_x,y + start_y) );
		}
	return result;
}

void Image::flipX()
{
	for(unsigned int x = 0; x < width * 0.5; ++x)
		for(unsigned int y = 0; y < height; ++y)
		{
			Color temp = getPixel(width - x - 1, y);
			setPixel( width - x - 1, y, getPixel(x,y));
			setPixel( x, y, temp );
		}
}

void Image::flipY()
{
	for(unsigned int x = 0; x < width; ++x)
		for(unsigned int y = 0; y < height * 0.5; ++y)
		{
			Color temp = getPixel(x, height - y - 1);
			setPixel( x, height - y - 1, getPixel(x,y) );
			setPixel( x, y, temp );
		}
}


//Loads an image from a TGA file
bool Image::loadTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];
	unsigned int bytesPerPixel;
	unsigned int imageSize;

	FILE * file = fopen(filename, "rb");
   	if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
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
	if(pixels)
		delete pixels;

	width = tgainfo->width;
	height = tgainfo->height;
	pixels = new Color[width*height];

	//convert to float all pixels
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
			this->setPixel(x , height - y - 1, Color( tgainfo->data[pos+2], tgainfo->data[pos+1], tgainfo->data[pos]) );
		}

	delete tgainfo->data;
	delete tgainfo;

	return true;
}

// Saves the image to a TGA file
bool Image::saveTGA(const char* filename)
{
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	FILE *file = fopen(filename, "wb");
	if ( file == NULL )
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
	unsigned char* bytes = new unsigned char[width*height*3];
	for(unsigned int y = 0; y < height; ++y)
		for(unsigned int x = 0; x < width; ++x)
		{
			Color c = pixels[(height-y-1)*width+x];
			unsigned int pos = (y*width+x)*3;
			bytes[pos+2] = c.r;
			bytes[pos+1] = c.g;
			bytes[pos] = c.b;
		}

	fwrite(bytes, 1, width*height*3, file);
	fclose(file);
	return true;
}

#ifndef IGNORE_LAMBDAS

//you can apply and algorithm for two images and store the result in the first one
//forEachPixel( img, img2, [](Color a, Color b) { return a + b; } );
template <typename F>
void forEachPixel(Image& img, const Image& img2, F f) {
	for(unsigned int pos = 0; pos < img.width * img.height; ++pos)
		img.pixels[pos] = f( img.pixels[pos], img2.pixels[pos] );
}
#endif

void Image::drawRectangle(int start_x, int start_y, int width, int height, Color color, bool fill) {
	
		if (fill) {
			for (int x = start_x; x < (start_x + width); ++x)
			{
				for (int y = start_y; y < (start_y + height); ++y)
				{
					this->setPixelSafe(x, y, color);
				}
			}
		}
		else {
			for (int x = start_x; x < (start_x + width); ++x)
			{
				this->setPixelSafe(x, start_y, color);
				this->setPixelSafe(x, start_y + height - 1, color);
			}
			for (int y = start_y + 1; y < (start_y + height - 1); ++y)
			{
				this->setPixelSafe(start_x, y, color);
				this->setPixelSafe(start_x + width - 1, y, color);
			}
		}
}

void Image::drawCircle(int start_x, int start_y, int r, Color color, bool fill) {
	
	if (fill) {
		for (int x = start_x-r; x < ((start_x - r)+2*r); ++x)
		{
			for (int y = start_y-r; y < ((start_y - r) + 2 * r); ++y)
			{
				if (((x- start_x)*(x- start_x) + (y- start_y)*(y- start_y))-(r*r) <= 0 ){
					this->setPixelSafe(x, y, color);
				}
			}
		}
	}	
	else
	{
		for (int x = start_x - r; x < ((start_x - r) + 2 * r); ++x)
		{
			for (int y = start_y - r; y < ((start_y - r) + 2 * r); ++y)
			{
				if (((x - start_x) * (x - start_x) + (y - start_y) * (y - start_y)) - (r * r) <= 0 && ((x - start_x) * (x - start_x) + (y - start_y) * (y - start_y)) - (r * r) >= -2 * r) {
					this->setPixelSafe(x, y, color);
				}
			}
		}
	}
}

void Image::drawLine(int x1, int y1, int x2, int y2, Color color) {
	
	int v1 = (x2 - x1);
	int v2 = (y2 - y1);
	int m = v2 / v1;
	
	for (int x = x1; x < x2; x++)
	{
		int y = m * (x - x1) + y1;
		this->setPixelSafe(x, y, color);
	}

}

void Image::drawGradientH(Color startColor, Color endColor) {
	
	for (int x = 0; x < this->width; ++x)
	{
		for (int y = 0; y < this->height; ++y)
		{
			
			float intensity = x / (float)this->width;

			//interpolate each color
			float red = startColor.r + (endColor.r - startColor.r) * (intensity);
			float blue= startColor.b + (endColor.b - startColor.b) * (intensity);
			float green= startColor.g + (endColor.g - startColor.g) * (intensity);
			
			
			this->setPixel(x, y, Color(red,green, blue));
		}
	}
}

void Image::drawGradientRadial() {


	int centerPointX = this->width / 2;
	int centerPointY = this->height / 2;
	float farDistanceCenter = sqrt(abs(pow(0 - centerPointX, 2)) + abs(pow(0 - centerPointY, 2)));

	for (int x = 0; x < this->width; x++)
	{
		for (int y = 0; y < this->height; y++)
		{

			float distanceFromPointToCenter = sqrt(abs(pow(x - centerPointX, 2)) + abs(pow(y - centerPointY, 2)));

			

		}
	}


	//double RadialGradient<T>::adjustRange(double oldNum, double oldMin, double oldMax, double newMin, double newMax) {
		//return ((((newMax - newMin) * (oldNum - oldMin)) / (oldMax - oldMin)) + newMin);
	//}


}
void Image::greyImg(Image img) {
	for (unsigned int x = 0; x < this->width; x++) {
		for (unsigned int y = 0; y < this->height; y++) {
			Color actualPixel = img.getPixelSafe(x, y);
			float average = (actualPixel.r+actualPixel.g+actualPixel.b)/3;
			this->setPixel(x, y, Color(average,average,average));
		}
	}
}
void Image::invertImg(Image img) {
	for (unsigned int x = 0; x < this->width; x++) {
		for (unsigned int y = 0; y < this->height; y++) {
			Color actualPixel = img.getPixelSafe(x, y);
			
			this->setPixel(x, y, Color(-actualPixel.r, -actualPixel.g, -actualPixel.b));
		}
	}
}

void Image::rotateImg(Image img, int angle) {
	float centerPointX = this->width / 2;
	float centerPointY = this->height / 2;
	for (unsigned int x = 0; x < this->width; x++) {
		for (unsigned int y = 0; y < this->height; y++) {
			float newAngle = angle * (PI / 180);
			
			float newX = cos(-newAngle) * (x-centerPointX) - sin(-newAngle) * (y-centerPointY)+centerPointX;
			float newY = sin(-newAngle) * (x - centerPointX) + cos(-newAngle) * (y-centerPointY)+centerPointY;

			this->setPixelSafe(round(newX), round(newY), img.getPixel(x, y));
		}
	}
}

void Image::scaleImg(Image img,float scaleRatio) {
	for (unsigned int x = 0; x < this->width; x++) {
		for (unsigned int y = 0; y < this->height; y++) {
			float s = (1.0 / scaleRatio);

			this->setPixelSafe(x, y, img.getPixel(x*s, y*s));
		}
	}
}
	