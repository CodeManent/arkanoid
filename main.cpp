#include "UI.h"
#include "StateManager.h"
#include "InitialState.h"

#include <memory>
#include <cstdlib>
#include <ctime>

int main(int argc, char*argv[]){
	std::srand((unsigned int)std::time(NULL));


	std::auto_ptr<State> initial(new InitialState());
	StateManager manager(initial);
	UI ui(argc, argv, manager);

	ui.run();

	return 0;
}
