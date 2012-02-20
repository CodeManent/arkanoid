#include "Racket.h"

namespace Arkanoid{
	Racket::Racket(const point3f &position, const point3f &size)
		:Actor(position, size, point3f(0.974f, 0.157f, 0.214f))
	{
	}

	void Racket::draw() const
	{
		(*this).Actor::draw();

	}
}
