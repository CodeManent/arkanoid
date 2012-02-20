#include "UI.h"
#include "GlutLayer.h"
#include "Input.h"

#include <cstdlib>
#include <gl/glut.h>


//---------------------------------------------------------


UI::UI(int &argc, char *argv[], StateManager &manager)
	:manager(manager)
{
	registerUI(*this);

	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("The Game");

	glutReshapeFunc(::reshape);
	glutDisplayFunc(::display);
	glutKeyboardFunc(::keyboard);
	glutSpecialFunc(::special);
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
	glutSwapBuffers();
}


//---------------------------------------------------------


void UI::keyboard(int key, const point3i &pos)
{
	Input input;

	switch(key){
		case 13:			input = Start;	break;
		case GLUT_KEY_UP:	input = Up;		break;
		case GLUT_KEY_DOWN:	input = Down;	break;
		case GLUT_KEY_LEFT:	input = Left;	break;
		case GLUT_KEY_RIGHT:input = Right;	break;
			//todo: add more input
		default:
			return;
	}
	manager.forwardInput(input);
}


//---------------------------------------------------------


void UI::reshape(const point3i &size)
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
	manager.step();
}


//---------------------------------------------------------


void UI::run()
{
	glutMainLoop();
}
