#ifndef Arkanoid_LevelEditor_H
#define Arkanoid_LevelEditor_H

#include "../State.h"
#include "Brick.h"

#include <vector>

namespace Arkanoid
{
	class LevelEditor: public State
	{
	public:
		LevelEditor();
		~LevelEditor();

		virtual bool step();
		virtual void draw();
		virtual void input(const Input &input);

		virtual bool exec(std::string);

	private:
		bool pointInBox(const point2f &upLeft, const point2f &downRight, const point2f &pointPosition);
		void mouseInteraction(const point2i &clickPosition);
		void resetBoard();
		void saveBoard(std::string path);
		void loadBoard(std::string filename);

		std::vector<Brick> bricks;

		std::vector<Brick> options;
		std::vector<Brick>::iterator selected;
		bool redraw;
	};
}


#endif
