#include "RectangleActor.h"

#include <cstdlib>
#include <gl/glut.h>

namespace Arkanoid{

	RectangleActor::RectangleActor(const point2f &position, const point2f &size, const point3f &color)
		:Actor(position, color),
		size(size)
	{
	}

	RectangleActor::~RectangleActor()
	{
	}

	void RectangleActor::draw() const
	{
		const point2f topleft = position - size/2;
		const point2f bottomright = position + size/2;

		glColor3f(color.x, color.y, color.z);
		glBegin(GL_QUADS);
			glVertex2f(topleft.x, topleft.y);
			glVertex2f(topleft.x, bottomright.y);
			glVertex2f(bottomright.x, bottomright.y);
			glVertex2f(bottomright.x, topleft.y);
		glEnd();

		const point3f borderColor = point3f(1.0f, 1.0f, 1.0f) - color;
		glColor3f(borderColor.x, borderColor.y, borderColor.z);
		glBegin(GL_LINE_LOOP);
			glVertex2f(topleft.x, topleft.y);
			glVertex2f(topleft.x, bottomright.y);
			glVertex2f(bottomright.x, bottomright.y);
			glVertex2f(bottomright.x, topleft.y);
		glEnd();
	}
}
