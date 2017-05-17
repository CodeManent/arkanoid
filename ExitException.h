#include <cstdlib>
#include <stdexcept>

class ExitException: public std::runtime_error
{
public:
	ExitException()
		:std::runtime_error("exit")
		,exitCode(EXIT_SUCCESS)
	{
	}
	ExitException(int value)
		:std::runtime_error("exit")
		,exitCode(value)
	{
	}

	~ExitException()
	{
	}

	int exitCode;
};
