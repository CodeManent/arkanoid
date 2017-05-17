#include "StateManager.h"
#include "ConsoleState/ConsoleState.h"
#include "ResourceManager.h"
#include "Console.h"
#include "ExitException.h"

#include <memory>
#include <vector>
#include <algorithm>


//---------------------------------------------------------


StateManager::StateManager(std::auto_ptr<State> state)
	:visibleConsole(false)
{
	pushState(state);

}


//---------------------------------------------------------

StateManager::~StateManager(void)
{
	std::for_each(stateStack.begin(), stateStack.end(), stateDeleter);
}


//---------------------------------------------------------


void StateManager::draw()
{
	//animate previous active states
	std::vector<State*>::iterator state;
	for(state = stateStack.begin(); state != stateStack.end() -1; ++state){
		if((*state)->isActive()){
			(*state)->draw();
		}
	}

	//animate current state
	stateStack.back()->draw();
}


//---------------------------------------------------------


bool StateManager::step()
{
	bool redraw = false;

	// push states to the stack
	for( unsigned int pos = 0; pos < stateStack.size(); ++pos )
	{
		std::auto_ptr<State> nextState = stateStack[pos]->getNextState();
		if( nextState.get() )
		{
			redraw = true;

			stateStack.insert(stateStack.begin()+pos+1, nextState.get() );
			nextState.release();
		}
	}


	//remove dead states
	for( unsigned int pos = 0; pos < stateStack.size(); ++pos )
	{
		if( stateStack[pos]->isFinished() )
		{
			redraw = true;

			//console is always at the top of the stack
			if(visibleConsole && pos == stateStack.size() -1)
			{
				visibleConsole = false;
			}
			delete stateStack[pos];
			stateStack.erase(stateStack.begin() + pos);
			--pos;
		}
	}

	//exit if no states left
	if(stateStack.empty()){
		throw ExitException();
	}


	//animate previous active states
	std::vector<State*>::iterator state;
	for(state = stateStack.begin(); state != stateStack.end() -1; ++state){
		if((*state)->isActive()){
			redraw |= (*state)->step();
		}
	}

	//animate top state
	 redraw |= stateStack.back()->step();


	 //play the sounds
	 resourceManager.updatePlayingSounds();

	 return redraw;
}


//---------------------------------------------------------


void StateManager::forwardInput(const Input &input)
{
	if(!visibleConsole)
		if(input.type == Input::Character && input.value.buttonValue == Input::Value::ButtonDown)
			if(input.value.charValue == '`' || input.value.charValue == '~')
			{
				pushState( std::auto_ptr<State>( new ConsoleState(*this) ) );
				visibleConsole = true;
				return;
			}

	stateStack.back()->input(input);
}


//---------------------------------------------------------


void StateManager::pushState(std::auto_ptr<State> state)
{
	if( state.get() != NULL)
	{
		stateStack.push_back(state.get());
		state.release();
	}
}


//---------------------------------------------------------


void StateManager::exec(std::string command)
{
	std::vector<State *>::reverse_iterator state;
	for(state = stateStack.rbegin(); state != stateStack.rend(); ++state)
	{
		if((*state)->exec(command))
			return;
	}
	console.write( std::string("Unrecognised command: ") + command );
}



//---------------------------------------------------------


void StateManager::stateDeleter(State *state)
{
	delete state;
}
