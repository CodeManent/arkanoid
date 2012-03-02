#include "Arkanoid.h"

#include <algorithm>
#include <functional>

#include <cstdlib>
#include <gl/glut.h>
#include <iostream>

namespace Arkanoid{

	/*
		Construct game objects
		Position actors
	*/
	//board size 10.0f, 10.0f
	Arkanoid::Arkanoid(void)
		:ball(NULL),
		racket(NULL),
		paused(false)
	{
		const point2f racketPosition(5.0f, 9.5f);
		const point2f racketSize(1.3f, 0.2f);
		racket.reset(new Racket(racketPosition, racketSize));


		{
			const float ballRadious = 0.15f;
			const point2f ballPosition(racketPosition.x, racketPosition.y - racketSize.y - ballRadious);
			const point3f ballColor(0.7f, 0.1f, 0.5f);
			ball.reset(new Ball(ballPosition, ballRadious, ballColor));
		}

		{
			point2f brickCount(10.0f, 10.0f);
			point2f bricksPosition(5.0f, 2.5f);
			point2f bricksSize(9.0f, 4.0f);

			point2f brickSize = bricksSize/brickCount;
			point2f firstBrickPosition = bricksPosition - bricksSize/2 + brickSize/2;
			for(float y = 0; y < brickCount.y; ++y){
				for(float x = 0; x < brickCount.x; ++x){
					const point2f position = firstBrickPosition + brickSize*(const point2f(x, y));

					const point3f color = point3f(x/brickCount.x, y/brickCount.y, (x+y)/(brickCount.x + brickCount.y));

					bricks.push_back(new Brick(position, brickSize, color));
				}
			}
		}
/*
		for(int i=0; i < 1000; ++i){
			randomPoints.push_back(point2f(10.0f*(float)rand()/float(RAND_MAX), 10.0f*(float)rand()/float(RAND_MAX)));
		}
//*/
/*
		for(float y = 0.5f; y < 10; y+=0.5f){
			for(float x = 0.5f; x < 10; x+=0.5f){
				randomPoints.push_back(point2f(x, y));
			}
		}
//*/
	}






	Arkanoid::~Arkanoid(void)
	{
		using std::for_each;
		using std::mem_fun;

		for_each(bricks.begin(), bricks.end(), mem_fun(&Actor::suicide));
	}






	bool Arkanoid::step()
	{
		
		//move racket
		racket->step();

		//move ball
		if(!paused)
			ball->step(randomPoints, bricks, *racket.get());

		if(bricks.empty())
		{
			finished = true;
		}

		return true;//redraw
	}






	void Arkanoid::draw()
	{
		glClearColor(.9f, .75f, .7f, 0.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		//glTranslatef(0.5f, 0.5f, 0.0f);
		glScalef(1.0f/10.0f, 1.0f/10.0f, 1.0f/10.0f);

//*
		using std::for_each;
		using std::mem_fun;

		for_each(bricks.begin(), bricks.end(), mem_fun(&Brick::draw));
//*/
		ball->draw();
		racket->draw();

		glColor3f(1.0f, 0.0f, 0.0f);

		glClear(GL_DEPTH_BUFFER_BIT);
		glBegin(GL_POINTS);
		std::vector<const point2f>::const_iterator point;
		for(point = randomPoints.begin(); point != randomPoints.end(); ++point)
		{
			glVertex2f(point->x, point->y);
		}
		glEnd();
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

			case Input::Start:
				if(input.value.buttonValue == Input::Value::ButtonDown)
					paused = !paused;

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
