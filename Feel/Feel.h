#ifndef FEEL_FEEL_H
#define FEEL_FEEL_H

#include "../State.h"

class Feel: public State
{
public:
	Feel();
	~Feel();

	virtual bool step();
	virtual void draw();
	virtual void input(const Input &input);
	virtual bool isActive();
	virtual bool isFinished();
};


#endif

