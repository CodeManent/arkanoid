#include "Wall.h"

namespace Arkanoid{
	Wall::Wall(const point3f &position, const point3f &size)
		:Actor(position, size, point3f(0.0f, 0.0f, 1.0f))
	{
	}

	Wall::Wall(const point3f &position, const point3f &size, const point3f &color)
		:Actor(position, size, color)
	{
	}

	void Wall::draw(){
		this->Actor::draw();
	}

}
