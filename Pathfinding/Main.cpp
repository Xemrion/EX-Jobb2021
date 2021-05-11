#include <iostream>
#include <fstream>
#include <chrono>
#include <array>
#include <Generator.h>
#include "Memory.h"
#include "AlgorithmAStar.h"
#include "AlgorithmDijkstras.h"
#include "AlgorithmDijkstras2.h"
#include "AlgorithmBacktrack.h"

//GLOBALS
const float g_percentageVisitable = 0.75f;
const int g_amountPerSize = 5;
const int g_iterations = 5;
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
#ifndef MEMORY_TEST_ACTIVE
	std::string filename = algorithm->getName() + std::string("_time.txt");
#else
	std::string filename = algorithm->getName() + std::string("_memory.txt");
#endif // !MEMORY_TEST_ACTIVE

	std::ofstream outputFile(filename);
	if (!outputFile.is_open())
	{
		std::cout << "Could not create test file: " << filename << "\n";
		return;
	}

	std::vector<Position> path;
	Environment* env = nullptr;

	for (int i = 0; i < static_cast<int>(g_envSizes.size()); i++)
	{
		for (int j = 0; j < g_amountPerSize; j++)
		{
			for (int k = 0; k < g_iterations; k++)
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

#ifndef MEMORY_TEST_ACTIVE
				auto before = std::chrono::system_clock::now();
#else
				Memory::resetMemUsed();
				Memory::recordMemUsed();

				SIZE_T virtualMemBefore, physicalMemBefore;
				Memory::getMemUsed(virtualMemBefore, physicalMemBefore);
#endif // !MEMORY_TEST_ACTIVE

				bool result = algorithm->pathfind(env, start, end, path);

#ifndef MEMORY_TEST_ACTIVE
				auto after = std::chrono::system_clock::now();
				long long time = std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count();
				std::cout << "++ DONE! Took " << time << " ms.\n";
				outputFile << name << "_iter" << (k + 1) << " - " << time << " ms\n";
#else
				SIZE_T virtualMemAfter, physicalMemAfter;
				Memory::getMemUsed(virtualMemAfter, physicalMemAfter);

				SIZE_T virtualUsed = virtualMemAfter - virtualMemBefore;
				SIZE_T physicalUsed = physicalMemAfter - physicalMemBefore;

				std::cout << "++ DONE! Took virtual: " << virtualUsed << " bytes. Took physical: " << physicalUsed << " bytes.\n";
				outputFile << name << "_iter" << (k + 1) << " - Virtual/Physical: " << virtualUsed << " / " << physicalUsed << " bytes\n";
#endif // !MEMORY_TEST_ACTIVE

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
	}

	outputFile.close();
	std::cout << "\nFULL TEST COMPLETE!\n";
}

bool doComparisonTest(Algorithm* algorithm1, Algorithm* algorithm2, int size, int seed, float amountVisitable)
{
	Environment* env = Generator::generateEnvironment(size, size, size, seed, amountVisitable, true);

	std::vector<Position> path1;
	std::vector<Position> path2;
	Position start(0, size / 2, 0);
	Position end(size - 1, size / 2, size - 1);

	std::cout << "+=@ Pathfinding " << algorithm1->getName() << " @=+\n";
	auto before = std::chrono::system_clock::now();

	bool result1 = algorithm1->pathfind(env, start, end, path1);

	auto after = std::chrono::system_clock::now();
	long long time = std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count();
	std::cout << "++ DONE! Took " << time << " ms.\n\n";

	std::cout << "+=@ Pathfinding " << algorithm2->getName() << " @=+\n";
	before = std::chrono::system_clock::now();

	bool result2 = algorithm2->pathfind(env, start, end, path2);

	after = std::chrono::system_clock::now();
	time = std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count();
	std::cout << "++ DONE! Took " << time << " ms.\n\n";

	if (result1 && result2)
	{
		std::cout << "++ Both found a path. Looking good!\n";

		delete env;
		return true;
	}
	else if (!result1 && !result2)
	{
		std::cout << "++ None found a path. Looking good!\n";

		delete env;
		return true;
	}
	
	std::cout << "++ UH OH. RESULT IS NOT THE SAME!\n";
	std::cout << "++ " << algorithm1->getName() << ": " << (result1 ? "Cleared" : "Failed") << "\n";
	std::cout << "++ " << algorithm2->getName() << ": " << (result2 ? "Cleared" : "Failed") << "\n";

	env->saveToFile("environment_mismatch.txt");

	delete env;
	return false;
}

void compareAlgorithmsInfinite(Algorithm* algorithm1, Algorithm* algorithm2)
{
	const int SIZE = 16;
	const float VISITABLE = 75.0f;

	int rng = 0;
	int seed = (int)(&rng);

	while (doComparisonTest(algorithm1, algorithm2, SIZE, seed, VISITABLE))
	{
		seed++;
	}
}

void doSimpleTest(Algorithm* algorithm)
{
	const int SIZE = 16;

	int rng = 0;
	int seed = (int)(&rng);

	//Environment* env = Generator::generateEnvironment(SIZE, SIZE, SIZE, seed, 0.75f);
	Environment* env = Environment::readFromFile("environment16_4.txt");

	std::cout << "\n";
	//env->saveToFile("environment.txt");
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
	AlgorithmDijkstras2 algorithmDijkstras2;
	AlgorithmBacktrack algorithmBacktrack;

	//Memory::recordMemUsed();
	//Memory::printMemUsed();

	//doSimpleTest(&algorithmBacktrack);

	compareAlgorithmsInfinite(&algorithmAstar, &algorithmDijkstras2);

	//createAllTests();

	//doFullTest(&algorithmBacktrack);

	std::cin.get();
	return 0;
}