#ifndef COMMAND_EXCUTOR_HPP
#define COMMAND_EXCUTOR_HPP

#include "CommandParser.hpp"
#include "StorageEngine.hpp"


bool executeCommand(const Command& command, StorageEngine& storage);



#endif