#pragma once
#include <vector>
#include <list>

class Console
{
public:
	static Console& get();
	virtual ~Console(void);
	void show();
	void hide();
	void write(std::string message);
	void draw();
	void clear();

private:
	Console(void);
	bool visible;
	unsigned int maxBufferSize;
	std::list<std::string> buffer;
	static Console *currentConsole;
};

extern Console &console;
