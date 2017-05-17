#ifndef Arkanoid_MainMenu_H
#define Arkanois_MainMenu_H

#include "../State.h"

#include <vector>
#include <string>

namespace Arkanoid
{
	class MainMenu: public State
	{
	public:
		MainMenu();
		~MainMenu();

		virtual bool step();
		virtual void draw();
		virtual void input(const Input &input);

		virtual std::auto_ptr<State> getNextState();
	private:
		std::vector<std::string> options;
		int selectedOption;
		bool buttonPressed;
		bool redisplay;
	};
}

#endif