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
		:ball(NULL), racket(NULL)
	{
		racket.reset(new Racket(point3f(0.5f, 0.95f), point3f(0.13f, 0.02f)));
		ball.reset(new Ball(point3f(0.5f, 0.95f - 0.01f - 0.01f), 0.01f));

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
	}

	void Arkanoid::step()
	{
		//move racket
		racket->step();

		//move ball
		ball->step();
		if(ball->getPosition().y > 1.0f){
			finished = true;
			return;
		}


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


		static unsigned int i = 0;
		if(++i == 10){
			i = 0;

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
