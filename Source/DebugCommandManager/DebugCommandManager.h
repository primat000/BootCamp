#pragma once
#include "Dispatcher.h"
#include "Parser.h"
#include "ConsoleCommands.h"
#include <functional> 

class DebugCommandManager
{
public:
	DebugCommandManager();
	~DebugCommandManager();
	void handleConsoleCommand(std::string& ConsoleCommand);
	void addConsoleCommand(const ConsoleCommand& command);

private:
	Parser _parser;
	Token_t _token;
};

