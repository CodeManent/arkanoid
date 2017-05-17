#ifndef UI_H
#define UI_H

#include "StateManager.h"
#include "point2.h"

class UI
{
public:
	UI(int &argc, char* argv[], StateManager &manager);
	~UI(void);

	void display(void);
	//void keyboard(int key, const point2i &pos, Input::Value::ButtonValue buttonValue);
	void input(const Input &input);
	void reshape(const point2i &size);
	void idle(void);
	void run();

protected:
	StateManager &manager;
	point2i windowSize;
	bool showFps;
	int windowId;
};

#endif
