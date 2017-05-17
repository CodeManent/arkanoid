#ifndef Arkanoid_Ball_H
#define Arkanoid_Ball_H

#include "CircleActor.h"
#include "Racket.h"
#include "Brick.h"

#include <vector>

namespace Arkanoid{

	struct collisionData{
		bool collided;
		enum CollisionType{
			POINT,
			PLANE,
		} type;
		point2f point, normal;
		point2f collisionPosition;
		float collisionTime;

		collisionData()
			:collided(false)
		{
		}

		collisionData(const point2f &point, const point2f &collisionPosition, const float &collisionTime)
			:collided(true)
			,type(POINT)
			,point(point)
			,normal(zeroPoint2f)
			,collisionPosition(collisionPosition)
			,collisionTime(collisionTime)
		{
		}

		collisionData(const point2f &point, const point2f &normal, const point2f &position, const float &time)
			:collided(true)
			,type(PLANE)
			,point(point)
			,normal(normal)
			,collisionPosition(position)
			,collisionTime(time)
		{
		}

		collisionData(const collisionData &right)
			:collided(right.collided)
			,type(right.type)
			,point(right.point)
			,normal(right.normal)
			,collisionPosition(right.collisionPosition)
			,collisionTime(right.collisionTime)
		{
		}

		collisionData operator=(const collisionData &right)
		{
			collided = right.collided;
			type = right.type;
			point = right.point;
			normal = right.normal;
			collisionPosition = right.collisionPosition;
			collisionTime = right.collisionTime;

			return *this;
		}
	};

	class Ball: public CircleActor
	{

	public:
		Ball(const point2f &position, const float &radious, const point3f &color = onePoint3f);
		virtual ~Ball();

		virtual void step(std::vector<Brick> &bricks, const Racket &racket);
		void reactToCollision(const collisionData &cdata);
		void reactToPointCollision2(const collisionData &cdata);
		void reactToPointCollision(const collisionData &cdata);
		void reactToRacketCollision(const collisionData &cdata, const Racket& racket);
		void reactToPlaneCollision(const point2f &point, const point2f &normal);

		bool lineCollision(const point2f &startPoint, const point2f &endPoint, float &u) const;

		collisionData spherePlaneSweepTest(const point2f &projectedPosition, const point2f &planePoint, const point2f &planeNormal) const;
		collisionData spherePointSweepTest(const point2f &projectedPosition, const point2f &p) const;
		collisionData sphereBoxSweepTest(const point2f &projectedPosition, const point2f &upleft, const point2f &downright) const;
		collisionData sphereMovingBoxSweepTest(const point2f &projectedPosition, const point2f& boxSpeed, const point2f &upleft, const point2f &downright) const;
		int getVectorSection(const point2f &testVector) const;

		virtual void setSpeedDirection(const point2f& direction);

	protected:
		point2f speed;
		static const float maxSpeed;
	};
}

#endif
