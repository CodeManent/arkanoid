#include "Racket.h"


namespace Arkanoid{
	const float Racket::acceleration = 0.0002f;
	const float Racket::maxSpeed = 0.002f;
	const point3f Racket::racketColor(0.974f, 0.157f, 0.214f);

	Racket::Racket(const point2f &position, const point2f &size)
		:RectangleActor(position, size, racketColor),
		speed(0.0f),
		direction(None)
	{
	}

	Racket::~Racket()
	{
	}


	void Racket::move(const Input &input)
	{
		if(input.value.buttonValue == Input::Value::ButtonUp){
			if((direction == Left && input.type == Input::Left) ||
				(direction ==Right && input.type == Input::Right)
				)
			{
				direction = None;
			}
		}
		else
		{
			direction = input.type == Input::Left ? Left : Right;
		}
	}

	void Racket::step()
	{
		switch(direction){
			case Left:
				if(speed > 0){
					speed = -acceleration;
				}
				else{
					speed -= acceleration;
				}

				if(speed < -maxSpeed){
					speed = -maxSpeed;
				}
				break;

			case Right:
				if(speed < 0){
					speed = acceleration;
				}
				else{
					speed += acceleration;
				}

				if(speed > maxSpeed){
					speed = maxSpeed;
				}
				break;

			case None:
			default:

				if(speed != 0.0f){
					const float decceleration = 1.1f*acceleration;
					const float newSpeed = fabs(speed) - decceleration;
					if(newSpeed <= 0.0f){
						speed = 0.0f;
					}
					else{
						speed = speed > 0 ? newSpeed : -newSpeed;
					}
				}
		}

		if(speed != 0.0f){
			position.x += speed;
			if(position.x - size.x/2 < 0.0f){
				speed = 0;
				position.x = size.x/2;
			}
			else if(position.x + size.x/2 > 1.0f){
				speed = 0;
				position.x = 1.0f - size.x/2;
			}
		}

	}
}
