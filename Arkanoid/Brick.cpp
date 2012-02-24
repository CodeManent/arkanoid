#include "Brick.h"

namespace Arkanoid{
	Brick::Brick(const point2f &position, const point2f &size, const point3f &color)
		:RectangleActor(position, size, color)
	{
	}

	Brick::~Brick()
	{
	}
}
