#ifndef Arkanoid_Brick_H
#define Arkanoid_Brick_H

#include "RectangleActor.h"
#include "../point2.h"

namespace Arkanoid{
	class Brick: public RectangleActor
	{
	public:
		static const point2f BRICK_SIZE;
		enum BrickTexture{PINK, BROWN, BLUE, DARK_GRAY, LIGHT_GRAY, WHITE, ORANGE, RED, YELLOW, DEFAULT};

		Brick(const point2f &position, const point2f &size, const point3f &color);
		Brick(const point2f &position, enum BrickTexture color);
		virtual ~Brick();
		virtual void draw() const;

		void step();

		BrickTexture getTexture() const;
	private:
		BrickTexture texture;
	};
}

#endif
