#include "UI.h"
#include "GlutLayer.h"
#include "Input.h"

#include "Console.h"

#include <windows.h>// Sleep
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
		static DWORD startTime = glutGet(GLUT_ELAPSED_TIME);
		static unsigned int frames = 0;
		++frames;
		const DWORD currentTime = glutGet(GLUT_ELAPSED_TIME);
		if(currentTime - startTime >= 1000){
			std::cout << frames << " fps" << std::endl;
			
			std::stringstream ss;
			ss << frames << " fps";
			console.write(ss.str());

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

/*
void UI::keyboard(int key, const point2i &pos, Input::Value::ButtonValue buttonValue)
{
	Input input;

	input.value.buttonValue = buttonValue;

	if(buttonValue == Input::Value::ButtonDown)
		std::cout << "key: " << key << "\t" << (char)key << std::endl;

	switch(key){
		case 27:			input.type = Input::Back;	break; // ESC
		case 13:			input.type = Input::Start;	break; // Enter
		case GLUT_KEY_UP:	input.type = Input::Up;		break;
		case GLUT_KEY_DOWN:	input.type = Input::Down;	break;
		case GLUT_KEY_LEFT:	input.type = Input::Left;	break;
		case GLUT_KEY_RIGHT:input.type = Input::Right;	break;
			//todo: add more input

		default:
			input.type = Input::Character;
			input.value.charValue =key;
	}

	manager.forwardInput(input);
}
*/

void UI::keyboard(const Input& input)
{
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


//idle will sleep for the remaining timeframe
void UI::idle()
{
	const float frameTime = 1000.0f/60.0f;
	const int startTime = glutGet(GLUT_ELAPSED_TIME);
	
	bool redraw = manager.step();

	if(redraw)
	{
		glutPostRedisplay();
		//display();
	}

	const DWORD currentTime = glutGet(GLUT_ELAPSED_TIME);
	const float elapsed = (const float)(currentTime - startTime);

	if(elapsed < frameTime){
		Sleep((const int)(frameTime - elapsed)/2);
	}
}


//---------------------------------------------------------


void UI::run()
{
	glutMainLoop();
}
