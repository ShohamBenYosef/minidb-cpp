#include "CommandParser.hpp"

#include <cassert>
#include <iostream>

void testInsert() {
    Command command = parseCommand( "insert 1 David 32");

    assert(command.type == CommandType::Insert);
    assert(command.id == 1);
    assert(command.name == "David");
    assert(command.age == 32);
}

void testInsertCase() {
    Command command = parseCommand("INsERT 2 Dana 24");

    assert(command.type == CommandType::Insert);
    assert(command.id == 2);
    assert(command.name == "Dana");
    assert(command.age == 24);
}

void testFind() {
    Command command = parseCommand("find 10");

    assert(command.type == CommandType::Find);
    assert(command.id == 10);
}

void testFindExtraArgument() {
    Command command = parseCommand("find 10 extra");

    assert(command.type == CommandType::Invalid);

}

void testListExtraArgument() {
    Command command = parseCommand("list extra");

    assert(command.type == CommandType::Invalid);
}

void testUnknownCommand() {
    Command command = parseCommand("unknown");

    assert(command.type == CommandType::Invalid);
}

int main() {
    testInsert();
    testInsertCase();
    testFind();
    testFindExtraArgument();
    testListExtraArgument();
    testUnknownCommand();

    std::cout << "\n\nAll parser tests passed.\n\n";

    return 0;
}