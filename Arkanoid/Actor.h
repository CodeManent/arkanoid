#ifndef Arkanoid_Actor_H
#define Arkanoid_Actor_H

#include "../point3.h"
#include "../point2.h"

namespace Arkanoid{
	class Actor
	{
	public:
		Actor(const point2f &position = onePoint2f/2, const point3f &color = onePoint3f);
		Actor(const point3f &color);
		virtual ~Actor();

		virtual void draw() const = 0;

		virtual void suicide();

	protected:
		point3f color;
		point2f position;
	};
}

#endif
