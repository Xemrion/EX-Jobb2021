#ifndef ALGORITHM_DIJK2_H
#define ALGORITHM_DIJK2_H
#include "Algorithm.h"
class AlgorithmDijkstras2 : public Algorithm
{
private:
	struct Node
	{
		Node() : pos(), cost(0), parent(nullptr) {}
		Node(const Position& pos) : pos(pos), cost(0), parent(nullptr) {}
		Node(const Position& pos, float cost, Node* parent) : pos(pos), cost(cost), parent(parent) {}

		Position pos;
		float cost;
		Node* parent;
	};

	struct NodeComparer
	{
		bool operator()(Node* first, Node* second) const
		{
			return first->cost < second->cost;
		}
	};

public:
	AlgorithmDijkstras2();
	virtual ~AlgorithmDijkstras2();

	const char* getName() const { return "Dijkstra2"; }

	bool pathfind(Environment* env, const Position& start, const Position& end, std::vector<Position>& outPath) override;
};

#endif // !ALGORITHM_DIJK2_H