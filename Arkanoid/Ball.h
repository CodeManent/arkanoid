#ifndef Arkanoid_Ball_H
#define Arkanoid_Ball_H

#include "CircleActor.h"

namespace Arkanoid{
	class Ball: public CircleActor
	{
	public:
		Ball(const point2f &position, const float &radious, const point3f &color = onePoint3f);
		virtual ~Ball();

		virtual void step();


	protected:
		point2f speed;
		static const float maxSpeed;
	};
}

#endif
