#ifndef Arkanoid_Racket_H
#define Arkanoid_Racket_H

#include "RectangleActor.h"
#include "../Input.h"

namespace Arkanoid{
	class Racket: public RectangleActor
	{
	public:
		Racket(const point2f &position, const point2f &size);
		virtual ~Racket();

		virtual void move(const Input &input);
		virtual void step();

	protected:
		float speed;

		enum MovementDirection{
			Left,
			Right,
			None
		} direction;


		static const float acceleration;
		static const float maxSpeed;
		static const float weight;
		static const point3f racketColor;
	};
}

#endif