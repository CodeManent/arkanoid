#include "InitialState.h"
#include <cstdlib>
#include <windows.h>
#include <gl/gl.h>

InitialState::InitialState(void)
	:State()
{
}


//---------------------------------------------------------


InitialState::~InitialState(void)
{
}


//---------------------------------------------------------


void InitialState::step()
{
}


//---------------------------------------------------------


void InitialState::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0.7f, 0.5f, 0.3f);

	glBegin(GL_QUADS);
	glVertex2i(-1, 1);
	glVertex2f(-1, -1);
	glVertex2f(1, -1);
	glVertex2f(1, 1);
	glEnd();
}


//---------------------------------------------------------


void InitialState::input(const Input &input)
{
	if(input == Start){
		finished = true;
	}
}


//---------------------------------------------------------


bool InitialState::isActive()
{
	return false;
}


//---------------------------------------------------------


bool InitialState::isFinished()
{
	return finished;
}


//---------------------------------------------------------


std::auto_ptr<State> InitialState::getNextState()
{
	return std::auto_ptr<State>(NULL);
}
