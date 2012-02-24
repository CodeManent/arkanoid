#pragma once
#include "state.h"

class InitialState :
	public State
{
public:
	InitialState(void);
	virtual ~InitialState(void);

	virtual void step();
	virtual void draw();
	virtual void input(const Input &input);
	virtual bool isActive();
	virtual bool isFinished();
	virtual std::auto_ptr<State> getNextState();
};
