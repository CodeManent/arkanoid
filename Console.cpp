#include "Console.h"
#include <cstdlib>
#include <gl/glut.h>
#include <sstream>

Console* Console::currentConsole = NULL;

Console &console = Console::get();

Console& Console::get()
{
	if(currentConsole == NULL)
	{
		currentConsole = new Console();
	}
	return *currentConsole;
}


Console::Console(void)
:visible(false),
maxBufferSize(50)
{
}

Console::~Console(void)
{
}

void Console::show()
{
	visible = true;
}

void Console::hide()
{
	visible = false;
}


//splits the input in lines and add each of them to the line buffer
//keeps buffer at 100 lines
void Console::write(std::string message)
{
	std::stringstream ss(message);
	std::string line;

	while(getline(ss, line, '\n'))
	{
		buffer.push_back(line);
	}

	while(buffer.size() > maxBufferSize)
	{
		buffer.pop_front();
	}
}

void Console::clear()
{
	buffer.clear();
}


void Console::draw()
{
	if(visible)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // GL_ONE);

		glPushMatrix();
			glLoadIdentity();
			glColor4f(0.2f, 0.2f, 0.2f, 0.5f);
			glBegin(GL_QUADS);
				glVertex2f(0.0f, 0.0f);
				glVertex2f(0.0f, 0.5f);
				glVertex2f(1.0f, 0.5f);
				glVertex2f(1.0f, 0.0f);
			glEnd();

			//set modelview matrix to represent pixels
			int windowX = glutGet(GLUT_WINDOW_WIDTH);
			int windowY = glutGet(GLUT_WINDOW_HEIGHT);
			glScalef(1.0f/windowX, 1.0f/windowY, 1.0f);

			int rasterX = 10;
			int rasterY = windowY/2 - 4;
			int consoleHeight = rasterY;

			std::list<std::string>::const_reverse_iterator line;

			for(line = buffer.rbegin(); line != buffer.rend() && rasterY > 0; ++line, rasterY-= 14)
			{
				float colorFactor = float(rasterY)/consoleHeight;
				glColor3f(0.8f*colorFactor + 0.19f, 0.8f*colorFactor + 0.19f, 0.8f*colorFactor + 0.19f);
				glRasterPos2i(rasterX, rasterY);

				size_t lineSize = line->size();
				for(unsigned int chPos = 0; chPos < lineSize; ++chPos)
				{
					glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, line->c_str()[chPos]);// line->at(chPos));
				}
			}
		glPopMatrix();
		glDisable(GL_BLEND);
	}
}
