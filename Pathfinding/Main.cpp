#include <iostream>
#include <fstream>
#include <chrono>
#include <array>
#include <Generator.h>
#include "Memory.h"
#include "AlgorithmAStar.h"
#include "AlgorithmDijkstras.h"

//GLOBALS
const float g_percentageVisitable = 0.75f;
const int g_amountPerSize = 5;
const std::array<int, 3> g_envSizes = { 16, 32, 64 };
//GLOBALS

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

void createEnvironmentSet(int size, float percentageVisitable, int amount)
{
	bool result = false;
	int seed = 0;

	Position start(0, size / 2, 0);
	Position end(size - 1, size / 2, size - 1);
	std::vector<Position> path;
	AlgorithmAStar algorithm;
	Environment* env = nullptr;

	for (int i = 0; i < amount; i++)
	{
		result = false;

		while (!result)
		{
			path.clear();

			env = Generator::generateEnvironment(size, size, size, seed, percentageVisitable, true);
			result = algorithm.pathfind(env, start, end, path);

			if (result)
			{
				env->saveToFile("environment" + std::to_string(size) + "_" + std::to_string(i) + ".txt");
			}

			delete env;
			seed += 1000;
		}
	}
}

void createAllTests()
{
	//Create environments.
	for (int i = 0; i < static_cast<int>(g_envSizes.size()); i++)
	{
		createEnvironmentSet(g_envSizes[i], g_percentageVisitable, g_amountPerSize);
	}

	std::cout << "\nALL TESTS CREATED!\n";
}

void doFullTest(Algorithm* algorithm)
{
	std::vector<Position> path;
	Environment* env = nullptr;

	for (int i = 0; i < static_cast<int>(g_envSizes.size()); i++)
	{
		for (int j = 0; j < g_amountPerSize; j++)
		{
			std::string name = std::to_string(g_envSizes[i]) + "_" + std::to_string(j);
			env = Environment::readFromFile("environment" + name + ".txt");

			if (!env)
			{
				std::cout << "Could not load: environment" << name << "\n";
				continue;
			}

			Position start(0, env->getSizeY() / 2, 0);
			Position end(env->getSizeX() - 1, env->getSizeY() / 2, env->getSizeZ() - 1);
			path.clear();

			std::cout << "+=@ Pathfinding @=+\n";
			auto before = std::chrono::system_clock::now();

			bool result = algorithm->pathfind(env, start, end, path);

			auto after = std::chrono::system_clock::now();
			long long time = std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count();
			std::cout << "++ DONE! Took " << time << " ms.\n";

			if (result)
			{
				savePathToFile(path, "path" + name + ".txt");
			}
			else
			{
				std::cout << "++ ERROR: No path found.\n";
			}

			std::cout << "\n";
			delete env;
		}
	}

	std::cout << "\nFULL TEST COMPLETE!\n";
}

void doSimpleTest(Algorithm* algorithm)
{
	const int SIZE = 16;

	int rng = 0;
	int seed = (int)(&rng);

	Environment* env = Generator::generateEnvironment(SIZE, SIZE, SIZE, seed, 1.0f);

	std::cout << "\n";
	env->saveToFile("environment.txt");
	std::cout << "\n";

	std::vector<Position> path;
	Position start(0, SIZE / 2, 0);
	Position end(SIZE - 1, SIZE / 2, SIZE - 1);

	std::cout << "+=@ Pathfinding @=+\n";
	auto before = std::chrono::system_clock::now();

	bool result = algorithm->pathfind(env, start, end, path);

	auto after = std::chrono::system_clock::now();
	long long time = std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count();
	std::cout << "++ DONE! Took " << time << " ms.\n\n";

	if (result)
	{
		savePathToFile(path, "path.txt");
	}
	else
	{
		std::cout << "++ No path found.\n";
	}

	delete env;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	AlgorithmAStar algorithmAstar;
	AlgorithmDijkstras algorithmDijkstras;

	Memory::recordMemUsed();
	Memory::printMemUsed();

	//doSimpleTest(&algorithmDijkstras);

	//createAllTests();

	//doFullTest(&algorithmAstar);

	std::cin.get();
	return 0;
}