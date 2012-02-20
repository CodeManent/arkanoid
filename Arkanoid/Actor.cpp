#include "Actor.h"
#include <cstdlib>
#include <gl/glut.h>

namespace Arkanoid{
	Actor::Actor()
		:position(0.0f), color(1.0f, 1.0f, 1.0f)
	{
	}

	Actor::Actor(const point3f &position, const point3f &size)
		:position(position), size(size), color(1.0f, 1.0f, 1.0f)
	{
	}

	Actor::Actor(const point3f &position, const point3f &size, const point3f &color)
		:position(position), size(size), color(color)
	{
	}

	Actor::~Actor()
	{
	}

	void Actor::draw()
	{
		const point3f topleft = position - size/2;
		const point3f bottomright = position + size/2;

		glColor3f(color.x, color.y, color.z);
		glBegin(GL_QUADS);
			glVertex2f(topleft.x, topleft.y);
			glVertex2f(topleft.x, bottomright.y);
			glVertex2f(bottomright.x, bottomright.y);
			glVertex2f(bottomright.x, topleft.y);
		glEnd();
	}
}