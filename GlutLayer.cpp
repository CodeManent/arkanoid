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
		ui->keyboard(key, point3i(x, y), Input::Value::ButtonDown);
	}
}


//---------------------------------------------------------


void keyboardUp(unsigned char key, int x, int y)
{
	if(ui){
		ui->keyboard(key, point3i(x, y), Input::Value::ButtonUp);
	}
}


//---------------------------------------------------------


void special(int key, int x, int y)
{
	if(ui){
		ui->keyboard(key, point3i(x, y), Input::Value::ButtonDown);
	}
}


//---------------------------------------------------------


void specialUp(int key, int x, int y)
{
	if(ui){
		ui->keyboard(key, point3i(x, y), Input::Value::ButtonUp);
	}
}


//---------------------------------------------------------


void reshape(int width, int height)
{
	if(ui){
		ui->reshape(point3i(width, height));
	}
}


//---------------------------------------------------------


void idle(void)
{
	if(ui){
		ui->idle();
	}
}
