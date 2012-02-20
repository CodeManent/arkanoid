#include "Brick.h"

namespace Arkanoid{
	Brick::Brick(const point3f &position, const point3f &size)
		:Actor(position, size, point3f(0.4f, 0.7f, 0.1f))
	{
	}

	void Brick::draw() const
	{
		this->Actor::draw();
	}
}
