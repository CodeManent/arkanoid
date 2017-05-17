#include "InitialState.h"
#include <cstdlib>
#include <gl/glut.h>

//#include "Arkanoid/LevelEditor.h"
#include "Arkanoid/MainMenu.h"
//#include "Arkanoid/Arkanoid.h"
#include "Arkanoid/WinState.h"
//#include "Feel/Feel.h"

InitialState::InitialState(void)
	:State()
{
	finished = true;
}


//---------------------------------------------------------


InitialState::~InitialState(void)
{
}


//---------------------------------------------------------


bool InitialState::step()
{
	return true;
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
	/*
	if(input == Start){
		finished = true;
	}
	*/
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
	State *nextState = NULL;

	nextState = new 
		Arkanoid::MainMenu()
//		Arkanoid::Arkanoid("first")
//		Arkanoid::LevelEditor()
//		Arkanoid::WinState()
//		Feel()
		;

	return std::auto_ptr<State>(nextState);
}
