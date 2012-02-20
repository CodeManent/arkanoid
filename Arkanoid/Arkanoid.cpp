#include "Arkanoid.h"
#include <algorithm>
#include <functional>

namespace Arkanoid{

	/*
		Construct game objects
		Position actors
	*/
	Arkanoid::Arkanoid(void)
		:ball(NULL), racket(NULL), floor(NULL)
	{
		racket.reset(new Racket(point3f(0.5f, 0.95f), point3f(0.2f, 0.02f)));
		ball.reset(new Ball(point3f(0.5f, 0.95f - 0.01f - 0.01f), 0.01f));
		floor.reset(new Wall(point3f(0.5f, 1.0f), point3f(1.0f, 0.01f)));

		point3f brickCount(20.0f, 20.0f);
		point3f bricksPosition(0.5f, 0.25f);
		point3f bricksSize(0.9f, 0.4f);

		point3f brickSize = bricksSize/brickCount;
		point3f firstBrickPosition = bricksPosition - bricksSize/2 + brickSize/2;
		for(float y = 0; y < brickCount.y; ++y){
			for(float x = 0; x < brickCount.x; ++x){
				bricks.push_back(new Brick(firstBrickPosition+brickSize*(const point3f(x, y)), brickSize));
			}
		}

	}

	Arkanoid::~Arkanoid(void)
	{
		using std::for_each;
		using std::mem_fun;

		for_each(bricks.begin(), bricks.end(), mem_fun(&Actor::suicide));
		for_each(walls.begin(),  walls.end(),  mem_fun(&Actor::suicide));

	}

	void Arkanoid::step()
	{
	}

	void Arkanoid::draw()
	{
		using std::for_each;
		using std::mem_fun;

		for_each(bricks.begin(), bricks.end(), mem_fun(&Brick::draw));
		for_each(walls.begin(), walls.end(), mem_fun(&Wall::draw));

		ball->draw();
		racket->draw();
		floor->draw();
	}

	void Arkanoid::input(const Input &input)
	{
		if(input == Start){
			finished = true;
		}
	}

	bool Arkanoid::isActive(){
		return active;
	}

	bool Arkanoid::isFinished()
	{
		return finished;
	}
}
