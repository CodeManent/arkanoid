#include "Arkanoid.h"

namespace Arkanoid{

	Arkanoid::Arkanoid(void)
	{

	}

	Arkanoid::~Arkanoid(void)
	{
	}

	void Arkanoid::step()
	{
	}

	void Arkanoid::draw()
	{
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
