#include "State.h"


//---------------------------------------------------------


State::State(void)
:active(true), finished(false)
{
}


//---------------------------------------------------------


State::~State(void)
{
}

bool State::isActive()
{
	return active;
}

bool State::isFinished()
{
	return finished;
}


std::auto_ptr<State> State::getNextState()
{
	return std::auto_ptr<State>(NULL);
}


bool State::exec(std::string)
{
	return false;
}
