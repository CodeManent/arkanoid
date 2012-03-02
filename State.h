#pragma once
#include "Input.h"

#include <memory>
#include <string>

class State
{
public:
	State(void);
	virtual ~State(void);

	virtual bool step() = 0;
	virtual void draw() = 0;
	virtual void input(const Input &input) = 0;
	virtual bool isActive() = 0;
	virtual bool isFinished() = 0;
	virtual std::auto_ptr<State> getNextState();
	virtual bool exec(std::string);

protected:
	bool active;
	bool finished;
};
