#ifndef Arkanoid_Arkanoid_H
#define Arkanoid_Arkanoid_H

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
		Arkanoid(std::string board);
		virtual ~Arkanoid(void);

		virtual bool step();
		virtual void draw();
		virtual void input(const Input &input);
		virtual std::auto_ptr<State> getNextState();

	protected:
		void loadBoard(std::string path);
		//actors
		std::auto_ptr<Ball> ball;
		std::auto_ptr<Racket> racket;
		std::vector<Brick> bricks;

		bool paused;
		bool lockedBall;
		unsigned int lives;
		std::string boardName;
		bool win;
	};
}

#endif
