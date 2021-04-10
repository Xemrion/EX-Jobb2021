#ifndef GENERETOR_H
#define GENERATOR_H

#include "Environment.h"

class Generator
{
public:
	static Environment* generateEnvironment(int xSize, int ySize, int zSize, unsigned int seed, float targetAmountVisitable = 0.5f);
};

#endif // !GENERETOR_H
