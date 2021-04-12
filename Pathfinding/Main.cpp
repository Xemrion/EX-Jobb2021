#include <iostream>
#include <fstream>
#include <Generator.h>
#include "AlgorithmAStar.h"

void savePathToFile(const std::vector<Position>& path, const std::string& filename)
{
	std::ofstream outputFile(filename);

	if (outputFile.is_open())
	{
		std::cout << "Saving to file...\n";

		for (const Position& pos : path)
		{
			outputFile << std::to_string(pos.x) << " " << std::to_string(pos.y) << " " << std::to_string(pos.z) << "\n";
		}

		outputFile.close();
		std::cout << "Saved path to: " << filename << "\n";
	}
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	const int SIZE = 32;

	int rng = 0;
	int seed = (int)(&rng);

	Environment* env = Generator::generateEnvironment(SIZE, SIZE, SIZE, seed, 0.66f);
	env->saveToFile("environment.txt");

	std::vector<Position> path;
	Position start(0, SIZE / 2, 0);
	Position end(SIZE - 1, SIZE / 2, SIZE - 1);
	AlgorithmAStar algorithm;

	std::cout << "++ Pathfinding...\n";
	if (algorithm.pathfind(env, start, end, path))
	{
		std::cout << "++ Path found!\n";
		savePathToFile(path, "path.txt");
	}
	else
	{
		std::cout << "++ No path found.\n";
	}

	std::cin.get();
	delete env;
	return 0;
}