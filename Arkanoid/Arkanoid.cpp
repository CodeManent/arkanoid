#include "Arkanoid.h"
#include <algorithm>
#include <functional>

#include <windows.h>
#include <cstdlib>
#include <gl/glut.h>
#include <iostream>

namespace Arkanoid{

	/*
		Construct game objects
		Position actors
	*/
	Arkanoid::Arkanoid(void)
		:ball(NULL),
		racket(NULL)
	{
		const point2f racketPosition(0.5f, 0.95f);
		const point2f racketSize(0.13f, 0.02f);
		racket.reset(new Racket(racketPosition, racketSize));


		{
			const float ballRadious = 0.015f;
			const point2f ballPosition(racketPosition.x, racketPosition.y - racketSize.y - ballRadious);
			const point3f ballColor(0.7f, 0.1f, 0.5f);
			ball.reset(new Ball(ballPosition, ballRadious, ballColor));
		}

		{
			point2f brickCount(15.0f, 20.0f);
			point2f bricksPosition(0.5f, 0.25f);
			point2f bricksSize(0.9f, 0.4f);

			point2f brickSize = bricksSize/brickCount;
			point2f firstBrickPosition = bricksPosition - bricksSize/2 + brickSize/2;
			for(float y = 0; y < brickCount.y; ++y){
				for(float x = 0; x < brickCount.x; ++x){
					const point2f position = firstBrickPosition + brickSize*(const point2f(x, y));

					const point3f color = point3f(0.9f, 0.9f, 0.9f);

					bricks.push_back(new Brick(position, brickSize, color));
				}
			}
		}
	}

	Arkanoid::~Arkanoid(void)
	{
		using std::for_each;
		using std::mem_fun;

		for_each(bricks.begin(), bricks.end(), mem_fun(&Actor::suicide));
	}

	void Arkanoid::step()
	{
		//move racket
		racket->step();

		//move ball
		ball->step();
/*
		if(ball->getPosition().y > 1.0f){
			finished = true;
			return;
		}
*/

/*
		if(ball->collidesWith(*racket.get())){
			ball->racketCollision(*racket.get());
		}

		for(unsigned int i = 0; i < bricks.size(); ++i){
			if(ball->collidesWith(*bricks[i]))
			{
				ball->adjustSpeed(*bricks[i]);
				bricks.erase(bricks.begin() + i);
			}
		}
*/

		glutPostRedisplay();

		static DWORD startTime = GetTickCount();
		static unsigned int frames = 0;
		++frames;
		const DWORD currentTime = GetTickCount();
		if(currentTime - startTime >= 1000){
			std::cout << frames << " fps" << std::endl;
			startTime = currentTime;
			frames = 0;
		}

	}

	void Arkanoid::draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		using std::for_each;
		using std::mem_fun;

		for_each(bricks.begin(), bricks.end(), mem_fun(&Brick::draw));

		ball->draw();
		racket->draw();
	}

	void Arkanoid::input(const Input &input)
	{
		switch(input.type){
			case Input::Left:
				racket->move(input);
				break;

			case Input::Right:
				racket->move(input);
				break;

			case Input::Back:
				finished = true;
				break;

			default:
				break;
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
