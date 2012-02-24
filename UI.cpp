#include "UI.h"
#include "GlutLayer.h"
#include "Input.h"

#include <windows.h>// DWORD, GetTickCount, Sleep
#include <iostream>
#include <cstdlib>
#include <gl/glut.h>

//---------------------------------------------------------


UI::UI(int &argc, char *argv[], StateManager &manager)
	:manager(manager),
	windowSize(800, 600),
	showFps(true)
{
	registerUI(*this);

	glutInit(&argc, argv);
	glutInitWindowSize(windowSize.x, windowSize.y);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("The Game");

	glutReshapeFunc(::reshape);
	glutDisplayFunc(::display);
	glutKeyboardFunc(::keyboard);
	glutKeyboardUpFunc(::keyboardUp);
	glutSpecialFunc(::special);
	glutSpecialUpFunc(::specialUp);
	glutIdleFunc(::idle);
}


//---------------------------------------------------------


UI::~UI(void)
{
	unregisterUI();
}


//---------------------------------------------------------


void UI::display(){

	manager.draw();

	if(showFps)
	{
		static DWORD startTime = GetTickCount();
		static unsigned int frames = 0;
		++frames;
		const DWORD currentTime = GetTickCount();
		if(currentTime - startTime >= 1000){
			std::cout << frames << " fps" << std::endl;
			startTime = currentTime;
			frames = 0;
		}
	}

	{
		GLenum err;
		unsigned int errCount = 0;

		while((err = glGetError()) != GL_NO_ERROR){
			std::cout << "GL error: " << gluErrorString(err) << std::endl;
			if(++errCount > 100){
				std::cout << "Too many GL Errors" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}

	glutSwapBuffers();
}


//---------------------------------------------------------


void UI::keyboard(int key, const point2i &pos, Input::Value::ButtonValue buttonValue)
{
	Input input;

	switch(key){
		case 27:/*Esc*/		input.type = Input::Back;	break;
		case 13:/*Enter*/	input.type = Input::Start;	break;
		case GLUT_KEY_UP:	input.type = Input::Up;		break;
		case GLUT_KEY_DOWN:	input.type = Input::Down;	break;
		case GLUT_KEY_LEFT:	input.type = Input::Left;	break;
		case GLUT_KEY_RIGHT:input.type = Input::Right;	break;
			//todo: add more input
		default:
			return;
	}
	input.value.buttonValue = buttonValue;

	manager.forwardInput(input);
}


//---------------------------------------------------------


void UI::reshape(const point2i &size)
{
	windowSize = size;

	glViewport(0, 0, size.x, size.y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	
	glutPostRedisplay();
}


//---------------------------------------------------------


void UI::idle()
{
	bool redraw = manager.step();

	if(redraw)
	{
		glutPostRedisplay();
	}

	Sleep(0);
}


//---------------------------------------------------------


void UI::run()
{
	glutMainLoop();
}
