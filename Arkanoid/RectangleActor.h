#ifndef Arkanoid_RectangleActor_H
#define Arkanoid_RectangleActor_H

#include "Actor.h"

namespace Arkanoid{
	class RectangleActor: public Actor
	{
	public:
		RectangleActor(const point2f &position, const point2f &size, const point3f &color = onePoint3f);
		virtual ~RectangleActor(void);

		virtual void draw() const;
		virtual const point2f& getSize() const;

	protected:
		point2f size;
	};
}

#endif
