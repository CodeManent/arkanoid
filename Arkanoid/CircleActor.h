#ifndef Arkanoid_CylinderActor_H
#define Arkanoid_CylinderActor_H

#include "actor.h"

namespace Arkanoid{
	class CircleActor :public Actor
	{
	public:
		CircleActor(const point2f &position, const float &radious, const point3f &color = onePoint3f);
		virtual ~CircleActor(void);

		virtual void draw() const;

		virtual bool boxCollision(const point2f &boxPosition, const point2f &boxSize) const;
		virtual const float getRadious() const;


	protected:
		float radious;
		void glCirclePoints(unsigned int segments) const;
	};
}

#endif
