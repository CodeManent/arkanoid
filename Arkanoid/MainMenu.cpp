#include "MainMenu.h"
#include "Arkanoid.h"
#include "LevelEditor.h"
#include "LevelSelector.h"
#include "../ExitException.h"
#include "../console.h"
#include "../ResourceManager.h"

#include <cstdlib>
#include <gl/glut.h>

namespace Arkanoid{
	MainMenu::MainMenu()
		:selectedOption(0)
		,buttonPressed(false)
		,redisplay(false)
	{
		options.push_back("Start");
		options.push_back("Play level");
		options.push_back("Level editor");
		options.push_back("Exit");

		resourceManager.playSound("Arkanoid\\ogg\\Ark_Intro_Music.ogg", true);
	}

	MainMenu::~MainMenu()
	{
	}

	bool MainMenu::step()
	{
		if(redisplay)
		{
			redisplay = false;
			return true;
		}

		if(active == false)
		{
			active = true;
			return true;
			//returned back to main menu
		}
		return false;
	}

	void MainMenu::draw()
	{
		glPushMatrix();
		glLoadIdentity();
		
		//glEnable(GL_TEXTURE_2D);

		glColor4f(0.2f, 0.3f, 0.5f, 0.1f);

		glBegin(GL_QUADS);

		glVertex2i(0, 0);
		glVertex2i(0, 1);
		glVertex2i(1, 1);
		glVertex2i(1, 0);

		glEnd();

		//glDisable(GL_TEXTURE_2D);


		glRasterPos2f(0.6f, 0.6f);
		point2f optionPos(0.6f, 0.6f);

		for(unsigned int i = 0; i < options.size(); ++i)
		{
			if(selectedOption == i)
				glColor3f(1.0f, 0.2f, 0.2f);
			else
				glColor3f(0.5f, 0.5f, 0.0f);

			glRasterPos2f(optionPos.x, optionPos.y);

			std::string &msg = options[i];
			for( unsigned int msgPos = 0; msgPos < msg.size(); ++msgPos )
			{
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 , msg[msgPos]);
			}

			optionPos.y += 0.05f;
		}


		glPopMatrix();
	}

	void MainMenu::input(const Input &input)
	{
		switch(input.type)
		{
		case Input::Start:
			if(input.value.buttonValue == Input::Value::ButtonDown)
			{
				buttonPressed = true;
				active = false;
			}
			break;

		case Input::Back:
			if(input.value.buttonValue == Input::Value::ButtonDown)
			{
				finished = true;
				active = false;
			}
			break;

		case Input::Up:
			if(input.value.buttonValue == Input::Value::ButtonDown)
			{
				--selectedOption;
				selectedOption %= options.size();
				redisplay = true;
			}
			break;

		case Input::Down:
			if(input.value.buttonValue == Input::Value::ButtonDown)
			{
				++selectedOption;
				selectedOption %= options.size();
				redisplay = true;
			}
			break;
		}
	}

	std::auto_ptr<State> MainMenu::getNextState()
	{
		State *retState = NULL;

		if(buttonPressed)
		{
			buttonPressed = false;
			switch(selectedOption)
			{
			case 0://PLAY
				retState = new Arkanoid("first");
				break;

			case 1://lvl selector
				try{
					retState = new LevelSelector("..\\assets\\Arkanoid\\boards\\list");
					active = true;
				}catch(std::exception &e){
					console.write(e.what());
				}
				break;

			case 2://editor
				retState = new LevelEditor();
				break;

			case 3://exit
				throw ExitException();

			default:
				break;
			}
		}

		return std::auto_ptr<State>(retState);
	}
}
