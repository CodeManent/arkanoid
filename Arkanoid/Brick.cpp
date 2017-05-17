#include "Brick.h"
#include "../ResourceManager.h"

#include <cstdlib>
#include <gl/glut.h>

namespace Arkanoid{
	const point2f Brick::BRICK_SIZE = point2f(0.9, 0.4);

	Brick::Brick(const point2f &position, const point2f &size, const point3f &color)
		:RectangleActor(position, size, color), texture(DEFAULT)
	{
	}

	Brick::Brick(const point2f &position, BrickTexture texture)
		:RectangleActor(position, BRICK_SIZE, zeroPoint3f)
		,texture(texture)
	{

	}

	Brick::~Brick()
	{
	}

	void Brick::draw() const
	{
		//initial tPositions are set to pink
		point2i tUpLeft(2, 2);
		point2i tDownRight(31, 16);
		point2i tSize = tDownRight - tUpLeft;

		switch(texture)
		{
		case PINK:		tUpLeft = point2i(2, 2);	break;
		case BROWN:		tUpLeft = point2i(34, 2);	break;
		case BLUE:		tUpLeft = point2i(66, 2);	break;
		case DARK_GRAY:	tUpLeft = point2i(98, 2);	break;
		case LIGHT_GRAY:tUpLeft = point2i(130, 2);	break;
		case WHITE:		tUpLeft = point2i(162, 2);	break;
		case ORANGE:	tUpLeft = point2i(2, 34);	break;
		case RED:		tUpLeft = point2i(2, 66);	break;
		case YELLOW:	tUpLeft = point2i(2, 98);	break;

		case DEFAULT:
		default:
			RectangleActor::draw();
			return;
		}

		tDownRight = tUpLeft + tSize;


		const point2f topleft = position - size/2;
		const point2f bottomright = position + size/2;

		resourceManager.bindTexture("Arkanoid\\arinoid_master.png");

		glEnable(GL_TEXTURE_2D);
		//glColor3f(color.x, color.y, color.z);
		glColor3f(1.0f, 1.0f, 1.0f);

		glBegin(GL_QUADS);
			glTexCoord2i(tUpLeft.x, tUpLeft.y);
			glVertex2f(topleft.x, topleft.y);

			glTexCoord2i(tUpLeft.x, tDownRight.y);
			glVertex2f(topleft.x, bottomright.y);

			glTexCoord2i(tDownRight.x, tDownRight.y);
			glVertex2f(bottomright.x, bottomright.y);

			glTexCoord2i(tDownRight.x, tUpLeft.y);
			glVertex2f(bottomright.x, topleft.y);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	}

	Brick::BrickTexture Brick::getTexture() const
	{
		return texture;
	}
}
