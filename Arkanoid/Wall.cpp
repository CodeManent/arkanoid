#include "Wall.h"

namespace Arkanoid{
	Wall::Wall(const point2f &position, const point2f &size)
		:RectangleActor(position, size, point3f(0.0f, 0.0f, 1.0f))
	{
	}

	Wall::~Wall()
	{
	}
}
