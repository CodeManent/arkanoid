#ifndef Arkanoid_Wall_H
#define Arkanoid_Wall_H

#include "RectangleActor.h"

namespace Arkanoid{
	class Wall: public RectangleActor
	{
	public:
		Wall(const point2f &position, const point2f &size);
		virtual ~Wall();
	};
}


#endif
