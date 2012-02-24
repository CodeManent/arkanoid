#pragma once
#include "State.h"
#include "Input.h"

#include <vector>
#include <memory>

class StateManager
{
public:
	StateManager(std::auto_ptr<State> state);
	~StateManager(void);

	void draw();
	void step();
	void forwardInput(const Input &i);
	void pushState(std::auto_ptr<State> state);

protected:
	static void stateDeleter(State *state);
	std::vector<State *> stateStack;

};
