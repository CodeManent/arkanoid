#include "GlutLayer.h"
#include "UI.h"

#include <GL/glut.h>
#include <stdexcept>

UI *ui = NULL;


//---------------------------------------------------------


void registerUI(UI &uiRef){
	if(ui != NULL){
		throw(std::runtime_error("registerUI: A UI is already registered"));
	}

	ui = &uiRef;
}


//---------------------------------------------------------


void unregisterUI(){
	glutReshapeFunc(NULL);
	glutDisplayFunc(NULL);
	glutKeyboardFunc(NULL);
	glutKeyboardUpFunc(NULL);
	glutSpecialFunc(NULL);
	glutSpecialUpFunc(NULL);
	glutIdleFunc(NULL);
	ui = NULL;
}


//---------------------------------------------------------


void display(void){
	if(ui){
		ui->display();
	}
}


//---------------------------------------------------------


void keyboard(unsigned char key, int x, int y)
{
	if(ui){
		//ui->keyboard(key, point2i(x, y), Input::Value::ButtonDown);
		Input input;
		input.value.buttonValue = Input::Value::ButtonDown;
		switch(key){
			case 27:			input.type = Input::Back;	break; // ESC
			case 13:			input.type = Input::Start;	break; // Enter
				//todo: add more input

			default:
				input.type = Input::Character;
				input.value.charValue =key;
		}

		ui->keyboard(input);
	}
}


//---------------------------------------------------------


void keyboardUp(unsigned char key, int x, int y)
{
	if(ui){
		//ui->keyboard(key, point2i(x, y), Input::Value::ButtonUp);
		Input input;
		input.value.buttonValue = Input::Value::ButtonUp;
		switch(key){
			case 27:			input.type = Input::Back;	break; // ESC
			case 13:			input.type = Input::Start;	break; // Enter
				//todo: add more input

			default:
				input.type = Input::Character;
				input.value.charValue =key;
		}

		ui->keyboard(input);
	}
}


//---------------------------------------------------------


void special(int key, int x, int y)
{
	if(ui){
		//ui->keyboard(key, point2i(x, y), Input::Value::ButtonDown);
		Input input;
		input.value.buttonValue = Input::Value::ButtonDown;

		switch(key){
			case GLUT_KEY_UP:	input.type = Input::Up;		break;
			case GLUT_KEY_DOWN:	input.type = Input::Down;	break;
			case GLUT_KEY_LEFT:	input.type = Input::Left;	break;
			case GLUT_KEY_RIGHT:input.type = Input::Right;	break;
				//todo: add more input`
		}
		ui->keyboard(input);
	}
}


//---------------------------------------------------------


void specialUp(int key, int x, int y)
{
	if(ui){
		//ui->keyboard(key, point2i(x, y), Input::Value::ButtonDoUp);
		Input input;
		input.value.buttonValue = Input::Value::ButtonUp;

		switch(key){
			case GLUT_KEY_UP:	input.type = Input::Up;		break;
			case GLUT_KEY_DOWN:	input.type = Input::Down;	break;
			case GLUT_KEY_LEFT:	input.type = Input::Left;	break;
			case GLUT_KEY_RIGHT:input.type = Input::Right;	break;
				//todo: add more input
		}
		ui->keyboard(input);
	}
}


//---------------------------------------------------------


void reshape(int width, int height)
{
	if(ui){
		ui->reshape(point2i(width, height));
	}
}


//---------------------------------------------------------


void idle(void)
{
	if(ui){
		ui->idle();
	}
}
