#ifndef COMMAND_EXECUTOR_HPP
#define COMMAND_EXECUTOR_HPP

#include "CommandParser.hpp"
#include "StorageEngine.hpp"


bool executeCommand(const Command& command, StorageEngine& storage);



#endif