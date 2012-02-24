#ifndef Arkanoid_Brick_H
#define Arkanoid_Brick_H

#include "RectangleActor.h"
#include "../point2.h"

namespace Arkanoid{
	class Brick: public RectangleActor
	{
	public:
		Brick(const point2f &position, const point2f &size, const point3f &color);
		virtual ~Brick();

		void step();
	};
}

#endif
