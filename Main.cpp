
#include <string>;
#include <iostream>;
#include <stdlib.h>;

#include "GameHandler.cpp";

using namespace std;

int main(int argc, char * argv[]) {

	GameHandler handler;

	bool done = false;
	while (!done)
	{
		handler.humanMove();
	}

	return 0;
}