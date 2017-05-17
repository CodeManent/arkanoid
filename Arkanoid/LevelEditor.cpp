#include "LevelEditor.h"
#include "../point2.h"
#include "../console.h"

#include <algorithm>
#include <functional>//std::mem_fun

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include <cstdlib>
#include <gl/glut.h>
namespace Arkanoid
{
	LevelEditor::LevelEditor()
		:redraw(false)
	{
		{
			//add options

			options.reserve(10);

			point2f position(1.8f, 8.0f);
			const point2f optionsDistance(Brick::BRICK_SIZE.x*3.0f/2.0f, 0.0f);

			position += Brick::BRICK_SIZE/2;
			options.push_back( Brick( position, Brick::PINK ) );

			position += optionsDistance;
			options.push_back( Brick( position, Brick::BROWN ) );

			position += optionsDistance;
			options.push_back( Brick( position, Brick::DARK_GRAY ) );

			position += optionsDistance;
			options.push_back( Brick( position, Brick::LIGHT_GRAY ) );

			position += optionsDistance;
			options.push_back( Brick( position, Brick::WHITE ) );

			//2nd row
			position = point2f( 1.8, 9.0f ) + Brick::BRICK_SIZE/2;
			options.push_back( Brick( position, Brick::ORANGE ) );

			position += optionsDistance;
			options.push_back( Brick( position, Brick::RED ) );

			position += optionsDistance;
			options.push_back( Brick( position, Brick::BLUE ) );

			position += optionsDistance;
			options.push_back( Brick( position, Brick::YELLOW ) );

			position += optionsDistance;
			options.push_back( Brick( position, Brick::BRICK_SIZE, zeroPoint3f ) );

			selected = options.begin();
		}
		{
			//add bricks
			point2f brickCount(10.0f, 10.0f);
			point2f bricksPosition(5.0f, 2.5f);
			point2f bricksSize = Brick::BRICK_SIZE * brickCount;

			point2f firstBrickPosition = bricksPosition - bricksSize/2 + Brick::BRICK_SIZE/2;

			for( float y = 0; y < brickCount.y; ++y )
				for( float x = 0; x < brickCount.x; ++x )
				{
					const point2f position = firstBrickPosition + Brick::BRICK_SIZE*(const point2f(x, y));
					bricks.push_back( Brick( position, Brick::BRICK_SIZE, zeroPoint3f) );
				}
		}
	}

	LevelEditor::~LevelEditor()
	{
	}

	bool LevelEditor::step()
	{
		if( redraw)
		{
			redraw = false;
			return true;
		}

		return false;
	}

	void LevelEditor::draw()
	{
		glPushMatrix();

		glLoadIdentity();
		glScalef(1.0f/10.0f, 1.0f/10.0f, 1);

		//draw bricks
		std::for_each( bricks.begin(), bricks.end(), std::mem_fun_ref( &Brick::draw ) );


		//draw options
		std::for_each( options.begin(), options.end(), std::mem_fun_ref( &Brick::draw ) );

		{
			//draw blue border around selected option
			glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
			const point2f upLeft    = selected->getPosition() - selected->getSize()/2 - point2f( 0.1f, 0.1f );
			const point2f downRight = selected->getPosition() + selected->getSize()/2 + point2f( 0.1f, 0.1f );

			glBegin( GL_LINE_LOOP );
				glVertex2f( upLeft.x, upLeft.y );
				glVertex2f( upLeft.x, downRight.y );
				glVertex2f( downRight.x, downRight.y );
				glVertex2f( downRight.x, upLeft.y );
			glEnd();
		}

		glPopMatrix();
	}

	void LevelEditor::input(const Input &input)
	{
		switch(input.type)
		{
		case Input::Back:
			if(input.value.buttonValue == Input::Value::ButtonDown)
				finished = true;
			break;

		case Input::MouseClick:
			if(input.value.buttonValue == Input::Value::ButtonDown)
			{
				//console.write(std::string("LevelEditor: click at ") + input.value.pointerPosition.toString() );
				mouseInteraction( input.value.pointerPosition );
			}
			break;
		default:
			break;
		}
	}



	bool LevelEditor::exec(const std::string command)
	{
		std::istringstream iss(command);
		std::vector<std::string> args;

		std::copy(
			std::istream_iterator<std::string>(iss),
			std::istream_iterator<std::string>(),
			std::back_insert_iterator<std::vector<std::string> >(args)
			);


		std::string &cmd = args[0];

		if(cmd == "resetBoard")
		{
			resetBoard();
			return true;
		}
		else if(cmd== "saveBoard")
		{
			if(args.size() != 2)

				console.write("LevelEditor: saveBoard: bad parameters");
			else
			{
				std::string path( "..\\assets\\Arkanoid\\boards\\");
				path += args[1] + ".brd";

				try{
					saveBoard(path);
				}
				catch( std::exception &e )
				{
					console.write( e.what() );
				}
			}
			return true;
		}
		else if(cmd == "loadBoard")
		{
			if(args.size() != 2)

				console.write("LevelEditor: loadBoard: bad parameters");
			else
			{
				std::string path( "..\\assets\\Arkanoid\\boards\\");
				path += args[1] + ".brd";

				try
				{
					loadBoard(path);
				}
				catch(std::exception &e)
				{
					console.write( e.what() );
				}
			}
			return true;
		}
		else if( cmd == "info" )
		{
			console.write( "LevelEditor" );
			console.write( "|- resetBoard" );
			console.write( "|- saveBoard path" );
			console.write( "|- loadBoard path" );
			console.write( "\\- info" );
		}

		return false;
	}

	bool LevelEditor::pointInBox(const point2f &upLeft, const point2f &downRight, const point2f &pointPosition)
	{
		if(upLeft < pointPosition && pointPosition < downRight)
			return true;
		else
			return false;
	}

	void LevelEditor::mouseInteraction(const point2i &clickPosition)
	{
		//transform window coordinates to world coordinates
		const point2f windowSize( glutGet( GLUT_WINDOW_WIDTH ), glutGet( GLUT_WINDOW_HEIGHT ) );

		const point2f worldPosition(
			10.0f*clickPosition.x/windowSize.x,
			10.0f*clickPosition.y/windowSize.y
			);

		//test options
		std::vector<Brick>::iterator brick;
		for( brick = options.begin(); brick != options.end(); ++brick )
		{
			const point2f upLeft = brick->getPosition() - brick->getSize()/2;
			const point2f downRight = brick->getPosition() + brick->getSize()/2;
			if( pointInBox( upLeft, downRight, worldPosition ) )
			{
				selected = brick;
				redraw = true;
				return;
			}
		}

		//test bricks
		for( brick = bricks.begin(); brick != bricks.end(); ++brick )
		{
			const point2f upLeft = brick->getPosition() - brick->getSize()/2;
			const point2f downRight = brick->getPosition() + brick->getSize()/2;
			if( pointInBox( upLeft, downRight, worldPosition ) )
			{
				*brick = Brick( brick->getPosition(), selected->getTexture() );

				redraw = true;
				return;
			}
		}
	}

	void LevelEditor::resetBoard()
	{
		std::vector<Brick>::iterator brick;
		for( brick = bricks.begin(); brick != bricks.end(); ++brick )
		{
			Brick resettedBrick(brick->getPosition(), Brick::BRICK_SIZE, zeroPoint3f );
			*brick = resettedBrick;
		}
	}

	void LevelEditor::saveBoard(std::string path)
	{
		std::ofstream file;
		file.exceptions( std::ofstream::failbit | std::ofstream::badbit );

		file.open( path.c_str() );

		std::vector<Brick>::const_iterator brick;
		for( brick = bricks.begin(); brick != bricks.end(); ++brick )
		{
			switch( brick->getTexture() )
			{
			case Brick::PINK:		file << "pink ";	break;
			case Brick::BROWN:		file << "brown ";	break;
			case Brick::BLUE:		file << "blue ";	break;
			case Brick::DARK_GRAY:	file << "dark_gray "; break;
			case Brick::LIGHT_GRAY:	file << "light_gray "; break;
			case Brick::WHITE:		file << "white ";	break;
			case Brick::ORANGE:		file << "orange ";	break;
			case Brick::RED:		file << "red ";		break;
			case Brick::YELLOW:		file << "yellow ";	break;
			default:
				file << "empty ";
				break;
			}
		}

		file.close();
	}

	void LevelEditor::loadBoard(std::string filename)
	{
		std::ifstream file;
		file.exceptions( std::ifstream::failbit | std::ifstream::badbit );

		file.open(filename.c_str());

		std::vector<Brick>::iterator brick;
		for( brick = bricks.begin(); brick != bricks.end(); ++brick )
		{
			std::string brickDescription;
			Brick::BrickTexture texture = Brick::DEFAULT;

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
				texture = Brick::DEFAULT;
			else
				throw std::runtime_error( std::string("LevelEditor::loadBoard: Bad entry in the board file: ") + brickDescription );

			*brick = Brick(brick->getPosition(), texture);
		}

		file.close();
		redraw = true;
	}
}
