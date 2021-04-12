#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector>
#include <Environment.h>
#include <Position.h>

class Algorithm
{
public:
	Algorithm() {}
	virtual ~Algorithm() {}

	virtual bool pathfind(Environment* env, const Position& start, const Position& end, std::vector<Position>& outPath) = 0;
};

#endif // !ALGORITHM_H
