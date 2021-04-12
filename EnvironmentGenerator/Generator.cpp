#include "Generator.h"
#include <random>
#include <iostream>
#include <chrono>

Environment* Generator::generateEnvironment(int xSize, int ySize, int zSize, unsigned int seed, float targetAmountVisitable)
{
	Environment* env = new Environment(xSize, ySize, zSize);
	int amount = static_cast<int>((xSize * ySize * zSize) * targetAmountVisitable);
	int x = 0, y = 0, z = 0;
	int lastPercentage = 0;
	int percentage = 0;
	int tries = 0;
	std::mt19937 rng(seed);

	std::cout << "-=@ Generating Environment @=-\n";
	std::cout << "-- Seed: " << seed << "\n";
	std::cout << "-- Size: { " << xSize << ", " << ySize << ", " << zSize << " }\n";
	std::cout << "-- Visitable: " << (targetAmountVisitable * 100.0f) << "%\n";

	auto before = std::chrono::system_clock::now();

	for (int i = 0; i < amount; i++)
	{
		do
		{
			if (tries >= 1000)
				break;
			
			x = rng() % xSize;
			y = rng() % ySize;
			z = rng() % zSize;

			tries++;
		}
		while (env->canVisit(x, y, z));

		env->setCanVisit(x, y, z, true);
		tries = 0;

		//Write progress to console.
		percentage = static_cast<int>((static_cast<float>(i + 1) / amount) * 100.0f);
		if (percentage != lastPercentage)
			std::cout << (i + 1) << " / " << amount << " (" << percentage << "%) complete...\n";
		lastPercentage = percentage;
	}

	auto after = std::chrono::system_clock::now();
	long long time = std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count();

	std::cout << "-- DONE! Took " << time << " ms.\n";
	std::cout << "-- Actual visitable amount: " << (env->getAmountVisitable() * 100.0f) << "%\n";
	return env;
}