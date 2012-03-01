#include "Ball.h"
#include <cstdlib>
#include <gl/glut.h>
#include <iostream>

#include "RectangleActor.h"
#include <cmath>


namespace Arkanoid{

	const float Ball::maxSpeed = 0.09;
	//point3f ballColor(0.37f, 0.89f, 0.75f)

	Ball::Ball(const point2f &position, const float &radious, const point3f &color)
		:CircleActor(position, radious, color),
		speed(point2f(0.7f, -1.0f).normalise()*maxSpeed)
	{
	}

	Ball::~Ball()
	{
	}

	void Ball::step(std::vector<const point2f> &points, std::vector<Brick*> &bricks, const Racket &racket)
	{
		float timeFrame = 1.0f;
		bool racketCollision = false;

		do{
			const point2f projectedPosition = position + speed*timeFrame;
			collisionData firstCollision;


			//collision with borders
			struct border{
				const point2f &point, &normal;
			} borders[] = {
				{zeroPoint2f, point2f(1.0f, 0.0f)}, //left
				{zeroPoint2f, point2f(0.0f, 1.0f)}, //top
				{onePoint2f*10.0f, point2f(-1.0f, 0.0f)}, //right
				{onePoint2f*10.0f, point2f(0.0f, -1.0f)}  //bottom
			};

			for(unsigned int i = 0; i < 4; ++i){
				const collisionData cdata = spherePlaneSweepTest(projectedPosition, borders[i].point, borders[i].normal);
				if(cdata.collided)
				{
					if((firstCollision.collided && cdata.collisionTime < firstCollision.collisionTime) || (!firstCollision.collided))
					{
						firstCollision = cdata;
					}
				}
			}

			std::vector<const point2f>::const_iterator firstPoint = points.end();
			std::vector<const point2f>::const_iterator point;
			for(point = points.begin(); point != points.end(); ++point)
			{
				const collisionData cdata = spherePointSweepTest(projectedPosition, *point);
				if(cdata.collided)
				{
					if((firstCollision.collided && cdata.collisionTime < firstCollision.collisionTime) || (!firstCollision.collided))
					{
						firstCollision = cdata;
						firstPoint = point;
					}
				}
			}

			//bricks collisions
			std::vector<Brick*>::const_iterator firstBrick = bricks.end();
			std::vector<Brick*>::const_iterator brick;
			for(brick = bricks.begin(); brick != bricks.end(); ++brick)
			{
				const point2f upleft = (*brick)->getPosition() - (*brick)->getSize()/2;
				const point2f downright = (*brick)->getPosition() + (*brick)->getSize()/2;

				const collisionData cdata = sphereBoxSweepTest(projectedPosition, upleft, downright);
				if(cdata.collided)
				{
					if((firstCollision.collided && cdata.collisionTime < firstCollision.collisionTime) || (!firstCollision.collided))
					{
						firstCollision = cdata;
						firstPoint = point;

						firstBrick = brick;
						firstPoint = points.end();
					}
				}
			}

			//racket collision
			//*
			{
				if(!racketCollision)
					//only one racket collision per timeframe
				{
					const point2f upleft = racket.getPosition() - racket.getSize()/2;
					const point2f downright = racket.getPosition() + racket.getSize()/2;

					const collisionData cdata = sphereMovingBoxSweepTest(projectedPosition, racket.getSpeed(), upleft, downright);
					if(cdata.collided)
					{
						if((firstCollision.collided && cdata.collisionTime < firstCollision.collisionTime) || (!firstCollision.collided))
						{
							racketCollision = true;
							firstCollision = cdata;

							firstBrick = bricks.end();
							firstPoint = points.end();
						}
					}
				}
			}
			//*/


			if( firstCollision.collided )
			{
				if(racketCollision)
				{
					reactToRacketCollision(firstCollision, racket);
				}
				else
				{
					reactToCollision(firstCollision);
				}

				position = firstCollision.collisionPosition;
				timeFrame -= firstCollision.collisionTime*timeFrame;

				if( firstPoint != points.end() )
				{
					points.erase(firstPoint);
				}
				else if( firstBrick != bricks.end() )
				{
					(*firstBrick)->suicide();
					bricks.erase(firstBrick);
				}
			}
			else
			{
				//no collisions
				position = projectedPosition;
				timeFrame = 0.0f;
			}
		}while(timeFrame > 0.0f );

		//hard limits
		position.x = ( position.x < radious ) ? radious : ( ( position.x > 10.0f - radious ) ? 10.0f-radious : position.x );
		position.y = ( position.y < radious ) ? radious : ( ( position.y > 10.0f - radious ) ? 10.0f-radious : position.y );

		//fix speed vector lrngth (because of floating point errors)
		speed = speed.normal()*maxSpeed;
	}




	void Ball::reactToCollision(const collisionData &cdata)
	{
		if(cdata.collided)
		{
			switch(cdata.type)
			{
			case collisionData::POINT:
				reactToPointCollision(cdata);
				break;

			case collisionData::PLANE:
				reactToPlaneCollision(cdata.point, cdata.normal);
				break;

			default:
				throw(std::runtime_error("Ball::reactToCollision: Unexpeced collition type"));
			}
		}
	}





	//react to collision with a point
	void Ball::reactToPointCollision2(const collisionData &cdata)
	{
		const point2f &direction = (cdata.collisionPosition - cdata.point).normalise();
		speed = direction*speed.length();
	}






	void Ball::reactToPointCollision(const collisionData &cdata)
	{
		const int section = getVectorSection(cdata.collisionPosition - cdata.point);
		switch(section){
			case 0://up
				speed.y = -fabs(speed.y);
				break;

			case 3://down
				speed.y = fabs(speed.y);
				break;

			case 1://left
				speed.x = -fabs(speed.x);
				break;

			case 2://right
				speed.x = fabs(speed.x);
				break;
		}
	}

	void Ball::reactToRacketCollision(const Arkanoid::collisionData &cdata, const Arkanoid::Racket &racket)
	{
		const point2f p = racket.getPosition() - point2f(0.0f, 0.6f);

		point2f direction = (cdata.collisionPosition - p).normalise();
		direction.y = -fabs(direction.y);

		const point2f oldDirection = point2f(speed.x, -fabs(speed.y)).normal();

		speed = ((direction+oldDirection)/2).normal()*speed.length();
	}


	//react to collision tith a plane
	//http://mathworld.wolfram.com/Reflection.html
	void Ball::reactToPlaneCollision(const point2f &point, const point2f &normal)
	{
		//speed = speed - normal*(speed.dot(normal)*2)/normal.dot(normal));
		//normal.dot(normal) always returns 1
		speed -= normal*(speed.dot(normal)*2);
	}






	//based on http://stackoverflow.com/questions/1073336/circle-line-collision-detection
	bool Ball::lineCollision(const point2f &startPoint, const point2f &endPoint, float &u) const
	{
		const point2f &d = endPoint - startPoint;
		const point2f &f = startPoint - position;
		const float &a = d.dot(d);
		const float &b = 2*f.dot(d);
		const float &c = f.dot(f) - radious*radious;

		float discriminant = b*b-4*a*c;
		if(discriminant < 0.0f){
			return false;
		}
		else if(discriminant == 0.0f){
			//one solution
			const float &t = -b/(2*a);
			if(t > 0 && t <= 1){
				u = t;
				return true;
			}
			else{
				return false;
			}
		}
		else
		{
			// ray didn't totally miss sphere,
			// so there is a solution to
			// the equation.
			discriminant = sqrt(discriminant);
			const float &t1 = (-b+discriminant)/(2*a);
			const float &t2 = (-b-discriminant)/(2*a);

			u = 1.0f;

			if(t1 > 0.0f && t1 <= u){
				//solution is on the ray
				u = t1;
			}
			/*
			else{
				//solution "out of range" of ray
				return false;
			}
			*/

			if(t2 > 0.0f && t2 <= u){
				u = t2;
			}

			if(u != 1.0f){
				return true;
			}
			else{
				return false;
			}
		}
	}




	//http://www.gamasutra.com/view/feature/3383/simple_intersection_tests_for_games.php
	collisionData Ball::spherePlaneSweepTest(const point2f &projectedPosition, const point2f &planePoint, const point2f &planeNormal) const
	{
		collisionData retval;

		const float d0 = (position - planePoint).dot(planeNormal);
		const float d1 = (projectedPosition - planePoint).dot(planeNormal);
		/*
		if(!assumeUncollidedStart && fabs(d0) <= radious){
			return collisionData(planePoint, planeNormal, position, 0.0f);
		}
		else
		//*/
		{
			if(d0 > radious && d1 < radious){
				const float u = (d0-radious)/(d0-d1);
				const point2f collisionPosition = position*(1-u) + projectedPosition*u;

				retval = collisionData(planePoint, planeNormal, collisionPosition, u);
			}
		}
		return retval;
	}





	//it is equivalent with a static ball and a ray starting from p
	collisionData Ball::spherePointSweepTest(const point2f &projectedPosition, const point2f &p) const
	{
		float u;
		const point2f distanceVector = projectedPosition - position;
		const point2f endPoint = p + distanceVector;

		if(lineCollision(p, endPoint, u))
		{
			const float collisionTime= 1-u;

			//const point2f &collisionPosition = (projectedPosition - position)/(projectedPosition - position).length() * collisionTime;
			const point2f collisionPosition = position + distanceVector* collisionTime;

			return collisionData(p, collisionPosition, collisionTime);
		}
		return collisionData();
	}







	collisionData Ball::sphereBoxSweepTest(const point2f &projectedPosition, const point2f &upleft, const point2f &downright) const
	{
		const point2f &upright  = point2f(downright.x, upleft.y);
		const point2f &downleft = point2f(upleft.x, downright.y);

		collisionData retval;



		//check point collisions
		const point2f testPoints[] = {
			upleft,
			upright,
			downleft,
			downright
		};

		//check line collisions
		struct line{
			const point2f p0, p1, &normal;
		};

		const line lines[] = {
			{upleft,   upright,   point2f(0.0f, -1.0f)}, //top
			{upleft,   downleft,  point2f(-1.0f, 0.0f)}, //left
			{upright,  downright, point2f(1.0f,  0.0f)}, //right
			{downleft, downright, point2f(0.0f,  1.0f)}  //bottom
		};

//*
		//check lines
		for(unsigned int i=0; i < 4; ++i)
		{
			float u;
			const point2f startPoint = lines[i].p0 + (position - projectedPosition);
			const point2f endPoint = lines[i].p1 + (position - projectedPosition);


			if(lineCollision(startPoint, endPoint, u))
			{
				const collisionData cdata = spherePlaneSweepTest(projectedPosition, lines[i].p0, lines[i].normal);
				if(cdata.collided)
				{
					if(!retval.collided || (retval.collided && cdata.collisionTime < retval.collisionTime) )
					{
						retval = cdata;
					}
				}
			}
		}
//*/

		//check egdes
		for(unsigned int i = 0; i < 4; ++i){
			const collisionData cdata= spherePointSweepTest(projectedPosition, testPoints[i]);
			if(cdata.collided)
			{
				if((retval.collided && cdata.collisionTime < retval.collisionTime) || (!retval.collided))
				{
					retval = cdata;
				}
			}
		}
		return retval;
	}




	collisionData Ball::sphereMovingBoxSweepTest(const point2f &projectedPosition, const point2f &boxSpeed, const point2f &upleft, const point2f &downright) const
	{
		collisionData cdata = sphereBoxSweepTest(projectedPosition - boxSpeed, upleft, downright);
		if(cdata.collided){
			cdata.collisionPosition += boxSpeed*cdata.collisionTime;
		}

		return cdata;
	}






	/*
		given a vector, it returns the sector it belongs 
		
		0 = up
		1 = left
		2 = right
		3 = down
	*/
	int Ball::getVectorSection(const point2f &testVector) const
	{
		const float pi = 3.1415926535897932384626433832795f;

		float rads = atan2(testVector.y, testVector.x);
		if( -pi/4 <= rads && rads < pi/4 )
		{
			return 2;//right
		}
		else if( pi/4 <= rads && rads < 3*pi/4 )
		{
			return 3; //down
		}
		else if( 3*pi/4 <= rads || rads < -3*pi/4 )
		{
			return 1; //left
		}
		else if( -3*pi/4 <= rads && rads < -pi/4 )
		{
			return 0; //up
		}
		else
		{
			throw( std::runtime_error( "getBoxSection: bad section computation" ) );
		}
	}
}
