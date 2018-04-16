
#include <string>;
#include <iostream>;
#include <stdlib.h>;

#include "TestingHandler.cpp";

using namespace std;

int main(int argc, char * argv[]) {

	TestingHandler handler;

	bool done = false;
	while (!done)
	{
		handler.getMove();
	}

	return 0;
}