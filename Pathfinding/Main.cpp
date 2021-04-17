#include <iostream>
#include <fstream>
#include <chrono>
#include <Generator.h>
#include "AlgorithmAStar.h"
#include "AlgorithmDijkstras.h"

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
	const int SIZE = 16;

	int rng = 0;
	int seed = (int)(&rng);

	Environment* env = Generator::generateEnvironment(SIZE, SIZE, SIZE, seed, 1.0f);

	std::cout << "\n";
	env->saveToFile("environment.txt");
	std::cout << "\n";

	std::vector<Position> path;
	Position start(0, SIZE / 2, 0);
	Position end(SIZE - 7, SIZE - 3, SIZE - 1);
	AlgorithmAStar algorithm;
	AlgorithmDijkstras algorithm2;

	std::cout << "+=@ Pathfinding @=+\n";
	auto before = std::chrono::system_clock::now();

	bool result = algorithm2.pathfind(env, start, end, path);

	auto after = std::chrono::system_clock::now();
	long long time = std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count();
	std::cout << "++ DONE! Took " << time << " ms.\n\n";
	//C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\CommonExtensions\Microsoft\TeamFoundation\Team Explorer\Git\mingw32\libexec\git-core
	//c/program files (x86)/microsoft visual studio/2019/community/common7/ide/commonextensions/microsoft/teamfoundation/team explorer/Git/mingw32/libexec/git-core/git-askpass.exe
	if (result)
	{
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