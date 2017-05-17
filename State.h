#ifndef State_H
#define State_H

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
	virtual bool isActive();
	virtual bool isFinished();
	virtual std::auto_ptr<State> getNextState();
	virtual bool exec(std::string);

protected:
	bool active;
	bool finished;
};

#endif
