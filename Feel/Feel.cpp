#include "Feel.h"
#include "../ResourceManager.h"

#include <cstdlib>
#include <gl/glut.h>
#include <string>

Feel::Feel()
{
}

Feel::~Feel()
{
}

bool Feel::step()
{
	return false;
}

void Feel::draw()
{
	glPushMatrix();
		glLoadIdentity();

		glColor3f(0.7f, 0.7f, 0.7f);

		resourceManager.bindTexture("transparent feel.png");
		glEnable(GL_TEXTURE_2D);
			glBegin(GL_QUADS);
				glTexCoord2i(0, 0);		glVertex2f(0.0f, 0.0f);
				glTexCoord2i(0, 733);	glVertex2f(0.0f, 1.0f);
				glTexCoord2i(645, 733);	glVertex2f(1.0f, 1.0f);
				glTexCoord2i(645, 0);	glVertex2f(1.0f, 0.0f);
			glEnd();
		glDisable(GL_TEXTURE_2D);

		const std::string msg("That feel when you lost the game");
		glColor3f(1.0f, 1.0f, 1.0f);
		glRasterPos2f(0.35f, 0.9f);
		for(unsigned int chPos = 0; chPos < msg.size(); ++chPos)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msg.at(chPos));
		}
	glPopMatrix();
}

void Feel::input(const Input &input)
{
	switch(input.type)
	{
	case Input::Start:
	case Input::Back:
	case Input::A:
	case Input::B:
	case Input::X:
	case Input::Y:
		if(input.value.buttonValue == Input::Value::ButtonDown)
			finished = true;
	default:
		return;
	}
}

bool Feel::isActive()
{
	return true;
}

bool Feel::isFinished()
{
	return finished;
}
