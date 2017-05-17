#include "Racket.h"
#include "../ResourceManager.h"

#include <cstdlib>
#include <gl/glut.h>

namespace Arkanoid{
	const float Racket::acceleration = 0.02f;
	const float Racket::maxSpeed = 0.2f;
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
			else if(position.x + size.x/2 > 10.0f){
				speed = 0;
				position.x = 10.0f - size.x/2;
			}
		}

	}

	point2f Racket::getSpeed() const
	{
		return speed;
	}

	void Racket::draw() const
	{
		const point2f topleft = position - size/2;
		const point2f bottomright = position + size/2;

		resourceManager.bindTexture("Arkanoid\\arinoid_master.png");

		glEnable(GL_TEXTURE_2D);
		glColor3f(1.0f, 1.0f, 1.0f);

		glBegin(GL_QUADS);
			glTexCoord2i(262, 143);
			glVertex2f(topleft.x, topleft.y);

			glTexCoord2i(262, 153);
			glVertex2f(topleft.x, bottomright.y);

			glTexCoord2i(316, 153);
			glVertex2f(bottomright.x, bottomright.y);

			glTexCoord2i(316, 143);
			glVertex2f(bottomright.x, topleft.y);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	}
}
