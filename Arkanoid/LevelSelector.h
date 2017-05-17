#ifndef Arkanoid_Level_Selector_H
#define Arkanoid_Level_Selector_H

#include "../State.h"
#include <string>
#include <vector>

namespace Arkanoid{
	class LevelSelector: public State
	{
	public:
		LevelSelector(std::string path);
		~LevelSelector();

		virtual bool step();
		virtual void draw();
		virtual void input(const Input &input);
		virtual std::auto_ptr<State> getNextState();

	private:
		void readLevels(std::string path);
		std::vector<std::string> levels;
		int highlightedLevel;
		bool selectedLevel;
		bool redraw;
	};
}
#endif
