#include "Ball.h"
#include <cstdlib>
#include <gl/glut.h>
#include <iostream>

namespace Arkanoid{
	const float Ball::maxSpeed = 0.0007;

	Ball::Ball(const point3f &position, const float &radious)
		:Actor(position, point3f(radious, radious, 0), point3f(0.37f, 0.89f, 0.75f)),
		radious(radious),
		speed(point3f(1.0f, -1.5f).normalise()*maxSpeed)
	{
	}

	//assumes that ball is round and all the other actors are square
	//http://hq.scene.ro/blog/read/circle-box-intersection-revised/
	bool Ball::collidesWith(const Actor &actor) const
	{
		const point3f& boxPosition = actor.getPosition();
		const point3f& boxSize = actor.getSize();

		const int xZone = position.x  < (boxPosition.x - boxSize.x/2) ? 0 : 
						(position.x > (boxPosition.x + boxSize.x/2) ? 2 : 1);

		const int yZone = position.y < (boxPosition.y - boxSize.y/2) ? 0 :
						(position.y > (boxPosition.y + boxSize.y/2) ? 2 : 1);

		const int zone = xZone + 3*yZone;
		
		bool collisionDetected = false;
		switch(zone){
			//top and bottom side zones
			//check vertical distance between centers
			case 1:
			case 7:
			{
				float distY = fabs(position.y - boxPosition.y);
				if(distY <= (radious + boxSize.y/2)){
					collisionDetected = true;
				}
				break;
			}


			//left right side zones
			//check horizontal distance between centers
			case 3:
			case 5:
			{
				float distX = fabs(position.x - boxPosition.x);
				if(distX <= (radious + boxSize.x/2)){
					collisionDetected = true;
				}
				break;
			}

			//inside zone, collision for sure
			case 4:
			{
				collisionDetected = true;
				break;
			}

			//corner zone.
			//get the corner and check if inside the circle
			default:
			{
				const float cornerX = (zone == 0 || zone == 6) ? 
					boxPosition.x - boxSize.x/2 : boxPosition.x + boxSize.x/2;

				const float cornerY = (zone == 0 || zone == 2) ?
					boxPosition.y - boxSize.y/2 : boxPosition.y + boxSize.y/2;

				if(position.distance(point3f(cornerX, cornerY)) <= radious){
					collisionDetected = true;
				}

			}
		}
		return collisionDetected;
	}

	void Ball::adjustSpeed(const Actor &actor)
	{
		const point3f& boxPosition = actor.getPosition();
		const point3f& boxSize = actor.getSize();

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
		point3f direction = position - actor.getPosition();
		direction.y = -fabs(direction.y)*2;
		direction.normalise();

		speed.y = -fabs(speed.y);
		speed += direction;
		speed.normalise();
		speed *= maxSpeed;
	}

	void Ball::draw() const
	{
		//TODO: must draw a ball
		glColor3f(color.x, color.y, color.z);
		glBegin(GL_POLYGON);
		glCirclePoints(45);
		glEnd();

		const point3f invColor = point3f(1.0f, 1.0f, 1.0f) - color;
		glColor3f(invColor.x, invColor.y, invColor.z);

		glBegin(GL_LINE_LOOP);
		glCirclePoints(45);
		glEnd();
	}

	void Ball::glCirclePoints(unsigned int segments) const
	{
		//http://slabode.exofire.net/circle_draw.shtml
		float theta = 2 * 3.1415926 / float(segments); 
		float c = cosf(theta);//precalculate the sine and cosine
		float s = sinf(theta);
		float t;

		float x = radious;//we start at angle = 0 
		float y = 0; 
	    
		for(unsigned int ii = 0; ii < segments; ii++) 
		{ 
			glVertex2f(x + position.x, y + position.y);//output vertex 
	        
			//apply the rotation matrix
			t = x;
			x = c * x - s * y;
			y = s * t + c * y;
		} 
	}

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
	}
}
