#include "Arkanoid.h"

namespace Arkanoid{

	Arkanoid::Arkanoid(void)
	{
		this->finished = false;
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
	}

	bool Arkanoid::isActive(){
		return active;
	}

	bool Arkanoid::isFinished()
	{
		return finished;
	}
}