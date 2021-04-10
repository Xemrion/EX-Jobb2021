#include "Generator.h"
#include <iostream>

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//Test generation and save.
	Environment* env = Generator::generateEnvironment(16, 16, 16, 0, 0.8f);
	env->saveToFile("testEnvironment.txt");
	delete env;

	//Test load and resave.
	env = Environment::readFromFile("testEnvironment.txt");
	env->saveToFile("testEnvironment2.txt");
	delete env;

	std::cin.get();
	return 0;
}