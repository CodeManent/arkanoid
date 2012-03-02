#include "StateManager.h"
#include "ConsoleState/ConsoleState.h"

#include <memory>
#include <vector>
#include <algorithm>


//---------------------------------------------------------


StateManager::StateManager(std::auto_ptr<State> state)
	:visibleConsole(false)
{
	stateStack.push_back(state.get());
	state.release();
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

	//remove finished states
	while(!stateStack.empty() && stateStack.back()->isFinished()){
		//get next possible state
		std::auto_ptr<State> substitute = stateStack.back()->getNextState();

		//remove top state from the stack
		stateDeleter( stateStack.back());
		stateStack.pop_back();
		visibleConsole = false;

		//push the substitute state if there is one
		if(substitute.get()){
			pushState(substitute);
			break;
		}
	}

	//exit if no states left
	if(stateStack.empty()){
		exit(EXIT_SUCCESS);
	}


	//animate previous active states
	std::vector<State*>::iterator state;
	for(state = stateStack.begin(); state != stateStack.end() -1; ++state){
		if((*state)->isActive()){
			redraw |= (*state)->step();
		}
	}

	//animate current state
	 redraw |= stateStack.back()->step();

	 return redraw;
}


//---------------------------------------------------------


void StateManager::forwardInput(const Input &input)
{
	if(!visibleConsole)
		if(input.type == Input::Character && input.value.buttonValue == Input::Value::ButtonDown)
			if(input.value.charValue == '`' || input.value.charValue == '~')
			{
				pushState( std::auto_ptr<State>( new ConsoleState() ) );
				visibleConsole = true;
				return;
			}

	stateStack.back()->input(input);
}


//---------------------------------------------------------


void StateManager::pushState(std::auto_ptr<State> state)
{
	stateStack.push_back(state.get());
	state.release();
}


//---------------------------------------------------------


void StateManager::exec(std::string command)
{
	std::vector<State *>::reverse_iterator state;
	for(state = stateStack.rbegin(); state != stateStack.rend(); ++state)
	{
		if((*state)->exec(command))
			break;
	}

}



//---------------------------------------------------------


void StateManager::stateDeleter(State *state)
{
	delete state;
}
