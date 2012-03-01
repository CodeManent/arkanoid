#include "Actor.h"
#include <cstdlib>
#include <gl/glut.h>

namespace Arkanoid{
	Actor::Actor(const point2f &position, const point3f &color)
		:position(position),
		color(color)
	{
	}
	Actor::Actor(const point3f &color)
		:position(onePoint2f/2),
		color(color)
	{
	}

	Actor::~Actor()
	{
	}

	void Actor::suicide(){
		delete this;
	}

	const point2f& Actor::getPosition() const
	{
		return position;
	}
}