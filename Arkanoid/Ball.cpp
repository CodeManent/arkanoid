#include "Ball.h"

namespace Arkanoid{
	Ball::Ball(const point3f &position, const float &radious)
		:Actor(position, point3f(radious, radious, 0), point3f(0.37f, 0.89f, 0.75f))
	{
	}

	bool Ball::collidesWith(const Actor &actor)
	{
		return false;
	}

	void Ball::draw()
	{
		//TODO: must draw a ball
	}
}
