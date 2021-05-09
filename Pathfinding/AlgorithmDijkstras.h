#ifndef ALGORITHM_DIJK_H
#define ALGORITHM_DIJK_H
#include "Algorithm.h"
class AlgorithmDijkstras : public Algorithm
{
private:
	struct Node
	{
		Node() : pos(), fCost(0), gCost(0), hCost(0), parent(nullptr) {}
		Node(const Position& pos) : pos(pos), fCost(0), gCost(0), hCost(0), parent(nullptr) {}
		Node(const Position& pos, float fCost, float gCost, float hCost, Node* parent) : pos(pos), fCost(fCost), gCost(gCost), hCost(hCost), parent(parent) {}

		Position pos;
		float fCost, gCost, hCost;
		Node* parent;
		bool visited = false;
		bool included = false;
		float totalCost = 0;
	};

	struct NodeComparer
	{
		bool operator()(Node* first, Node* second) const
		{
			return first->fCost < second->fCost || (first->fCost == second->fCost && first->hCost < second->hCost);
		}
	};

public:
	AlgorithmDijkstras();
	virtual ~AlgorithmDijkstras();

	const char* getName() const { return "Dijkstra"; }

	bool pathfind(Environment* env, const Position& start, const Position& end, std::vector<Position>& outPath) override;
};

#endif // !ALGORITHM_DIJK_H