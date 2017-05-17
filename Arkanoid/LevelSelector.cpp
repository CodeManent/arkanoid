#include "LevelSelector.h"
#include "Arkanoid.h"
#include "../Console.h"

#include <fstream>
#include <algorithm>

#include <cstdlib>
#include <gl/glut.h>


namespace Arkanoid
{
	LevelSelector::LevelSelector(std::string path)
		:highlightedLevel(0)
		,selectedLevel(false)
		,redraw(false)
	{
		readLevels(path);
	}

	LevelSelector::~LevelSelector()
	{
	}

	void LevelSelector::draw()
	{
		glPushMatrix();
		glLoadIdentity();

		//fade background
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
		glBegin(GL_QUADS);
			glVertex2f(0.2f, 0.2f);
			glVertex2f(0.2f, 0.8f);
			glVertex2f(0.8f, 0.8f);
			glVertex2f(0.8f, 0.2f);
		glEnd();

		glDisable(GL_BLEND);

		//draw level names
		glEnable(GL_TEXTURE_2D);

		point2f pos(0.25f, 0.25f);
		for(int level = 0; level < levels.size(); ++level)
		{
			if(level == highlightedLevel)
			{
				glColor4f(1.0f, 0.8f, 0.8f, 0.0f);
				glColor3f(1.0f, 1.0f, 1.0f);
			}
			else
			{
				glColor4f(0.5f, 1.0f, 1.0f, 0.0f);
				glColor3f(0.5f, 0.5f, 0.5f);
			}

			glRasterPos2f(pos.x, pos.y);
			std::string::const_iterator ch;
			for(ch = levels[level].begin(); ch != levels[level].end(); ++ch)
			{
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *ch);
			}

			pos.y += 0.05;
		}
		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
	}

	bool LevelSelector::step()
	{
		if(redraw)
		{
			redraw = false;
			return true;
		}
		return true;
	}

	void LevelSelector::input(const Input &input)
	{
		switch(input.type)
		{
		case Input::Back:
			if(input.value.buttonValue == Input::Value::ButtonDown )
				finished = true;
			break;

		case Input::Start:
			if(input.value.buttonValue == Input::Value::ButtonDown )
			{
				finished = true;
				selectedLevel = true;
			}
			break;

		case Input::Up:
			if(input.value.buttonValue == Input::Value::ButtonDown )
			{
				--highlightedLevel;
				highlightedLevel %= levels.size();
				redraw = true;
			}
			break;

		case Input::Down:
			if(input.value.buttonValue == Input::Value::ButtonDown )
			{
				++highlightedLevel;
				highlightedLevel %= levels.size();
				redraw = true;
			}
		}
	}

	void LevelSelector::readLevels(std::string path)
	{
		using namespace std;

		ifstream file;

		file.open(path.c_str());

		while(!file.eof())
		{
			std::string name;
			file >> name;
			if(!file)
			{
				return;
			}
			levels.push_back(name);
		}

		file.close();
	}

	std::auto_ptr<State> LevelSelector::getNextState()
	{
		State *retval = NULL;
		if(selectedLevel)
		{
			retval = new Arkanoid(levels[highlightedLevel]);
		}

		return std::auto_ptr<State>(retval);
	}
}
