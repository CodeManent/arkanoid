#pragma once
#include <memory>
#include "Input.h"

class State
{
public:
	State(void);
	virtual ~State(void);

	virtual void step() = 0;
	virtual void draw() = 0;
	virtual void input(const Input &input) = 0;
	virtual bool isActive() = 0;
	virtual bool isFinished() = 0;
	virtual std::auto_ptr<State> getNextState();

protected:
	bool active;
	bool finished;
};
