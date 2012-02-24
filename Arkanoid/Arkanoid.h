#pragma once
#include "../State.h"

#include "Ball.h"
#include "Racket.h"
#include "Brick.h"
#include "Wall.h"

#include <vector>
#include <memory>

namespace Arkanoid{
	class Arkanoid : public State
	{
	public:
		Arkanoid(void);
		virtual ~Arkanoid(void);

		virtual bool step();
		virtual void draw();
		virtual void input(const Input &input);
		virtual bool isActive();
		virtual bool isFinished();

	protected:

		//actors
		std::auto_ptr<Ball> ball;
		std::auto_ptr<Racket> racket;
		std::vector<Brick *> bricks;
	};
}
