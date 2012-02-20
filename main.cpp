#include "UI.h"
#include "StateManager.h"
#include "InitialState.h"

#include <memory>

int main(int argc, char*argv[]){
	std::auto_ptr<State> initial(new InitialState());
	StateManager manager(initial);
	UI ui(argc, argv, manager);

	ui.run();

	return 0;
}
