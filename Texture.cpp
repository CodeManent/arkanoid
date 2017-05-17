#include "Texture.h"
#include "Console.h"

#include <windows.h>
#include <Gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
#include <stdexcept>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <memory>
#include <gl/glut.h>

Texture::Texture(const std::string &path)
:width(0)
,height(0)
,texturePointer(-1)
,path(path)
{
	loadFromFile(path);
	uploadToGraphicsCard();
	bind();
}






Texture::~Texture()
{
	if(texturePointer != -1)
		glDeleteTextures(1, &texturePointer);
}





void Texture::bind() const
{
	if(texturePointer == -1)
		throw( std::runtime_error( "Texture::bind: Trying to bind texture that doesn't exist" ) );

	glBindTexture(GL_TEXTURE_2D, texturePointer);
	glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glScalef(1.0f/float(width), 1.0f/float(height), 1.0f);
		//glScalef(float(width), float(height), 1.0f);
	glMatrixMode(GL_MODELVIEW);
}



//Load an image using GDI+
void Texture::loadFromFile(const std::string &path)
{
	using namespace Gdiplus;

	ULONG_PTR token = NULL;
	Bitmap *bmp = NULL;

	try{
		int length = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path.c_str(), -1, NULL, 0);
		std::auto_ptr<WCHAR> pathBuffer(new WCHAR[length]);
		int retval = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, path.c_str(), -1, pathBuffer.get(), length);
		if(retval == 0)
		{
			std::stringstream ss;
			ss << "Texture::loadFromFile(string):Error converting path to WCHAR: ";
			ss << GetLastError();
			throw(std::runtime_error(ss.str()));
		}

		Status st;
		GdiplusStartupInput startupInput;
		st = GdiplusStartup(&token, &startupInput, NULL);
		if(st != Ok)
		{
			throw(std::runtime_error("Texture::loadFromFile(string): GDI+ initialisation"));
		}

		bmp = Bitmap::FromFile(pathBuffer.get());
		if(bmp->GetLastStatus() != Ok)
		{
			throw(std::runtime_error("Texture::loadFromFile(string): Reading from disk"));
		}

		Color backgroundColor;
		st = bmp->GetPixel(0, 0, &backgroundColor);
		if(st != Ok)
		{
			throw(std::runtime_error("Texture::loadFromFile(string): Couldn't get pixel (0, 0)"));
		}

		this->width = bmp->GetWidth();
		this->height = bmp->GetHeight();
		imageData.clear();
		imageData.reserve(width*height*4);//RGBA
		Color currentPixel;

		for(unsigned int y = 0; y < height; ++y)
		{
			for(unsigned int x = 0; x < width; ++x)
			{
				if(bmp->GetPixel(x, y, &currentPixel) != Ok)
				{
					std::stringstream ss;
					ss << "Texture::loadFromFile(string): Couldn't get pixel (" << x << ", " << y << ")";
					throw(std::runtime_error(ss.str()));
				}

				imageData.push_back(currentPixel.GetRed());
				imageData.push_back(currentPixel.GetGreen());
				imageData.push_back(currentPixel.GetBlue());
				imageData.push_back(currentPixel.GetAlpha());
			}
		}

		this->format = GL_RGBA;

		delete bmp;
		GdiplusShutdown(token);
	}
	catch(std::exception &e)
	{
		//clean
		if(!bmp) delete bmp;
		if(!token) GdiplusShutdown(token);

		console.write(e.what());

		throw;
	}
}

//pushes the image data that are loaded from the fime to the gpu
//assumes valid openGL context available
//loaded texture is binded
void Texture::uploadToGraphicsCard()
{
	//generate texture
	glGenTextures(1, &texturePointer);

	//set texture paramters
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//bind the generated texture pointer
	glBindTexture(GL_TEXTURE_2D, texturePointer);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);// GL_BLEND);

	//push it to GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, &(imageData[0]));
}
