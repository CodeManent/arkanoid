#pragma once

#include "StateManager.h"
#include "point3.h"

class UI
{
	StateManager &manager;
	point3i windowSize;
public:
	UI(int &argc, char* argv[], StateManager &manager);
	~UI(void);

	void display(void);
	void keyboard(int key, const point3i &pos, Input::Value::ButtonValue buttonValue);
	void reshape(const point3i &size);
	void idle(void);
	void run();
};
