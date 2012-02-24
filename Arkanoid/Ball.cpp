#include "Ball.h"
#include <cstdlib>
#include <gl/glut.h>
#include <iostream>

namespace Arkanoid{
	const float Ball::maxSpeed = 0.007;
	//point3f ballColor(0.37f, 0.89f, 0.75f)

	Ball::Ball(const point2f &position, const float &radious, const point3f &color)
		:CircleActor(position, radious, color),
		speed(point2f(0.7f, -1.0f).normalise()*maxSpeed)
	{
	}

	Ball::~Ball()
	{
	}
/*
	void Ball::adjustSpeed(const Actor &actor)
	{
		const point2f& boxPosition = actor.getPosition();
		const point2f& boxSize = actor.getSize();

		const int xZone = position.x  < (boxPosition.x - boxSize.x/2) ? 0 : 
						(position.x > (boxPosition.x + boxSize.x/2) ? 2 : 1);

		const int yZone = position.y < (boxPosition.y - boxSize.y/2) ? 0 :
						(position.y > (boxPosition.y + boxSize.y/2) ? 2 : 1);

		const int zone = xZone + 3*yZone;

		switch(zone)
		{
		case 0:
			speed.x = -fabs(speed.x);
			speed.y = -fabs(speed.y);
			break;

		case 1:
		case 7:
			speed.y = -speed.y;
			break;

		case 2:
			speed.x = fabs(speed.x);
			speed.y = -fabs(speed.y);
			break;

		case 3:
		case 5:
			speed.x = -speed.x;
			break;

		case 4:
			//inside the brick!!!!!
			break;

		case 6:
			speed.x = -fabs(speed.x);
			speed.y = fabs(speed.y);
			break;

		case 8:
			speed.x = fabs(speed.x);
			speed.y = fabs(speed.x);
		}
	}

	void Ball::racketCollision(const Actor &actor)
	{
		point2f direction = position - actor.getPosition();
		direction.y = -fabs(direction.y)*2;
		direction.normalise();

		speed.y = -fabs(speed.y);
		speed += direction;
		speed.normalise();
		speed *= maxSpeed;
	}
*/

	void Ball::step()
	{
		position += speed;

		if(position.x + radious >= 1.0f){
			position.x = 1.0f - radious;
			speed.x = -speed.x;
		}
		
		if(position.x - radious <= 0){
			position.x = radious;
			speed.x = -speed.x;
		}

		if(position.y - radious <= 0){
			position.y = radious;
			speed.y = -speed.y;
		}

		if(position.y + radious >= 1){
			position.y = 1 - radious;
			speed.y = -speed.y;
			//std::cout << "game ended: " << std::endl;
		}
	}
}
