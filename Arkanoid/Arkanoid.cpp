#include "Arkanoid.h"
#include "WinState.h"
#include "../Console.h"
#include "../ResourceManager.h"

#include "../Feel/Feel.h"

#include <algorithm>
#include <functional>
#include <fstream>

#include <cstdlib>
#include <gl/glut.h>
//#include <iostream>

namespace Arkanoid{

	/*
		Preload resources
		Construct game objects
		Position actors
	*/
	//board size 10.0f, 10.0f
	Arkanoid::Arkanoid(std::string board)
		:ball(NULL)
		,racket(NULL)
		,paused(true)
		,lockedBall(true)
		,lives(3)
		,boardName(board)
		,win(true)
	{
		console.write(std::string("Arkanoid: start ") + board );
		
		//preload textures and sounds
		resourceManager.loadTexture( "Arkanoid\\arinoid_master.png" );

		resourceManager.loadSound( "Arkanoid\\ogg\\Ark_Game_Start_Music.ogg" );
		resourceManager.loadSound( "Arkanoid\\ogg\\Ark_die.ogg" );
		resourceManager.loadSound( "Arkanoid\\ogg\\Ark_release.ogg" );
		resourceManager.loadSound( "Arkanoid\\ogg\\Ark_Bounce_2.ogg" );


		const point2f racketPosition(5.0f, 9.5f);
		const point2f racketSize(1.3f, 0.2f);
		racket.reset(new Racket(racketPosition, racketSize));

		{
			const float ballRadious = 0.1f;
			const point2f ballPosition(racketPosition.x, racketPosition.y - racketSize.y - ballRadious);
			const point3f ballColor(0.7f, 0.1f, 0.5f);
			ball.reset(new Ball(ballPosition, ballRadious, ballColor));
		}

		std::string path("..\\assets\\Arkanoid\\boards\\");
		path += board + ".brd";

		loadBoard(path);


		resourceManager.playSound( "Arkanoid\\ogg\\Ark_Game_Start_Music.ogg" );
	}

	Arkanoid::Arkanoid(void)
		:ball(NULL)
		,racket(NULL)
		,paused(true)
		,lockedBall(true)
		,lives(3)
		,boardName("default")
		,win(true)
	{
		console.write("Arkanoid: start default");

		//preload textures and sounds
		resourceManager.loadTexture( "Arkanoid\\arinoid_master.png" );

		resourceManager.loadSound( "Arkanoid\\ogg\\Ark_Game_Start_Music.ogg" );
		resourceManager.loadSound( "Arkanoid\\ogg\\Ark_die.ogg" );
		resourceManager.loadSound( "Arkanoid\\ogg\\Ark_release.ogg" );
		resourceManager.loadSound( "Arkanoid\\ogg\\Ark_Bounce_2.ogg" );


		const point2f racketPosition(5.0f, 9.5f);
		const point2f racketSize(1.3f, 0.2f);
		racket.reset(new Racket(racketPosition, racketSize));


		{
			const float ballRadious = 0.1f;
			const point2f ballPosition(racketPosition.x, racketPosition.y - racketSize.y - ballRadious);
			const point3f ballColor(0.7f, 0.1f, 0.5f);
			ball.reset(new Ball(ballPosition, ballRadious, ballColor));
		}

		{
			point2f brickCount(10.0f, 10.0f);
			point2f bricksPosition(5.0f, 2.5f);
			point2f bricksSize(9.0f, 4.0f);

			point2f brickSize = bricksSize/brickCount;
			point2f firstBrickPosition = bricksPosition - bricksSize/2 + brickSize/2;
			for(float y = 0; y < brickCount.y; ++y){
				for(float x = 0; x < brickCount.x; ++x){
					const point2f position = firstBrickPosition + brickSize*(const point2f(x, y));

					const point3f color = point3f(x/brickCount.x, y/brickCount.y, (x+y)/(brickCount.x + brickCount.y));

					bricks.push_back(Brick(position, brickSize, color));
				}
			}
		}
		resourceManager.playSound( "Arkanoid\\ogg\\Ark_Game_Start_Music.ogg" );
	}






	Arkanoid::~Arkanoid(void)
	{
		using std::for_each;
		using std::mem_fun;

		//for_each(bricks.begin(), bricks.end(), mem_fun(&Actor::suicide));
	}






	bool Arkanoid::step()
	{
		
		//move racket
		racket->step();

		//move ball
		if(!paused)
			ball->step(bricks, *racket.get());
		else if(lockedBall)
		{
			ball->setPosition( racket->getPosition() - point2f( 0.0f, racket->getSize().y + ball->getRadious() ) );
			ball->setSpeedDirection(point2f(0.7f, -1.0f));
		}
		
		if(ball->getPosition().y > 10.0f)
		{
			paused = true;
			lockedBall = true;
			--lives;
			if(lives == 0){
				finished = true;
				win = false;
			}
			console.write("Life lost");
			resourceManager.playSound( "Arkanoid\\ogg\\Ark_die.ogg" );
		}


		if(bricks.empty())
		{
			finished = true;
		}

		return true;//redraw
	}






	void Arkanoid::draw()
	{
		glLoadIdentity();
		glScalef(1.0f/10.0f, 1.0f/10.0f, 1.0f/10.0f);

		glEnable(GL_TEXTURE_2D);
		resourceManager.bindTexture("Arkanoid\\arinoid_master.png");

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		//draw background
		glPushMatrix();
			glScalef(1.0f/2.5f, 1.0f/2.5f, 1.0f);
			glBegin(GL_QUADS);
			for(unsigned int y = 0; y < 25; ++y)
				for(unsigned int x = 0; x < 25; ++x)
				{
					glTexCoord2i(162, 322);
					glVertex2i(x, y);

					glTexCoord2i(162, 351);
					glVertex2i(x, y+1);

					glTexCoord2i(191, 351);
					glVertex2i(x+1, y+1);

					glTexCoord2i(191, 322);
					glVertex2i(x+1, y);
				}
			glEnd();
		glPopMatrix();

		//draw lives
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		for(unsigned int live = 0; live+1 < lives; ++live)
		{
			glTexCoord2i(425, 141);
			glVertex2f(0.5f+live*0.4, 9.7f);
			glTexCoord2i(425, 149);
			glVertex2f(0.5f+live*0.4, 9.8f);
			glTexCoord2i(442, 149);
			glVertex2f(0.8f+live*0.4, 9.8f);
			glTexCoord2i(442, 141);
			glVertex2f(0.8f+live*0.4, 9.7f);
		}
		glEnd();

		glDisable(GL_TEXTURE_2D);

//*

		std::for_each(bricks.begin(), bricks.end(), std::mem_fun_ref(&Brick::draw));
//*/
		ball->draw();
		racket->draw();
	}






	void Arkanoid::input(const Input &input)
	{
		switch(input.type){
			case Input::Left:
				racket->move(input);
				break;

			case Input::Right:
				racket->move(input);
				break;

			case Input::Back:
				if(input.value.buttonValue == Input::Value::ButtonDown)
					win = false;
					finished = true;
				break;

			case Input::Start:
				if(input.value.buttonValue == Input::Value::ButtonDown)
				{
					paused = !paused;
					lockedBall = false;
				}

				break;

			default:
				break;
		}
	}




	std::auto_ptr<State> Arkanoid::getNextState()
	{
		State *nextState = NULL;
		if( finished )
		{
			if( win )
			{
				if( boardName == "first" )
					nextState = new Arkanoid("half");
				else if( boardName == "half" )
					nextState = new Arkanoid("heart");
				else if( boardName == "heart")
					nextState = new WinState();
			}
			else
			{
				nextState = new Feel();
			}
		}
		return std::auto_ptr<State>(nextState);
	}

	void Arkanoid::loadBoard(std::string path)
	{
		std::ifstream file;
		file.exceptions( std::ifstream::failbit | std::ifstream::badbit );

		file.open(path.c_str());

		bricks.clear();

		//add bricks
		point2f brickCount(10.0f, 10.0f);
		point2f bricksPosition(5.0f, 2.5f);
		point2f bricksSize = Brick::BRICK_SIZE * brickCount;

		point2f firstBrickPosition = bricksPosition - bricksSize/2 + Brick::BRICK_SIZE/2;

		for( float y = 0; y < brickCount.y; ++y )
		{
			for( float x = 0; x < brickCount.x; ++x )
			{
				std::string brickDescription;
				Brick::BrickTexture texture;

				file >> brickDescription;
				if( brickDescription == "pink" )
					texture = Brick::PINK;
				else if( brickDescription == "brown" )
					texture = Brick::BROWN;
				else if( brickDescription == "blue" )
					texture = Brick::BLUE;
				else if( brickDescription == "dark_gray" )
					texture = Brick::DARK_GRAY;
				else if( brickDescription == "light_gray" )
					texture = Brick::LIGHT_GRAY;
				else if( brickDescription == "white" )
					texture = Brick::WHITE;
				else if( brickDescription == "orange" )
					texture = Brick::ORANGE;
				else if( brickDescription == "red" )
					texture = Brick::RED;
				else if( brickDescription == "yellow" )
					texture = Brick::YELLOW;
				else if( brickDescription == "empty" )
					//texture = Brick::DEFAULT;
					continue;
				else
					throw std::runtime_error( std::string("Arkanoid::loadBoard: Bad entry in the board file: ") + brickDescription );

				const point2f position = firstBrickPosition + Brick::BRICK_SIZE*(const point2f(x, y));
				//bricks.push_back( Brick( position, Brick::BRICK_SIZE, zeroPoint3f) );
				bricks.push_back( Brick( position, texture ) );
			}
		}
		file.close();
	}
}
